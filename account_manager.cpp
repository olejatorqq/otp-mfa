#include "account_manager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

AccountManager::AccountManager(const QString &filePath)
    : filePath(filePath) {}

QList<Account> AccountManager::loadAccounts() {
    QList<Account> accounts;
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonArray array = doc.array();

        for (const QJsonValue &value : array) {
            accounts.append(jsonToAccount(value.toObject()));
        }
    }
    return accounts;
}

void AccountManager::saveAccount(const Account &account) {
    QList<Account> accounts = loadAccounts();
    accounts.append(account);

    QJsonArray array;
    for (const Account &acc : accounts) {
        array.append(accountToJson(acc));
    }

    QJsonDocument doc(array);
    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(doc.toJson());
        file.close();
    }
}

QJsonObject AccountManager::accountToJson(const Account &account) {
    QJsonObject json;
    json["name"] = account.name;
    json["secret"] = account.secret;
    json["algorithm"] = account.algorithm;
    return json;
}

Account AccountManager::jsonToAccount(const QJsonObject &json) {
    Account account;
    account.name = json["name"].toString();
    account.secret = json["secret"].toString();
    account.algorithm = json["algorithm"].toString();
    return account;
}
