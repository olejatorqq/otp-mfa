#include "account_manager.h"
#include "encryption_utils.h"
#include "recovery_manager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QDateTime>
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
const char  AccountManager::xorKey    = 0x5A;

// Получение единственного экземпляра AccountManager
AccountManager& AccountManager::instance()
{
    static AccountManager mgr;
    return mgr;
}

// Приватный конструктор
AccountManager::AccountManager()
    : QObject(nullptr),
    networkManager(new QNetworkAccessManager(this))
{
    // Открываем или создаём базу данных SQLite
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

// Вспомогательные функции для чтения/записи в таблицу settings
QByteArray AccountManager::getSettingValueInternal(const QString &key) const
{
    QSqlQuery query(db);
    query.prepare("SELECT value FROM settings WHERE key = :k");
    query.bindValue(":k", key);

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

QByteArray AccountManager::getSettingValue(const QString &key) const
{
    return getSettingValueInternal(key);
}

bool AccountManager::setSettingValue(const QString &key, const QByteArray &value)
{
    // Проверяем, существует ли уже ключ
    {
        QSqlQuery checkQuery(db);
        checkQuery.prepare("SELECT COUNT(*) FROM settings WHERE key = :k");
        checkQuery.bindValue(":k", key);
        if (!checkQuery.exec()) {
            qWarning() << "Не удалось проверить существование ключа в settings:"
                       << checkQuery.lastError().text();
            return false;
        }

        if (checkQuery.next()) {
            int count = checkQuery.value(0).toInt();
            if (count > 0) {
                // Обновляем существующий ключ
                QSqlQuery updateQ(db);
                updateQ.prepare("UPDATE settings SET value = :val WHERE key = :k");
                updateQ.bindValue(":val", value);
                updateQ.bindValue(":k",   key);

                if (!updateQ.exec()) {
                    qWarning() << "Не удалось обновить значение в settings:"
                               << updateQ.lastError().text();
                    return false;
                }
                return true;
            }
        }
    }
    // Вставляем новый ключ
    QSqlQuery insertQ(db);
    insertQ.prepare("INSERT INTO settings (key, value) VALUES (:k, :val)");
    insertQ.bindValue(":k", key);
    insertQ.bindValue(":val", value);

    if (!insertQ.exec()) {
        qWarning() << "Не удалось вставить запись в settings:"
                   << insertQ.lastError().text();
        return false;
    }
    return true;
}

// Загрузка хэша мастер-пароля из базы данных
bool AccountManager::loadStoredMasterPasswordHash()
{
    QByteArray hash = getSettingValueInternal("masterPasswordHash");
    if (!hash.isEmpty()) {
        storedHash = hash;
        return true;
    }
    return false;
}

// Сохранение хэша мастер-пароля в базе данных
bool AccountManager::storeMasterPasswordHash(const QString &newPass)
{
    QByteArray hash = QCryptographicHash::hash(newPass.toUtf8(), QCryptographicHash::Sha256);
    storedHash = hash;

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
        // Если пароль ещё не установлен — проверка провалилась
        return false;
    }
    QByteArray inputHash = QCryptographicHash::hash(inputPass.toUtf8(), QCryptographicHash::Sha256);
    return (inputHash == storedHash);
}

// Установка Recovery Key
bool AccountManager::setRecoveryKey(const QString &recoveryKey)
{
    QByteArray rKeyHash = RecoveryManager::instance().hashRecoveryKey(recoveryKey);

    if (!setSettingValue("recoveryKeyHash", rKeyHash)) {
        qWarning() << "Не удалось сохранить recoveryKeyHash";
        return false;
    }
    logEvent("Установлен/изменён Recovery Key");
    return true;
}

// Сброс мастер-пароля с использованием Recovery Key
bool AccountManager::resetMasterPasswordUsingRecoveryKey(const QString &recoveryKey, const QString &newMasterPassword)
{
    QByteArray storedRecoveryHash = getSettingValueInternal("recoveryKeyHash");
    if (storedRecoveryHash.isEmpty()) {
        qWarning() << "Recovery Key ещё не установлен!";
        return false;
    }

    if (!RecoveryManager::instance().verifyRecoveryKey(recoveryKey, storedRecoveryHash)) {
        qWarning() << "Введён неверный Recovery Key!";
        return false;
    }

    if (!storeMasterPasswordHash(newMasterPassword)) {
        qWarning() << "Не удалось сохранить новый мастер-пароль!";
        return false;
    }

    logEvent("Мастер-пароль восстановлен с помощью Recovery Key");
    return true;
}

// Логирование событий
void AccountManager::logEvent(const QString &eventDescription)
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
        qWarning() << "Не удалось выполнить SELECT из accounts:"
                   << query.lastError().text();
        return accounts;
    }

    while (query.next()) {
        Account acc;
        acc.name      = query.value(0).toString();
        QByteArray encSecret = query.value(1).toByteArray();

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
        " VALUES (:n, :s, :a, :d, :p, :t, :c)"
        );
    query.bindValue(":n",   account.name);

    QByteArray encSecret = EncryptionUtils::instance().encrypt(account.secret);
    query.bindValue(":s", encSecret);

    query.bindValue(":a",   account.algorithm);
    query.bindValue(":d",   account.digits);
    query.bindValue(":p",   account.period);
    query.bindValue(":t",   account.type);
    query.bindValue(":c",   account.counter);

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
void AccountManager::updateAccount(const QString& accountName, const Account& updatedAccount)
{
    QSqlQuery query(db);
    query.prepare(
        "UPDATE accounts SET name = :newName, secret = :secret, algorithm = :algo, "
        "digits = :d, period = :p, type = :t, counter = :c "
        "WHERE name = :oldName"
        );
    query.bindValue(":newName", updatedAccount.name);

    QByteArray encSecret = EncryptionUtils::instance().encrypt(updatedAccount.secret);
    query.bindValue(":secret", encSecret);

    query.bindValue(":algo",   updatedAccount.algorithm);
    query.bindValue(":d",      updatedAccount.digits);
    query.bindValue(":p",      updatedAccount.period);
    query.bindValue(":t",      updatedAccount.type);
    query.bindValue(":c",      updatedAccount.counter);
    query.bindValue(":oldName", accountName);

    if (!query.exec()) {
        qWarning() << "Не удалось обновить аккаунт:" << query.lastError().text();
    } else {
        logEvent(QString("Обновлён аккаунт: %1 на %2").arg(accountName, updatedAccount.name));
    }
}

// Сетевые методы
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
        logEvent("Получены данные с сервера");
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

    QList<QSslCertificate> certs = reply->sslConfiguration().peerCertificateChain();
    if (!certs.isEmpty()) {
        QSslCertificate cert = certs.first();
        QByteArray certHash = QCryptographicHash::hash(cert.toDer(), QCryptographicHash::Sha256).toHex();
        QByteArray expected  = getExpectedCertHash();

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
