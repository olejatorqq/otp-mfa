#ifndef ENCRYPTED_STORAGE_H
#define ENCRYPTED_STORAGE_H

#include <QString>
#include <QByteArray>
#include <openssl/aes.h>

class EncryptedStorage {
public:
    static QByteArray encrypt(const QString &data, const QString &password);
    static QString decrypt(const QByteArray &encryptedData, const QString &password);
};

#endif // ENCRYPTED_STORAGE_H
