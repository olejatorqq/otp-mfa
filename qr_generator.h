// qr_generator.h

#ifndef QR_GENERATOR_H
#define QR_GENERATOR_H

#include <QObject>
#include <QPixmap>
#include <QString>

class QRGenerator : public QObject
{
    Q_OBJECT
public:
    explicit QRGenerator(QObject *parent = nullptr);
    ~QRGenerator();

public slots:
    void processPixmap(const QPixmap &pixmap);

signals:
    void qrDecoded(const QString &decodedText);
    void qrError(const QString &errorString);
};

#endif // QR_GENERATOR_H
