#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include <QString>
#include <QList>
#include <QJsonObject>

struct Account {
    QString name;
    QString secret;
    QString algorithm; // "TOTP" или "HOTP"
};

class AccountManager {
public:
    AccountManager(const QString &filePath);
    QList<Account> loadAccounts();
    void saveAccount(const Account &account);

private:
    QString filePath;
    QJsonObject accountToJson(const Account &account);
    Account jsonToAccount(const QJsonObject &json);
};

#endif // ACCOUNT_MANAGER_H
