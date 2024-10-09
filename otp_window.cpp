#include "otp_window.h"
#include "ui_otp_window.h"

OtpWindow::OtpWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::OtpWindow), accountManager("accounts.json") {
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &OtpWindow::updateOtp);
    timer->start(1000); // Обновление каждую секунду

    accounts = accountManager.loadAccounts();
    updateOtp(); // Первоначальный вызов для отображения OTP при запуске
}

OtpWindow::~OtpWindow() {
    delete ui;
}

void OtpWindow::updateOtp() {
    if (!accounts.isEmpty()) {
        const Account &account = accounts.first(); // Используем первую учётную запись для примера
        QString otp = (account.algorithm == "TOTP") ?
                          otpGenerator.generateTOTP(account.secret) :
                          otpGenerator.generateHOTP(account.secret, 1); // Placeholder: использовать счётчик для HOTP
        ui->otpLabel->setText(otp);
    }
}

void OtpWindow::on_addAccountButton_clicked() {
    AddAccountDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Account newAccount = dialog.getAccount();
        accountManager.saveAccount(newAccount);
        accounts = accountManager.loadAccounts();
        updateOtp();
    }
}
