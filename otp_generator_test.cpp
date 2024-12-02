#include <QCoreApplication>
#include "otp_generator.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    OtpGenerator generator;

    // Используем секретный ключ из RFC 4226 и RFC 6238
    QString secret = "GEZDGNBVGY3TQOJQGEZDGNBVGY3TQOJQ";

    qDebug() << "HOTP Test:";
    for (quint64 counter = 0; counter < 10; ++counter) {
        QString otpCode = generator.generateHOTP(secret, counter, "SHA1", 6); // Изменили на 6 цифр
        qDebug() << "HOTP Counter:" << counter << "OTP Code:" << otpCode;
    }

    qDebug() << "\nTOTP Test:";
    QList<quint64> testTimes = {59, 1111111109, 1111111111, 1234567890, 2000000000};
    for (quint64 testTime : testTimes) {
        quint64 counter = testTime / 30;
        QString otpCode = generator.generateHOTP(secret, counter, "SHA1", 8); // Изменили на 8 цифр
        qDebug() << "TOTP Time:" << testTime << "OTP Code:" << otpCode;
    }

    return 0;
}
