#ifndef ENCRYPTION_UTILS_H
#define ENCRYPTION_UTILS_H

#include <QString>
#include <QByteArray>

class EncryptionUtils {
public:
    static EncryptionUtils& instance();

    // Убирал setMasterPassword и isMasterPasswordSet для мастер-пароля
    // void setMasterPassword(const QString& password);
    // bool isMasterPasswordSet() const;

    QByteArray encrypt(const QString& plaintext);
    QString decrypt(const QByteArray& ciphertext);

private:
    EncryptionUtils();

    // Можно хранить просто константный ключ
    QByteArray key;
};

#endif // ENCRYPTION_UTILS_H
