#ifndef OTP_GENERATOR_H
#define OTP_GENERATOR_H

#include <QString>
#include <openssl/hmac.h>

class OtpGenerator {
public:
    QString generateTOTP(const QString &secret, int timeStep = 30);
    QString generateHOTP(const QString &secret, quint64 counter);

private:
    QString base32Decode(const QString &input);
    QByteArray hmacSha1(const QByteArray &key, const QByteArray &message);
};

#endif // OTP_GENERATOR_H
