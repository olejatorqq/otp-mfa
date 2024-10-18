#include "qr_generator.h"
#include <QImage>
#include <QPixmap>
#include "/home/oorlovsk/otp-mfa/include/qrcodegen.hpp"

QPixmap QRGenerator::generate(const QString &data) {
    using qrcodegen::QrCode;

    // Генерация QR-кода с низким уровнем исправления ошибок
    QrCode qr = QrCode::encodeText(data.toUtf8().constData(), QrCode::Ecc::LOW);

    const int size = qr.getSize();
    QImage image(size, size, QImage::Format_RGB32);

    // Прорисовка QR-кода в изображении
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            image.setPixel(x, y, qr.getModule(x, y) ? qRgb(0, 0, 0) : qRgb(255, 255, 255));
        }
    }

    return QPixmap::fromImage(image);  // Возвращаем QPixmap для отображения в UI
}
