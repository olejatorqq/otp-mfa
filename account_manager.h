#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include <QString>
#include <QList>

struct Account {
    QString name;
    QString secret;
    QString algorithm;
    int counter;  // Для HOTP
};

class AccountManager {
public:
    AccountManager();
    QList<Account> getAccounts() const;
    void addAccount(const Account& account);

private:
    QList<Account> accounts;
    void saveAccounts() const;  // Сохранение учётных записей в JSON файл
    void loadAccounts();        // Загрузка учётных записей из JSON файла
    void saveAccounts();
};

#endif // ACCOUNT_MANAGER_H
