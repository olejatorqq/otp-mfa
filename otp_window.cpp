#include "otp_window.h"
#include "otp_generator.h"
#include <QVBoxLayout>
#include <QTimer>

otp_window::otp_window(QWidget *parent) : QWidget(parent) {
    otpLabel = new QLabel("OTP will appear here", this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(otpLabel);
    setLayout(layout);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &otp_window::updateOTP);
    timer->start(30000);  // 30 секунд

    updateOTP();  // Первоначальное обновление OTP
}

void otp_window::updateOTP() {
    QByteArray secretKey = "your-secret-key";  // Замените на реальный секретный ключ
    QString totp = otp_generator::generateTOTP(secretKey);
    otpLabel->setText("TOTP: " + totp);
}
