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
    AccountManager();
    ~AccountManager();

    QList<Account> getAccounts() const;
    void addAccount(const Account& account);
    bool verifyMasterPassword();


private:
    QSqlDatabase db;

    void initializeDatabase();  // Инициализация базы данных
};

#endif // ACCOUNT_MANAGER_H
