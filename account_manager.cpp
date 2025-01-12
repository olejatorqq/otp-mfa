#include "account_manager.h"
#include "encryption_utils.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QDateTime>
#include <QSettings>
#include <QCryptographicHash>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QSslCertificate>
#include <QNetworkReply>
#include <QSslError>

// Инициализация статических членов класса
const char* AccountManager::hashPart1 = "980afe02";
const char* AccountManager::hashPart2 = "39f05b57";
const char* AccountManager::hashPart3 = "d83b4c38";
const char* AccountManager::hashPart4 = "06de0ea7";
const char* AccountManager::hashPart5 = "4ec4718f";
const char* AccountManager::hashPart6 = "e1c0ec44";
const char* AccountManager::hashPart7 = "4ad8f48d";
const char* AccountManager::hashPart8 = "7683a89d";

// XOR ключ
const char AccountManager::xorKey = 0x5A;

// Получение единственного экземпляра AccountManager
AccountManager& AccountManager::instance()
{
    static AccountManager instance;
    return instance;
}

// Приватный конструктор
AccountManager::AccountManager()
    : QObject(nullptr),
    networkManager(new QNetworkAccessManager(this))
{
    // Инициализируем или открываем базу данных SQLite
    if (QSqlDatabase::contains("OTPConnection")) {
        db = QSqlDatabase::database("OTPConnection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", "OTPConnection");
        db.setDatabaseName("accounts.db");
    }

    if (!db.open()) {
        qWarning() << "Не удалось открыть базу данных:" << db.lastError().text();
    } else {
        initializeDatabase();
    }

    // Подключаем сигналы для сетевых операций
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &AccountManager::onNetworkReplyFinished);
    connect(networkManager, &QNetworkAccessManager::sslErrors,
            this, &AccountManager::onSslErrors);

    // Пытаемся считать хэш мастер-пароля (если он есть)
    isMasterPasswordSet = loadStoredMasterPasswordHash();
}

// Деструктор
AccountManager::~AccountManager()
{
    if (db.isOpen()) {
        db.close();
    }
}

// Инициализация базы данных
void AccountManager::initializeDatabase()
{
    QSqlQuery query(db);

    // Создание таблицы accounts, если она не существует
    QString createAccountsTable = R"(
        CREATE TABLE IF NOT EXISTS accounts (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            secret TEXT NOT NULL,
            algorithm TEXT NOT NULL,
            digits INTEGER NOT NULL,
            period INTEGER NOT NULL,
            type TEXT NOT NULL,
            counter INTEGER DEFAULT 0
        )
    )";

    if (!query.exec(createAccountsTable)) {
        qWarning() << "Не удалось создать таблицу accounts:"
                   << query.lastError().text();
    }

    // Создание таблицы settings, если она не существует
    QString createSettingsTable = R"(
        CREATE TABLE IF NOT EXISTS settings (
            key TEXT PRIMARY KEY,
            value BLOB NOT NULL
        )
    )";

    if (!query.exec(createSettingsTable)) {
        qWarning() << "Не удалось создать таблицу settings:"
                   << query.lastError().text();
    }
}

// Получение значения из таблицы settings по ключу
QByteArray AccountManager::getSettingValue(const QString &key) const
{
    QSqlQuery query(db);
    query.prepare("SELECT value FROM settings WHERE key = :key");
    query.bindValue(":key", key);

    if (!query.exec()) {
        qWarning() << "Не удалось выполнить запрос к таблице settings:"
                   << query.lastError().text();
        return QByteArray();
    }

    if (query.next()) {
        return query.value(0).toByteArray();
    }

    return QByteArray();
}

// Установка значения в таблице settings по ключу
bool AccountManager::setSettingValue(const QString &key, const QByteArray &value)
{
    QSqlQuery query(db);
    // Проверяем, существует ли уже ключ
    query.prepare("SELECT COUNT(*) FROM settings WHERE key = :key");
    query.bindValue(":key", key);

    if (!query.exec()) {
        qWarning() << "Не удалось проверить существование ключа в settings:"
                   << query.lastError().text();
        return false;
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        if (count > 0) {
            // Обновляем существующий ключ
            query.prepare("UPDATE settings SET value = :value WHERE key = :key");
            query.bindValue(":value", value);
            query.bindValue(":key", key);

            if (!query.exec()) {
                qWarning() << "Не удалось обновить значение в settings:"
                           << query.lastError().text();
                return false;
            }
        } else {
            // Вставляем новый ключ
            query.prepare("INSERT INTO settings (key, value) VALUES (:key, :value)");
            query.bindValue(":key", key);
            query.bindValue(":value", value);

            if (!query.exec()) {
                qWarning() << "Не удалось вставить новое значение в settings:"
                           << query.lastError().text();
                return false;
            }
        }
    }

    return true;
}

// Загрузка хэша мастер-пароля из базы данных
bool AccountManager::loadStoredMasterPasswordHash()
{
    QByteArray hash = getSettingValue("masterPasswordHash");

    if (!hash.isEmpty()) {
        storedHash = hash;
        isMasterPasswordSet = true;
        return true;
    }
    isMasterPasswordSet = false;
    return false;
}

// Сохранение хэша мастер-пароля в базе данных
bool AccountManager::storeMasterPasswordHash(const QString &newPass)
{
    // Хэшируем новый пароль с помощью SHA-256
    QByteArray hash = QCryptographicHash::hash(newPass.toUtf8(), QCryptographicHash::Sha256);
    storedHash = hash;

    // Сохраняем хэш в таблице settings
    if (!setSettingValue("masterPasswordHash", storedHash)) {
        qWarning() << "Не удалось сохранить хэш мастер-пароля в settings.";
        return false;
    }

    isMasterPasswordSet = true;
    return true;
}

// Проверка введённого мастер-пароля
bool AccountManager::verifyMasterPassword(const QString &inputPass)
{
    if (!isMasterPasswordSet) {
        // Пароль ещё не задан => проверка провалилась
        return false;
    }

    // Хэшируем введённый пользователем пароль
    QByteArray inputHash = QCryptographicHash::hash(inputPass.toUtf8(), QCryptographicHash::Sha256);

    // Сравниваем с сохранённым хэшем
    return (inputHash == storedHash);
}

// Логирование событий (пример: вывод в консоль)
void AccountManager::logEvent(const QString& eventDescription)
{
    qDebug() << "[LOG]" << eventDescription;
}

// Получение списка аккаунтов
QList<Account> AccountManager::getAccounts() const
{
    QList<Account> accounts;
    QSqlQuery query(db);
    query.prepare("SELECT name, secret, algorithm, digits, period, type, counter FROM accounts");

    if (!query.exec()) {
        qWarning() << "Не удалось выполнить запрос к таблице accounts:"
                   << query.lastError().text();
        return accounts;
    }

    while (query.next()) {
        Account acc;
        acc.name = query.value(0).toString();
        QByteArray encSecret = query.value(1).toByteArray();

        // Расшифровка секрета
        QString decSecret = EncryptionUtils::instance().decrypt(encSecret);
        if (decSecret.isEmpty()) {
            qWarning() << "Не удалось расшифровать секрет аккаунта:" << acc.name;
            continue;
        }
        acc.secret    = decSecret;
        acc.algorithm = query.value(2).toString();
        acc.digits    = query.value(3).toInt();
        acc.period    = query.value(4).toInt();
        acc.type      = query.value(5).toString();
        acc.counter   = query.value(6).toULongLong();

        accounts.append(acc);
    }

    return accounts;
}

// Добавление нового аккаунта
void AccountManager::addAccount(const Account& account)
{
    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO accounts (name, secret, algorithm, digits, period, type, counter)"
        "VALUES (:name, :secret, :algo, :digits, :period, :type, :cnt)"
        );
    query.bindValue(":name",   account.name);

    QByteArray encSecret = EncryptionUtils::instance().encrypt(account.secret);
    query.bindValue(":secret", encSecret);

    query.bindValue(":algo",   account.algorithm);
    query.bindValue(":digits", account.digits);
    query.bindValue(":period", account.period);
    query.bindValue(":type",   account.type);
    query.bindValue(":cnt",    account.counter);

    if (!query.exec()) {
        qWarning() << "Не удалось добавить аккаунт:" << query.lastError().text();
    } else {
        logEvent("Добавлен аккаунт: " + account.name);
    }
}

// Удаление аккаунта
void AccountManager::deleteAccount(const QString& accountName)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM accounts WHERE name = :nm");
    query.bindValue(":nm", accountName);

    if (!query.exec()) {
        qWarning() << "Не удалось удалить аккаунт:" << query.lastError().text();
    } else {
        logEvent("Удалён аккаунт: " + accountName);
    }
}

// Обновление аккаунта
void AccountManager::updateAccount(const QString& accountName,
                                   const Account& updatedAccount)
{
    QSqlQuery query(db);
    query.prepare(
        "UPDATE accounts SET name = :newName, secret = :secret, algorithm = :algo, "
        "digits = :digits, period = :period, type = :type, counter = :cnt "
        "WHERE name = :oldName"
        );
    query.bindValue(":newName", updatedAccount.name);

    QByteArray encSecret = EncryptionUtils::instance().encrypt(updatedAccount.secret);
    query.bindValue(":secret", encSecret);

    query.bindValue(":algo",   updatedAccount.algorithm);
    query.bindValue(":digits", updatedAccount.digits);
    query.bindValue(":period", updatedAccount.period);
    query.bindValue(":type",   updatedAccount.type);
    query.bindValue(":cnt",    updatedAccount.counter);
    query.bindValue(":oldName", accountName);

    if (!query.exec()) {
        qWarning() << "Не удалось обновить аккаунт:" << query.lastError().text();
    } else {
        logEvent(QString("Обновлён аккаунт: %1 на %2").arg(accountName, updatedAccount.name));
    }
}

// Выполнение сетевого запроса
void AccountManager::fetchDataFromServer(const QUrl &url)
{
    if (!url.isValid() || url.scheme() != "https") {
        emit fetchError("Неверный URL или не используется HTTPS.");
        return;
    }

    QNetworkRequest req(url);
    QSslConfiguration sslConfig = req.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer);
    sslConfig.setProtocol(QSsl::TlsV1_2);
    req.setSslConfiguration(sslConfig);

    networkManager->get(req);
}

// Обработка завершения сетевого запроса
void AccountManager::onNetworkReplyFinished(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        qDebug() << "Ответ сервера:" << data;
        emit dataFetched(data);
    } else {
        qWarning() << "Ошибка сетевого запроса:" << reply->errorString();
        emit fetchError(reply->errorString());
    }

    reply->deleteLater();
}

// Обработка SSL ошибок
void AccountManager::onSslErrors(QNetworkReply* reply, const QList<QSslError> &errors)
{
    for (const QSslError &err : errors) {
        qWarning() << "SSL ошибка:" << err.errorString();
    }

    // Пример сертификатного пиннинга
    QList<QSslCertificate> certs = reply->sslConfiguration().peerCertificateChain();
    if (!certs.isEmpty()) {
        QSslCertificate cert = certs.first();
        QByteArray certHash = QCryptographicHash::hash(cert.toDer(), QCryptographicHash::Sha256).toHex();
        QByteArray expected = getExpectedCertHash();
        if (certHash != expected) {
            qWarning() << "Неверный сертификат сервера!";
            emit fetchError("Неверный сертификат сервера.");
            reply->abort();
            return;
        }
    } else {
        qWarning() << "Сертификаты сервера не найдены!";
        emit fetchError("Сертификаты сервера не найдены.");
        reply->abort();
        return;
    }
    // Сертификат прошёл проверку
}

// Получение ожидаемого хэша сертификата (пример)
QByteArray AccountManager::getExpectedCertHash() const
{
    QByteArray hx;
    hx.append(hashPart1);
    hx.append(hashPart2);
    hx.append(hashPart3);
    hx.append(hashPart4);
    hx.append(hashPart5);
    hx.append(hashPart6);
    hx.append(hashPart7);
    hx.append(hashPart8);

    QByteArray bin = QByteArray::fromHex(hx);
    for (int i = 0; i < bin.size(); ++i) {
        bin[i] = bin[i] ^ xorKey;
    }
    return bin.toHex().toLower();
}
