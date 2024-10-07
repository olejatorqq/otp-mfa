#ifndef OTP_WINDOW_H
#define OTP_WINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>


class otp_window : public QWidget {
    Q_OBJECT
public:
    otp_window(QWidget *parent = nullptr);

public slots:
    void updateOTP();

private:
    QLabel *otpLabel;
};

#endif // OTP_WINDOW_H
