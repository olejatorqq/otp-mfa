#ifndef OTP_WINDOW_H
#define OTP_WINDOW_H

#include <QWidget>
#include <QTimer>
#include "otp_generator.h"
#include "account_manager.h"
#include "add_account_dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class OtpWindow; }
QT_END_NAMESPACE

class OtpWindow : public QWidget {
    Q_OBJECT

public:
    OtpWindow(QWidget *parent = nullptr);
    ~OtpWindow();

private slots:
    void updateOtp();
    void on_addAccountButton_clicked();

private:
    Ui::OtpWindow *ui;
    OtpGenerator otpGenerator;
    AccountManager accountManager;
    QTimer *timer;
    QList<Account> accounts;
};

#endif // OTP_WINDOW_H
