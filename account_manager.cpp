#include "account_manager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

AccountManager::AccountManager() {
    loadAccounts();
}

void AccountManager::addAccount(const Account& account) {
    accounts.append(account);
    saveAccounts();  // Сохраняем аккаунты после добавления
}

QList<Account> AccountManager::getAccounts() const {
    return accounts;
}

void AccountManager::loadAccounts() {
    QFile file("accounts.json");
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonArray accountArray = document.array();

    for (int i = 0; i < accountArray.size(); ++i) {
        QJsonObject accountObject = accountArray[i].toObject();
        Account account;
        account.name = accountObject["name"].toString();
        account.secret = accountObject["secret"].toString();
        account.algorithm = accountObject["algorithm"].toString();
        account.counter = accountObject["counter"].toInt();
        accounts.append(account);
    }

    file.close();
}

void AccountManager::saveAccounts() {
    QJsonArray accountArray;
    for (const Account& account : accounts) {
        QJsonObject accountObject;
        accountObject["name"] = account.name;
        accountObject["secret"] = account.secret;
        accountObject["algorithm"] = account.algorithm;
        accountObject["counter"] = account.counter;

        accountArray.append(accountObject);
    }

    QJsonDocument document(accountArray);
    QFile file("accounts.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(document.toJson());
        file.close();
    }
}
