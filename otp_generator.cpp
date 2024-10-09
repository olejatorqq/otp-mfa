#include "otp_generator.h"
#include <QDateTime>
#include <QByteArray>
#include <QCryptographicHash>

QString OtpGenerator::generateTOTP(const QString &secret, int timeStep) {
    quint64 time = QDateTime::currentSecsSinceEpoch() / timeStep;
    return generateHOTP(secret, time);
}

QString OtpGenerator::generateHOTP(const QString &secret, quint64 counter) {
    QByteArray key = base32Decode(secret).toUtf8();
    QByteArray message;
    for (int i = 7; i >= 0; --i) {
        message.append((counter >> (i * 8)) & 0xFF);
    }

    QByteArray hash = hmacSha1(key, message);
    int offset = hash[hash.size() - 1] & 0xF;
    int binary = ((hash[offset] & 0x7F) << 24) |
                 ((hash[offset + 1] & 0xFF) << 16) |
                 ((hash[offset + 2] & 0xFF) << 8) |
                 (hash[offset + 3] & 0xFF);

    int otp = binary % 1000000;
    return QString("%1").arg(otp, 6, 10, QLatin1Char('0'));
}

QByteArray OtpGenerator::hmacSha1(const QByteArray &key, const QByteArray &message) {
    unsigned char *result;
    unsigned int len = 20;
    result = (unsigned char*)malloc(sizeof(char) * len);

    HMAC_CTX *ctx = HMAC_CTX_new();
    HMAC_Init_ex(ctx, key.data(), key.size(), EVP_sha1(), nullptr);
    HMAC_Update(ctx, (unsigned char*)message.data(), message.size());
    HMAC_Final(ctx, result, &len);
    HMAC_CTX_free(ctx);

    QByteArray hash = QByteArray((char*)result, len);
    free(result);
    return hash;
}

QString OtpGenerator::base32Decode(const QString &input) {
    // Простая реализация декодирования base32. Для большей гибкости, лучше использовать специализированные библиотеки.
    return input; // Placeholder: Реализуй эту функцию
}
