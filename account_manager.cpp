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

static const char* hashPart1 = "980afe02";
static const char* hashPart2 = "39f05b57";
static const char* hashPart3 = "d83b4c38";
static const char* hashPart4 = "06de0ea7";
static const char* hashPart5 = "4ec4718f";
static const char* hashPart6 = "e1c0ec44";
static const char* hashPart7 = "4ad8f48d";
static const char* hashPart8 = "7683a89d";

// Примерный ключ для XOR
static const char xorKey = 0x5A;

AccountManager& AccountManager::instance() {
    static AccountManager instance;
    return instance;
}

AccountManager::AccountManager()
    : QObject(nullptr),
    networkManager(new QNetworkAccessManager(this))
{
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

    connect(networkManager, &QNetworkAccessManager::finished,
            this, &AccountManager::onNetworkReplyFinished);
    connect(networkManager, &QNetworkAccessManager::sslErrors,
            this, &AccountManager::onSslErrors);
}

AccountManager::~AccountManager() {
    if (db.isOpen()) {
        db.close();
    }
}

void AccountManager::initializeDatabase() {
    QSqlQuery query(db);

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
}

void AccountManager::logEvent(const QString& eventDescription) {
    // Здесь можно реализовать логику логирования (в файл, БД и т.п.)
    qDebug() << "[LOG]" << eventDescription;
}

QList<Account> AccountManager::getAccounts() const {
    QList<Account> accounts;
    QSqlQuery query(db);
    query.prepare("SELECT name, secret, algorithm, digits, period, type, counter FROM accounts");

    if (!query.exec()) {
        qWarning() << "Не удалось выполнить запрос к таблице accounts:"
                   << query.lastError().text();
        return accounts;
    }

    while (query.next()) {
        Account account;
        account.name = query.value(0).toString();

        QByteArray encryptedSecret = query.value(1).toByteArray();
        QString decryptedSecret = EncryptionUtils::instance().decrypt(encryptedSecret);
        if (decryptedSecret.isEmpty()) {
            qWarning() << "Не удалось расшифровать секрет для аккаунта:"
                       << account.name;
            continue;
        }
        account.secret    = decryptedSecret;
        account.algorithm = query.value(2).toString();
        account.digits    = query.value(3).toInt();
        account.period    = query.value(4).toInt();
        account.type      = query.value(5).toString();
        account.counter   = query.value(6).toULongLong();

        accounts.append(account);
    }

    return accounts;
}

void AccountManager::addAccount(const Account& account) {
    // Убираем проверку isMasterPasswordSet
    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO accounts (name, secret, algorithm, digits, period, type, counter) "
        "VALUES (:name, :secret, :algorithm, :digits, :period, :type, :counter)"
        );
    query.bindValue(":name", account.name);

    QByteArray encryptedSecret = EncryptionUtils::instance().encrypt(account.secret);
    query.bindValue(":secret", encryptedSecret);

    query.bindValue(":algorithm", account.algorithm);
    query.bindValue(":digits",    account.digits);
    query.bindValue(":period",    account.period);
    query.bindValue(":type",      account.type);
    query.bindValue(":counter",   account.counter);

    if (!query.exec()) {
        qWarning() << "Не удалось добавить аккаунт:"
                   << query.lastError().text();
    } else {
        logEvent("Добавлен аккаунт: " + account.name);
    }
}

void AccountManager::deleteAccount(const QString& accountName) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM accounts WHERE name = :name");
    query.bindValue(":name", accountName);

    if (!query.exec()) {
        qWarning() << "Не удалось удалить аккаунт:"
                   << query.lastError().text();
    } else {
        logEvent("Удален аккаунт: " + accountName);
    }
}

void AccountManager::updateAccount(const QString& accountName,
                                   const Account& updatedAccount)
{
    // Убираем проверку isMasterPasswordSet
    QSqlQuery query(db);
    query.prepare(
        "UPDATE accounts SET name = :newName, secret = :secret, algorithm = :algorithm, "
        "digits = :digits, period = :period, type = :type, counter = :counter "
        "WHERE name = :oldName"
        );
    query.bindValue(":newName", updatedAccount.name);

    QByteArray encryptedSecret = EncryptionUtils::instance().encrypt(updatedAccount.secret);
    query.bindValue(":secret", encryptedSecret);

    query.bindValue(":algorithm", updatedAccount.algorithm);
    query.bindValue(":digits",    updatedAccount.digits);
    query.bindValue(":period",    updatedAccount.period);
    query.bindValue(":type",      updatedAccount.type);
    query.bindValue(":counter",   updatedAccount.counter);
    query.bindValue(":oldName",   accountName);

    if (!query.exec()) {
        qWarning() << "Не удалось обновить аккаунт:"
                   << query.lastError().text();
    } else {
        logEvent("Обновлен аккаунт: " + accountName + " на " + updatedAccount.name);
    }
}

bool AccountManager::verifyMasterPassword() {
    // Удаляем или возвращаем всегда true
    return true;
}

void AccountManager::fetchDataFromServer(const QUrl &url) {
    if (!url.isValid() || url.scheme() != "https") {
        emit fetchError("Неверный URL или не используется HTTPS.");
        return;
    }

    QNetworkRequest request(url);

    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer);
    sslConfig.setProtocol(QSsl::TlsV1_2);
    request.setSslConfiguration(sslConfig);

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

void AccountManager::onSslErrors(QNetworkReply* reply,
                                 const QList<QSslError> &errors)
{
    for (const QSslError &error : errors) {
        qWarning() << "SSL ошибка:" << error.errorString();
    }

    QList<QSslCertificate> certs = reply->sslConfiguration().peerCertificateChain();
    if (!certs.isEmpty()) {
        QSslCertificate serverCert = certs.first();
        QByteArray certHash = QCryptographicHash::hash(serverCert.toDer(),
                                                       QCryptographicHash::Sha256).toHex();
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
    // Сертификат прошёл проверку
}

QByteArray AccountManager::getExpectedCertHash() const
{
    // Допустим, меняем логику, чтобы она была проще
    // — Собираем одну большую hex-строку, декодируем и XOR'им.

    // Склеиваем 8 частей
    QByteArray hexString;
    hexString.append(hashPart1);
    hexString.append(hashPart2);
    hexString.append(hashPart3);
    hexString.append(hashPart4);
    hexString.append(hashPart5);
    hexString.append(hashPart6);
    hexString.append(hashPart7);
    hexString.append(hashPart8);

    // Декодируем hex -> бинарные данные
    QByteArray binaryData = QByteArray::fromHex(hexString);

    // Применяем XOR
    for (int i = 0; i < binaryData.size(); ++i) {
        binaryData[i] = binaryData[i] ^ xorKey;
    }

    // Возвращаем результат в виде hex
    return binaryData.toHex().toLower();
}
