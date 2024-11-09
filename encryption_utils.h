#ifndef ENCRYPTION_UTILS_H
#define ENCRYPTION_UTILS_H

#include <QString>
#include <QByteArray>

class EncryptionUtils {
public:
    static EncryptionUtils& instance();

    void setMasterPassword(const QString& password);
    bool isMasterPasswordSet() const;

    QByteArray encrypt(const QString& plaintext);
    QString decrypt(const QByteArray& ciphertext);

private:
    EncryptionUtils();
    QByteArray key;

    QByteArray deriveKey(const QString& password);
};

#endif // ENCRYPTION_UTILS_H
