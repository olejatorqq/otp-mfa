#include "account_manager.h"
#include "encryption_utils.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

AccountManager::AccountManager() {
    // Инициализируем подключение к базе данных
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("accounts.db");

    if (!db.open()) {
        qWarning() << "Failed to open database:" << db.lastError().text();
    } else {
        initializeDatabase();
    }
}

AccountManager::~AccountManager() {
    if (db.isOpen()) {
        db.close();
    }
}

void AccountManager::initializeDatabase() {
    QSqlQuery query;

    // Создаем таблицу, если ее еще нет
    QString createTable = R"(
        CREATE TABLE IF NOT EXISTS accounts (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            secret TEXT NOT NULL,
            algorithm TEXT NOT NULL,
            counter INTEGER DEFAULT 0
        )
    )";

    // Создаем таблицу key_verification
    QString createKeyVerificationTable = R"(
        CREATE TABLE IF NOT EXISTS key_verification (
            id INTEGER PRIMARY KEY,
            encrypted_value TEXT NOT NULL
        )
    )";
    if (!query.exec(createKeyVerificationTable)) {
        qWarning() << "Failed to create key_verification table:" << query.lastError().text();
    }

    if (!query.exec(createTable)) {
        qWarning() << "Failed to create table:" << query.lastError().text();
    }

}

QList<Account> AccountManager::getAccounts() const {
    QList<Account> accounts;
    QSqlQuery query("SELECT name, secret, algorithm, counter FROM accounts");

    while (query.next()) {
        Account account;
        account.name = query.value(0).toString();

        // Расшифровываем секрет
        QByteArray encryptedSecret = query.value(1).toByteArray();
        QString decryptedSecret = EncryptionUtils::instance().decrypt(encryptedSecret);
        if (decryptedSecret.isEmpty()) {
            qWarning() << "Failed to decrypt secret for account:" << account.name;
            continue; // Пропускаем этот аккаунт
        }
        account.secret = decryptedSecret;

        account.algorithm = query.value(2).toString();
        account.counter = query.value(3).toInt();
        accounts.append(account);
    }

    return accounts;
}


void AccountManager::addAccount(const Account& account) {
    if (!EncryptionUtils::instance().isMasterPasswordSet()) {
        qWarning() << "Master password is not set.";
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO accounts (name, secret, algorithm, counter) VALUES (:name, :secret, :algorithm, :counter)");
    query.bindValue(":name", account.name);

    // Шифруем секрет перед сохранением
    QByteArray encryptedSecret = EncryptionUtils::instance().encrypt(account.secret);
    query.bindValue(":secret", encryptedSecret);

    query.bindValue(":algorithm", account.algorithm);
    query.bindValue(":counter", account.counter);

    if (!query.exec()) {
        qWarning() << "Failed to add account:" << query.lastError().text();
    }
}

bool AccountManager::verifyMasterPassword() {
    QSqlQuery query;
    query.prepare("SELECT encrypted_value FROM key_verification WHERE id = 1");
    if (query.exec()) {
        if (query.next()) {
            // Проверочное значение существует, пытаемся расшифровать
            QByteArray encryptedValue = query.value(0).toByteArray();
            QString decryptedValue = EncryptionUtils::instance().decrypt(encryptedValue);
            if (decryptedValue == "verification") {
                return true; // Мастер-пароль верен
            } else {
                return false; // Неверный мастер-пароль
            }
        } else {
            // Проверочное значение не существует, создаём его
            QByteArray verificationValue = "verification";
            QByteArray encryptedValue = EncryptionUtils::instance().encrypt(verificationValue);
            query.prepare("INSERT INTO key_verification (id, encrypted_value) VALUES (1, :encrypted_value)");
            query.bindValue(":encrypted_value", encryptedValue);
            if (!query.exec()) {
                qWarning() << "Failed to insert key verification value:" << query.lastError().text();
                return false;
            }
            return true; // Мастер-пароль установлен
        }
    } else {
        qWarning() << "Failed to query key_verification table:" << query.lastError().text();
        return false;
    }
}

