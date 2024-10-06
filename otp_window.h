#ifndef OTP_WINDOW_H
#define OTP_WINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class otp_window; }
QT_END_NAMESPACE

class otp_window : public QMainWindow
{
    Q_OBJECT

public:
    otp_window(QWidget *parent = nullptr);
    ~otp_window();

private:
    Ui::otp_window *ui;
};
#endif // OTP_WINDOW_H
