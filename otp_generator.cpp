#include "otp_generator.h"
#include <openssl/hmac.h>
#include <QDateTime>
#include <cmath>


QString otp_generator::generateHOTP(const QByteArray &key, quint64 counter, int digits) {
    QByteArray counterBytes = QByteArray::number(counter);
    unsigned char* hash = HMAC(EVP_sha1(), key.data(), key.size(),
                               reinterpret_cast<const unsigned char*>(counterBytes.data()), counterBytes.size(), nullptr, nullptr);

    int offset = hash[19] & 0xf;
    quint32 binaryCode = (hash[offset] & 0x7f) << 24
                         | (hash[offset + 1] & 0xff) << 16
                         | (hash[offset + 2] & 0xff) << 8
                         | (hash[offset + 3] & 0xff);

    quint32 otp = binaryCode % static_cast<quint32>(std::pow(10, digits));
    return QString::number(otp).rightJustified(digits, '0');
}


QString otp_generator::generateTOTP(const QByteArray &key, int timeStep, int digits) {
    quint64 timeCounter = QDateTime::currentSecsSinceEpoch() / timeStep;
    return generateHOTP(key, timeCounter, digits);
}
