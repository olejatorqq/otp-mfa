#include "otp_generator.h"
#include <QDateTime>
#include <QByteArray>
#include <QCryptographicHash>
#include <QDebug>
#include <QMessageAuthenticationCode>
#include <QMap>
#include <cmath>

QString OtpGenerator::generateTOTP(const QString &secret, quint64 currentTime, int timeStep, const QString &algorithm, int digits) {
    quint64 time = currentTime / timeStep;

    // Добавляем отладочный вывод текущего времени в формате ISO 8601
    qDebug() << "Current time:" << currentTime << "(" << QDateTime::fromSecsSinceEpoch(currentTime).toString(Qt::ISODate) << ")"
             << "Time counter:" << time;

    return generateHOTP(secret, time, algorithm, digits);
}

QString OtpGenerator::generateHOTP(const QString &secret, quint64 counter, const QString &algorithm, int digits) {
    QByteArray key = base32Decode(secret);
    if (key.isEmpty()) {
        qWarning() << "Invalid Base32 secret.";
        return QString();
    }

    qDebug() << "Decoded secret key (hex):" << key.toHex();

    QByteArray message(8, 0);
    for (int i = 7; i >= 0; --i) {
        message[7 - i] = static_cast<char>((counter >> (i * 8)) & 0xFF);
    }

    qDebug() << "Counter:" << counter;
    qDebug() << "Message (hex):" << message.toHex();

    QByteArray hash;
    if (algorithm == "SHA1") {
        hash = QMessageAuthenticationCode::hash(message, key, QCryptographicHash::Sha1);
    } else if (algorithm == "SHA256") {
        hash = QMessageAuthenticationCode::hash(message, key, QCryptographicHash::Sha256);
    } else if (algorithm == "SHA512") {
        hash = QMessageAuthenticationCode::hash(message, key, QCryptographicHash::Sha512);
    } else {
        qWarning() << "Unsupported algorithm:" << algorithm;
        return QString();
    }

    qDebug() << "HMAC hash (hex):" << hash.toHex();

    if (hash.size() < 20) { // SHA1 hash size
        qWarning() << "HMAC hash is too short.";
        return QString();
    }

    int offset = hash[hash.size() - 1] & 0x0F;
    qDebug() << "Offset:" << offset;

    // Убедимся, что offset + 3 не выходит за границы массива
    if (offset + 3 >= hash.size()) {
        qWarning() << "Offset + 3 exceeds hash size.";
        return QString();
    }

    quint32 binary = ((static_cast<quint8>(hash[offset]) & 0x7F) << 24) |
                     ((static_cast<quint8>(hash[offset + 1]) & 0xFF) << 16) |
                     ((static_cast<quint8>(hash[offset + 2]) & 0xFF) << 8) |
                     (static_cast<quint8>(hash[offset + 3]) & 0xFF);

    qDebug() << "Binary code:" << binary;

    quint32 otp = binary % static_cast<quint32>(std::pow(10, digits));
    qDebug() << "Generated OTP:" << otp;

    return QString("%1").arg(otp, digits, 10, QLatin1Char('0'));
}


QByteArray OtpGenerator::base32Decode(const QString &input) {
    static const QString base32Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    QMap<QChar, quint8> base32Lookup;
    for (int i = 0; i < base32Alphabet.size(); ++i) {
        base32Lookup[base32Alphabet[i]] = i;
    }

    QString buffer = input.trimmed().toUpper().remove('=').replace(" ", "");
    QByteArray bytes;

    int bits = 0;
    int value = 0;
    for (int i = 0; i < buffer.size(); ++i) {
        QChar c = buffer[i];

        if (!base32Lookup.contains(c)) {
            qWarning() << "Invalid Base32 character:" << c;
            return QByteArray();
        }

        value = (value << 5) | base32Lookup[c];
        bits += 5;

        if (bits >= 8) {
            bits -= 8;
            bytes.append(static_cast<char>((value >> bits) & 0xFF));
            value &= (1 << bits) - 1;
        }
    }

    return bytes;
}
