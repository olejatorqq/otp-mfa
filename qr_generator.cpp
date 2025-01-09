#include "qr_generator.h"

// Подключаем заголовки ZXing так, как это нужно вашей сборке:
#include <ReadBarcode.h>
#include <TextUtfEncoding.h>
#include <BarcodeFormat.h>
#include <ReaderOptions.h>

#include <QImage>
#include <QDebug>

QRGenerator::QRGenerator(QObject *parent)
    : QObject(parent)
{
}

QRGenerator::~QRGenerator()
{
}

void QRGenerator::processPixmap(const QPixmap &pixmap)
{
    if (pixmap.isNull()) {
        emit qrError("Пустой QPixmap.");
        return;
    }

    // Переводим QPixmap → QImage (ARGB32)
    QImage img = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);

    // Готовим буфер для RGB (3 байта на пиксель)
    std::vector<uint8_t> buffer;
    buffer.reserve(img.width() * img.height() * 3);

    for (int y = 0; y < img.height(); ++y) {
        const uchar *scanLine = img.scanLine(y);
        for (int x = 0; x < img.width(); ++x) {
            // ARGB32 (BGRA) => берем R,G,B
            uint8_t b = scanLine[x * 4 + 0];
            uint8_t g = scanLine[x * 4 + 1];
            uint8_t r = scanLine[x * 4 + 2];
            // alpha = scanLine[x * 4 + 3]; // не используем

            buffer.push_back(r);
            buffer.push_back(g);
            buffer.push_back(b);
        }
    }

    // Указываем формат: RGB
    ZXing::ImageView zxingImage(
        buffer.data(),
        img.width(),
        img.height(),
        ZXing::ImageFormat::RGB
        );

    // Вместо DecodeHints используем ReaderOptions
    ZXing::ReaderOptions hints;
    hints.setTryHarder(true);  // <== СЕТТЕР
    hints.setFormats({ZXing::BarcodeFormat::QRCode}); // <== СЕТТЕР

    // Декодируем
    ZXing::Result result = ZXing::ReadBarcode(zxingImage, hints);

    if (result.isValid()) {
        QString text = QString::fromStdString(result.text());
        qDebug() << "QR-код распознан:" << text;
        emit qrDecoded(text);
    } else {
        qWarning() << "Не удалось распознать QR-код.";
        emit qrError("Не удалось распознать QR-код.");
    }
}
