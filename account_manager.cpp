#include "account_manager.h"
#include "encryption_utils.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QDateTime>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QSslCertificate>
#include <QCryptographicHash>

// Разбиение хэша на части - Поменяй логику!!!
static const char* hashPart1 = "980afe02";
static const char* hashPart2 = "39f05b57";
static const char* hashPart3 = "d83b4c38";
static const char* hashPart4 = "06de0ea7";
static const char* hashPart5 = "4ec4718f";
static const char* hashPart6 = "e1c0ec44";
static const char* hashPart7 = "4ad8f48d";
static const char* hashPart8 = "7683a89d";

// Приватный ключ для XOR
static const char xorKey = 0x5A; // Пример ключа

AccountManager& AccountManager::instance() {
    static AccountManager instance;
    return instance;
}

AccountManager::AccountManager() : QObject(nullptr), networkManager(new QNetworkAccessManager(this)) {
    // Проверяем, существует ли подключение с именем "OTPConnection"
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

    // Подключаем сигналы для обработки ответов и SSL ошибок
    connect(networkManager, &QNetworkAccessManager::finished, this, &AccountManager::onNetworkReplyFinished);
    connect(networkManager, &QNetworkAccessManager::sslErrors, this, &AccountManager::onSslErrors);
}

AccountManager::~AccountManager() {
    if (db.isOpen()) {
        db.close();
    }
}

void AccountManager::initializeDatabase() {
    QSqlQuery query(db);

    // Создание таблицы accounts
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
        qWarning() << "Не удалось создать таблицу accounts:" << query.lastError().text();
    }
}

void AccountManager::logEvent(const QString& eventDescription) {
    // Логирования событий
    Q_UNUSED(eventDescription);
}

QList<Account> AccountManager::getAccounts() const {
    QList<Account> accounts;
    QSqlQuery query(db);
    query.prepare("SELECT name, secret, algorithm, digits, period, type, counter FROM accounts");

    if (!query.exec()) {
        qWarning() << "Не удалось выполнить запрос к таблице accounts:" << query.lastError().text();
        return accounts;
    }

    while (query.next()) {
        Account account;
        account.name = query.value(0).toString();

        // Расшифровываем секрет
        QByteArray encryptedSecret = query.value(1).toByteArray();
        QString decryptedSecret = EncryptionUtils::instance().decrypt(encryptedSecret);
        if (decryptedSecret.isEmpty()) {
            qWarning() << "Не удалось расшифровать секрет для аккаунта:" << account.name;
            continue;
        }
        account.secret = decryptedSecret;

        account.algorithm = query.value(2).toString();
        account.digits = query.value(3).toInt();
        account.period = query.value(4).toInt();
        account.type = query.value(5).toString();
        account.counter = query.value(6).toULongLong();
        accounts.append(account);
    }

    return accounts;
}

void AccountManager::addAccount(const Account& account) {
    if (!EncryptionUtils::instance().isMasterPasswordSet()) {
        qWarning() << "Мастер-пароль не установлен.";
        return;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO accounts (name, secret, algorithm, digits, period, type, counter) "
                  "VALUES (:name, :secret, :algorithm, :digits, :period, :type, :counter)");
    query.bindValue(":name", account.name);

    // Шифруем секрет перед сохранением
    QByteArray encryptedSecret = EncryptionUtils::instance().encrypt(account.secret);
    query.bindValue(":secret", encryptedSecret);

    query.bindValue(":algorithm", account.algorithm);
    query.bindValue(":digits", account.digits);
    query.bindValue(":period", account.period);
    query.bindValue(":type", account.type);
    query.bindValue(":counter", account.counter);

    if (!query.exec()) {
        qWarning() << "Не удалось добавить аккаунт:" << query.lastError().text();
    } else {
        // Логируем событие добавления аккаунта
        logEvent("Добавлен аккаунт: " + account.name);
    }
}

void AccountManager::deleteAccount(const QString& accountName) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM accounts WHERE name = :name");
    query.bindValue(":name", accountName);

    if (!query.exec()) {
        qWarning() << "Не удалось удалить аккаунт:" << query.lastError().text();
    } else {
        // Логируем событие удаления аккаунта
        logEvent("Удален аккаунт: " + accountName);
    }
}

void AccountManager::updateAccount(const QString& accountName, const Account& updatedAccount) {
    if (!EncryptionUtils::instance().isMasterPasswordSet()) {
        qWarning() << "Мастер-пароль не установлен.";
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE accounts SET name = :newName, secret = :secret, algorithm = :algorithm, "
                  "digits = :digits, period = :period, type = :type, counter = :counter "
                  "WHERE name = :oldName");
    query.bindValue(":newName", updatedAccount.name);

    // Шифруем секрет перед сохранением
    QByteArray encryptedSecret = EncryptionUtils::instance().encrypt(updatedAccount.secret);
    query.bindValue(":secret", encryptedSecret);

    query.bindValue(":algorithm", updatedAccount.algorithm);
    query.bindValue(":digits", updatedAccount.digits);
    query.bindValue(":period", updatedAccount.period);
    query.bindValue(":type", updatedAccount.type);
    query.bindValue(":counter", updatedAccount.counter);
    query.bindValue(":oldName", accountName);

    if (!query.exec()) {
        qWarning() << "Не удалось обновить аккаунт:" << query.lastError().text();
    } else {
        // Логируем событие обновления аккаунта
        logEvent("Обновлен аккаунт: " + accountName + " на " + updatedAccount.name);
    }
}

bool AccountManager::verifyMasterPassword() {
    // Реализация проверки мастер-пароля (чекни)
    return true;
}

void AccountManager::fetchDataFromServer(const QUrl &url) {
    if (!url.isValid() || url.scheme() != "https") {
        emit fetchError("Неверный URL или не используется HTTPS.");
        return;
    }

    QNetworkRequest request(url);

    // Настройка SSL конфигурации
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer); // Верификация сертификата сервера
    sslConfig.setProtocol(QSsl::TlsV1_2); // Установка версии протокола TLS
    request.setSslConfiguration(sslConfig);

    // Выполнение GET-запроса
    networkManager->get(request);
}

void AccountManager::onNetworkReplyFinished(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        qDebug() << "Ответ сервера:" << responseData;
        emit dataFetched(responseData);
    } else {
        qWarning() << "Ошибка сетевого запроса:" << reply->errorString();
        emit fetchError(reply->errorString());
    }

    reply->deleteLater();
}

void AccountManager::onSslErrors(QNetworkReply* reply, const QList<QSslError> &errors) {
    for (const QSslError &error : errors) {
        qWarning() << "SSL ошибка:" << error.errorString();
    }

    // Реализация сертификатного пиннинга
    QList<QSslCertificate> certs = reply->sslConfiguration().peerCertificateChain();
    if (!certs.isEmpty()) {
        QSslCertificate serverCert = certs.first();
        QByteArray certHash = QCryptographicHash::hash(serverCert.toDer(), QCryptographicHash::Sha256).toHex();

        // Восстанавливаем ожидаемый хэш
        QByteArray expectedHash = getExpectedCertHash();

        if (certHash != expectedHash) {
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
}

QByteArray AccountManager::getExpectedCertHash() const {
    // Обфусцированный хэш сертификата
    QByteArray obfuscatedHash;

    // Восстанавливаем хэш из частей и применяем XOR
    // Шляпа, поправь
    QByteArray fullHash;
    fullHash.append(hashPart1);
    fullHash.append(hashPart2);
    fullHash.append(hashPart3);
    fullHash.append(hashPart4);
    fullHash.append(hashPart5);
    fullHash.append(hashPart6);
    fullHash.append(hashPart7);
    fullHash.append(hashPart8);

    // Применяем XOR к каждому байту
    for (char byte : fullHash) {
        obfuscatedHash.append(byte ^ xorKey);
    }

    // Расшифровка хэша
    QByteArray decryptedHash;
    for (char byte : obfuscatedHash) {
        decryptedHash.append(byte ^ xorKey);
    }

    return decryptedHash.toLower();
}
