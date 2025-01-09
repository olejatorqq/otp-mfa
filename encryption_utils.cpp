#include "encryption_utils.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <QDebug>

EncryptionUtils::EncryptionUtils() {
}

EncryptionUtils& EncryptionUtils::instance() {
    static EncryptionUtils instance;
    return instance;
}

void EncryptionUtils::setMasterPassword(const QString& password) {
    key = deriveKey(password);
}

bool EncryptionUtils::isMasterPasswordSet() const {
    return !key.isEmpty();
}

QByteArray EncryptionUtils::deriveKey(const QString& password) {
    // Используем PBKDF2 для генерации ключа из мастер-пароля
    QByteArray salt = "saltsalt";
    const int iterations = 100000;
    const int keyLength = 32; // Для AES-256

    QByteArray key(keyLength, 0);
    PKCS5_PBKDF2_HMAC_SHA1(
        password.toUtf8().constData(),
        password.toUtf8().size(),
        reinterpret_cast<const unsigned char*>(salt.constData()),
        salt.size(),
        iterations,
        keyLength,
        reinterpret_cast<unsigned char*>(key.data())
        );
    return key;
}

QByteArray EncryptionUtils::encrypt(const QString& plaintext) {
    if (key.isEmpty()) {
        qWarning() << "Encryption key is not set.";
        return QByteArray();
    }

    // Инициализация
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    const EVP_CIPHER* cipher = EVP_aes_256_cbc();

    // Генерация случайного IV
    QByteArray iv(EVP_CIPHER_iv_length(cipher), 0);
    RAND_bytes(reinterpret_cast<unsigned char*>(iv.data()), iv.size());

    // Настройка шифрования
    EVP_EncryptInit_ex(ctx, cipher, nullptr, reinterpret_cast<const unsigned char*>(key.constData()), reinterpret_cast<const unsigned char*>(iv.constData()));

    // Шифрование данных
    QByteArray ciphertext;
    ciphertext.resize(plaintext.toUtf8().size() + EVP_CIPHER_block_size(cipher));
    int outLen1 = 0;
    EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char*>(ciphertext.data()), &outLen1, reinterpret_cast<const unsigned char*>(plaintext.toUtf8().constData()), plaintext.toUtf8().size());

    int outLen2 = 0;
    EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(ciphertext.data()) + outLen1, &outLen2);

    ciphertext.resize(outLen1 + outLen2);

    // Освобождение ресурсов
    EVP_CIPHER_CTX_free(ctx);

    // Добавляем IV в начало шифротекста
    QByteArray result = iv + ciphertext;
    return result.toBase64();
}

QString EncryptionUtils::decrypt(const QByteArray& ciphertextBase64) {
    if (key.isEmpty()) {
        qWarning() << "Encryption key is not set.";
        return QString();
    }

    QByteArray ciphertextWithIV = QByteArray::fromBase64(ciphertextBase64);

    // Инициализация
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    const EVP_CIPHER* cipher = EVP_aes_256_cbc();
    int ivLength = EVP_CIPHER_iv_length(cipher);

    // Извлечение IV
    QByteArray iv = ciphertextWithIV.left(ivLength);
    QByteArray ciphertext = ciphertextWithIV.mid(ivLength);

    // Настройка расшифровки
    if (!EVP_DecryptInit_ex(ctx, cipher, nullptr, reinterpret_cast<const unsigned char*>(key.constData()), reinterpret_cast<const unsigned char*>(iv.constData()))) {
        qWarning() << "Failed to initialize decryption.";
        EVP_CIPHER_CTX_free(ctx);
        return QString();
    }

    // Расшифровка данных
    QByteArray plaintext;
    plaintext.resize(ciphertext.size() + EVP_CIPHER_block_size(cipher));
    int outLen1 = 0;
    if (!EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(plaintext.data()), &outLen1, reinterpret_cast<const unsigned char*>(ciphertext.constData()), ciphertext.size())) {
        qWarning() << "Decryption failed during update.";
        EVP_CIPHER_CTX_free(ctx);
        return QString();
    }

    int outLen2 = 0;
    if (!EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(plaintext.data()) + outLen1, &outLen2)) {
        qWarning() << "Decryption failed during finalization.";
        EVP_CIPHER_CTX_free(ctx);
        return QString();
    }

    plaintext.resize(outLen1 + outLen2);

    // Освобождение ресурсов
    EVP_CIPHER_CTX_free(ctx);

    return QString::fromUtf8(plaintext);
}

