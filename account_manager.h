#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include <QString>
#include <QList>
#include <QSqlDatabase>

struct Account {
    QString name;
    QString secret;
    QString algorithm;
    int counter;  // Для HOTP
};

class AccountManager {
public:
    static AccountManager& instance();

    // Запрещаем копирование и присваивание
    AccountManager(AccountManager const&) = delete;
    void operator=(AccountManager const&) = delete;

    QList<Account> getAccounts() const;
    void addAccount(const Account& account);
    void deleteAccount(const QString& accountName);
    void updateAccount(const QString& accountName, const Account& updatedAccount);

    bool verifyMasterPassword();

    // Метод для записи в журнал
    void logEvent(const QString& eventDescription);

private:
    AccountManager();
    ~AccountManager();

    QSqlDatabase db;

    void initializeDatabase();  // Инициализация базы данных
};

#endif // ACCOUNT_MANAGER_H
