#ifndef QR_GENERATOR_H
#define QR_GENERATOR_H

#include <QString>
#include <QPixmap>

class QRGenerator {
public:
    static QPixmap generate(const QString &data);  // Возвращаем QPixmap для отображения QR-кода
};

#endif // QR_GENERATOR_H
