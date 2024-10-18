#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include <QString>
#include <QList>
#include <QByteArray>

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
    void setPassword(const QString &password);  // Установка пароля для шифрования

    QString decryptSecret(const QByteArray &encryptedSecret) const;  // Расшифровка секрета

private:
    QList<Account> accounts;
    QByteArray encryptionKey;  // Ключ для AES-шифрования

    void saveAccounts() const;  // Сохранение учётных записей в JSON файл
    void loadAccounts();        // Загрузка учётных записей из JSON файла

    QByteArray encryptSecret(const QString &secret) const;      // Шифрование секрета
};

#endif // ACCOUNT_MANAGER_H
