#include "account_manager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <QCryptographicHash>

static const int AES_KEY_SIZE = 32;  // 256-битный ключ
static const int AES_BLOCK_SIZE = 16;

AccountManager::AccountManager() {
    loadAccounts();
}

void AccountManager::setPassword(const QString &password) {
    QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    encryptionKey = hashedPassword.mid(0, AES_KEY_SIZE);  // Используем первые 32 байта как ключ
}

QByteArray AccountManager::encryptSecret(const QString &secret) const {
    if (encryptionKey.isEmpty()) return QByteArray();

    QByteArray secretBytes = secret.toUtf8();
    QByteArray encryptedSecret(secretBytes.size() + AES_BLOCK_SIZE, '\0');

    unsigned char iv[AES_BLOCK_SIZE];
    RAND_bytes(iv, AES_BLOCK_SIZE);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char*>(encryptionKey.data()), iv);

    int outLen1 = 0;
    EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char*>(encryptedSecret.data()), &outLen1, reinterpret_cast<const unsigned char*>(secretBytes.data()), secretBytes.size());

    int outLen2 = 0;
    EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(encryptedSecret.data()) + outLen1, &outLen2);
    encryptedSecret.resize(outLen1 + outLen2);

    EVP_CIPHER_CTX_free(ctx);

    return QByteArray(reinterpret_cast<const char*>(iv), AES_BLOCK_SIZE) + encryptedSecret;
}

QString AccountManager::decryptSecret(const QByteArray &encryptedSecret) const {
    if (encryptionKey.isEmpty() || encryptedSecret.size() <= AES_BLOCK_SIZE) return QString();

    // Извлечение IV
    unsigned char iv[AES_BLOCK_SIZE];
    memcpy(iv, encryptedSecret.constData(), AES_BLOCK_SIZE);

    QByteArray secretBytes(encryptedSecret.size() - AES_BLOCK_SIZE, '\0');

    // Настройка расшифровки AES-256
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char*>(encryptionKey.data()), iv);

    int outLen1 = 0;
    EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(secretBytes.data()), &outLen1, reinterpret_cast<const unsigned char*>(encryptedSecret.constData() + AES_BLOCK_SIZE), encryptedSecret.size() - AES_BLOCK_SIZE);

    int outLen2 = 0;
    EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(secretBytes.data()) + outLen1, &outLen2);
    secretBytes.resize(outLen1 + outLen2);

    EVP_CIPHER_CTX_free(ctx);

    return QString::fromUtf8(secretBytes);
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
        QByteArray encryptedSecret = QByteArray::fromBase64(accountObject["secret"].toString().toUtf8());
        account.secret = decryptSecret(encryptedSecret);  // Расшифровка секрета
        account.algorithm = accountObject["algorithm"].toString();
        account.counter = accountObject["counter"].toInt();
        accounts.append(account);
    }

    file.close();
}

void AccountManager::saveAccounts() const {
    QJsonArray accountArray;
    for (const Account& account : accounts) {
        QJsonObject accountObject;
        accountObject["name"] = account.name;
        accountObject["algorithm"] = account.algorithm;
        accountObject["counter"] = account.counter;

        // Преобразование зашифрованного секрета в строку
        accountObject["secret"] = QString::fromUtf8(encryptSecret(account.secret).toBase64());

        accountArray.append(accountObject);
    }

    QJsonDocument document(accountArray);
    QFile file("accounts.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(document.toJson());
        file.close();
    }
}
