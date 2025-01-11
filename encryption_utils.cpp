#include "encryption_utils.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <QDebug>

EncryptionUtils::EncryptionUtils() {
    // Инициализация фиксированным ключом:
    key = QByteArray::fromHex("00112233445566778899AABBCCDDEEFF00112233445566778899AABBCCDDEEFF");
}

EncryptionUtils& EncryptionUtils::instance() {
    static EncryptionUtils instance;
    return instance;
}

QByteArray EncryptionUtils::encrypt(const QString& plaintext) {
    if (key.size() != 32) {
        qWarning() << "Encryption key is invalid size. Must be 256 bits.";
        return {};
    }

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    const EVP_CIPHER* cipher = EVP_aes_256_cbc();

    QByteArray iv(EVP_CIPHER_iv_length(cipher), 0);
    RAND_bytes(reinterpret_cast<unsigned char*>(iv.data()), iv.size());

    EVP_EncryptInit_ex(ctx, cipher, nullptr,
                       reinterpret_cast<const unsigned char*>(key.constData()),
                       reinterpret_cast<const unsigned char*>(iv.constData()));

    QByteArray ciphertext;
    ciphertext.resize(plaintext.toUtf8().size() + EVP_CIPHER_block_size(cipher));
    int outLen1 = 0;
    EVP_EncryptUpdate(ctx,
                      reinterpret_cast<unsigned char*>(ciphertext.data()),
                      &outLen1,
                      reinterpret_cast<const unsigned char*>(plaintext.toUtf8().constData()),
                      plaintext.toUtf8().size());

    int outLen2 = 0;
    EVP_EncryptFinal_ex(ctx,
                        reinterpret_cast<unsigned char*>(ciphertext.data()) + outLen1,
                        &outLen2);

    ciphertext.resize(outLen1 + outLen2);
    EVP_CIPHER_CTX_free(ctx);

    QByteArray result = iv + ciphertext;
    return result.toBase64();
}

QString EncryptionUtils::decrypt(const QByteArray& ciphertextBase64) {
    if (key.size() != 32) {
        qWarning() << "Encryption key is invalid size.";
        return QString();
    }

    QByteArray ciphertextWithIV = QByteArray::fromBase64(ciphertextBase64);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    const EVP_CIPHER* cipher = EVP_aes_256_cbc();
    int ivLength = EVP_CIPHER_iv_length(cipher);

    // Разделяем IV и зашифрованный текст
    QByteArray iv = ciphertextWithIV.left(ivLength);
    QByteArray ciphertext = ciphertextWithIV.mid(ivLength);

    if (!EVP_DecryptInit_ex(ctx, cipher, nullptr,
                            reinterpret_cast<const unsigned char*>(key.constData()),
                            reinterpret_cast<const unsigned char*>(iv.constData()))) {
        qWarning() << "Failed to initialize decryption.";
        EVP_CIPHER_CTX_free(ctx);
        return QString();
    }

    QByteArray plaintext;
    plaintext.resize(ciphertext.size() + EVP_CIPHER_block_size(cipher));
    int outLen1 = 0;
    if (!EVP_DecryptUpdate(ctx,
                           reinterpret_cast<unsigned char*>(plaintext.data()),
                           &outLen1,
                           reinterpret_cast<const unsigned char*>(ciphertext.constData()),
                           ciphertext.size())) {
        qWarning() << "Decryption failed during update.";
        EVP_CIPHER_CTX_free(ctx);
        return QString();
    }

    int outLen2 = 0;
    if (!EVP_DecryptFinal_ex(ctx,
                             reinterpret_cast<unsigned char*>(plaintext.data()) + outLen1,
                             &outLen2)) {
        qWarning() << "Decryption failed during finalization.";
        EVP_CIPHER_CTX_free(ctx);
        return QString();
    }

    plaintext.resize(outLen1 + outLen2);
    EVP_CIPHER_CTX_free(ctx);

    return QString::fromUtf8(plaintext);
}
