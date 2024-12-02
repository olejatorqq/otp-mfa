#ifndef OTP_GENERATOR_H
#define OTP_GENERATOR_H

#include <QString>
#include <QByteArray>

class OtpGenerator {
public:
    QString generateTOTP(const QString &secret,
                         quint64 currentTime,
                         int timeStep = 30,
                         const QString &algorithm = "SHA1",
                         int digits = 6);

    QString generateHOTP(const QString &secret,
                         quint64 counter,
                         const QString &algorithm = "SHA1",
                         int digits = 6);

    static QByteArray base32Decode(const QString &input,
                                   QString &errorMessage);
};

#endif // OTP_GENERATOR_H
