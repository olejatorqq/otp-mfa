#include "encrypted_storage.h"
#include <QCryptographicHash>

QByteArray EncryptedStorage::encrypt(const QString &data, const QString &password) {
    AES_KEY encryptKey;
    unsigned char key[32];

    QByteArray keyHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    memcpy(key, keyHash.constData(), 32);

    AES_set_encrypt_key(key, 256, &encryptKey);

    QByteArray paddedData = data.toUtf8();
    paddedData.append(QByteArray(16 - paddedData.size() % 16, '\0'));

    QByteArray encryptedData(paddedData.size(), '\0');
    AES_encrypt((unsigned char*)paddedData.data(), (unsigned char*)encryptedData.data(), &encryptKey);

    return encryptedData;
}

QString EncryptedStorage::decrypt(const QByteArray &encryptedData, const QString &password) {
    AES_KEY decryptKey;
    unsigned char key[32];

    QByteArray keyHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    memcpy(key, keyHash.constData(), 32);

    AES_set_decrypt_key(key, 256, &decryptKey);

    QByteArray decryptedData(encryptedData.size(), '\0');
    AES_decrypt((unsigned char*)encryptedData.data(), (unsigned char*)decryptedData.data(), &decryptKey);

    return QString::fromUtf8(decryptedData).trimmed();
}
