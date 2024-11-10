#include "account_manager.h"
#include "encryption_utils.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QDateTime>

AccountManager& AccountManager::instance() {
    static AccountManager instance;
    return instance;
}

AccountManager::AccountManager() {
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
}

AccountManager::~AccountManager() {
    if (db.isOpen()) {
        db.close();
    }
}

void AccountManager::initializeDatabase() {
    QSqlQuery query(db);

    // Создаем таблицу accounts
    QString createAccountsTable = R"(
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

    // Создаем таблицу logs для журналирования действий
    QString createLogsTable = R"(
        CREATE TABLE IF NOT EXISTS logs (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp TEXT NOT NULL,
            event TEXT NOT NULL
        )
    )";

    if (!query.exec(createAccountsTable)) {
        qWarning() << "Не удалось создать таблицу accounts:" << query.lastError().text();
    }

    if (!query.exec(createKeyVerificationTable)) {
        qWarning() << "Не удалось создать таблицу key_verification:" << query.lastError().text();
    }

    if (!query.exec(createLogsTable)) {
        qWarning() << "Не удалось создать таблицу logs:" << query.lastError().text();
    }
}

void AccountManager::logEvent(const QString& eventDescription) {
    if (!db.isOpen()) {
        if (!db.open()) {
            qWarning() << "Не удалось открыть базу данных при записи события в журнал:" << db.lastError().text();
            return;
        }
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO logs (timestamp, event) VALUES (:timestamp, :event)");
    QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
    query.bindValue(":timestamp", timestamp);
    query.bindValue(":event", eventDescription);

    if (!query.exec()) {
        qWarning() << "Не удалось записать событие в журнал:" << query.lastError().text();
    }
}

QList<Account> AccountManager::getAccounts() const {
    QList<Account> accounts;
    QSqlQuery query(db);
    query.prepare("SELECT name, secret, algorithm, counter FROM accounts");

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
        qWarning() << "Мастер-пароль не установлен.";
        return;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO accounts (name, secret, algorithm, counter) VALUES (:name, :secret, :algorithm, :counter)");
    query.bindValue(":name", account.name);

    // Шифруем секрет перед сохранением
    QByteArray encryptedSecret = EncryptionUtils::instance().encrypt(account.secret);
    query.bindValue(":secret", encryptedSecret);

    query.bindValue(":algorithm", account.algorithm);
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
    QSqlQuery query(db);
    query.prepare("UPDATE accounts SET name = :newName, secret = :secret, algorithm = :algorithm, counter = :counter WHERE name = :oldName");
    query.bindValue(":newName", updatedAccount.name);

    // Шифруем секрет перед сохранением
    QByteArray encryptedSecret = EncryptionUtils::instance().encrypt(updatedAccount.secret);
    query.bindValue(":secret", encryptedSecret);

    query.bindValue(":algorithm", updatedAccount.algorithm);
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
    QSqlQuery query(db);
    query.prepare("SELECT encrypted_value FROM key_verification WHERE id = 1");

    if (query.exec()) {
        if (query.next()) {
            // Проверочное значение существует, пытаемся расшифровать
            QByteArray encryptedValue = query.value(0).toByteArray();
            QString decryptedValue = EncryptionUtils::instance().decrypt(encryptedValue);
            if (decryptedValue == "verification") {
                // Логируем успешный вход
                logEvent("Пользователь вошел в приложение");
                return true; // Мастер-пароль верен
            } else {
                // Логируем неуспешную попытку входа
                logEvent("Неудачная попытка входа");
                return false; // Неверный мастер-пароль
            }
        } else {
            // Проверочное значение не существует, создаем его
            QByteArray verificationValue = "verification";
            QByteArray encryptedValue = EncryptionUtils::instance().encrypt(verificationValue);
            query.prepare("INSERT INTO key_verification (id, encrypted_value) VALUES (1, :encrypted_value)");
            query.bindValue(":encrypted_value", encryptedValue);
            if (!query.exec()) {
                qWarning() << "Не удалось вставить проверочное значение ключа:" << query.lastError().text();
                return false;
            }
            // Логируем установку мастер-пароля
            logEvent("Установлен мастер-пароль");
            return true; // Мастер-пароль установлен
        }
    } else {
        qWarning() << "Не удалось выполнить запрос к таблице key_verification:" << query.lastError().text();
        return false;
    }
}
