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
    // Реализация логирования событий (если требуется)
    // Например, запись в файл или базу данных
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
            continue; // Пропускаем этот аккаунт
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
    // Реализация проверки мастер-пароля (если требуется)
    return true;
}
