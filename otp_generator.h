#ifndef OTP_GENERATOR_H
#define OTP_GENERATOR_H

#include <QString>
#include <QByteArray>

class otp_generator
{
public:
    static QString generateHOTP(const QByteArray &key, quint64 counter, int digits = 6);
    static QString generateTOTP(const QByteArray &key, int timeStep = 30, int digits = 6);
    otp_generator();
};

#endif // OTP_GENERATOR_H
