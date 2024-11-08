#include "otp_generator.h"
#include <QDateTime>
#include <QByteArray>
#include <QCryptographicHash>
#include <QDebug>
#include <QMessageAuthenticationCode>
#include <QMap>
#include <cctype> // Добавляем для функции isspace

QString OtpGenerator::generateTOTP(const QString &secret, int timeStep) {
    quint64 time = QDateTime::currentSecsSinceEpoch() / timeStep;
    return generateHOTP(secret, time);
}

QString OtpGenerator::generateHOTP(const QString &secret, quint64 counter) {
    QByteArray key = base32Decode(secret);
    if (key.isEmpty()) {
        qWarning() << "Invalid Base32 secret.";
        return QString();
    }

    QByteArray message(8, 0);
    for (int i = 7; i >= 0; --i) {
        message[7 - i] = (counter >> (i * 8)) & 0xFF;
    }

    QByteArray hash = hmacSha1(key, message);
    if (hash.isEmpty()) {
        qWarning() << "HMAC calculation failed.";
        return QString();
    }

    int offset = hash[hash.size() - 1] & 0x0F;
    int binary = ((hash[offset] & 0x7F) << 24) |
                 ((hash[offset + 1] & 0xFF) << 16) |
                 ((hash[offset + 2] & 0xFF) << 8) |
                 (hash[offset + 3] & 0xFF);

    int otp = binary % 1000000;
    return QString("%1").arg(otp, 6, 10, QLatin1Char('0'));
}

QByteArray OtpGenerator::hmacSha1(const QByteArray &key, const QByteArray &message) {
    // Используем QMessageAuthenticationCode для вычисления HMAC-SHA1
    QMessageAuthenticationCode code(QCryptographicHash::Sha1, key);
    code.addData(message);
    return code.result();
}

QByteArray OtpGenerator::base32Decode(const QString &input) {
    // Обновленная функция base32Decode с правильной обработкой символов
    static const char base32Alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    QMap<char, quint8> base32Lookup;
    for (int i = 0; i < 32; ++i) {
        base32Lookup[base32Alphabet[i]] = i;
    }
    // Добавляем соответствия для цифр и часто путаемых символов
    base32Lookup['0'] = base32Lookup['O']; // 0 как O
    base32Lookup['1'] = base32Lookup['L']; // 1 как L
    base32Lookup['8'] = base32Lookup['B']; // 8 как B

    QByteArray buffer = input.trimmed().toUpper().remove('=').toLatin1();
    QByteArray bytes;

    int bits = 0;
    int value = 0;
    for (int i = 0; i < buffer.size(); ++i) {
        char c = buffer[i];
        if (isspace(static_cast<unsigned char>(c))) {
            continue; // Пропускаем пробелы
        }

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
