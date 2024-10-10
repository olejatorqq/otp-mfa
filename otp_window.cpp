#include "otp_window.h"
#include "ui_otp_window.h"
#include "add_account_dialog.h"
#include "otp_generator.h"
#include <QListWidgetItem>
#include <QTimer>

OTPWindow::OTPWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OTPWindow)
{
    ui->setupUi(this);

    accounts = accountManager.getAccounts();
    displayAccounts();

    connect(ui->addButton, &QPushButton::clicked, this, &OTPWindow::onAddAccountClicked);

    // Таймер для обновления OTP каждые 5 секунд
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &OTPWindow::displayAccounts);
    timer->start(5000);  // Обновляем OTP каждые 5 секунд
}

OTPWindow::~OTPWindow()
{
    delete ui;
}

void OTPWindow::onAddAccountClicked() {
    AddAccountDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Account newAccount = dialog.getAccount();
        accountManager.addAccount(newAccount);
        accounts = accountManager.getAccounts();
        displayAccounts();
    }
}

void OTPWindow::displayAccounts() {
    ui->accountsTableWidget->setRowCount(0);  // Очищаем таблицу

    for (const Account& account : accounts) {
        QString otp;
        OtpGenerator generator;
        if (account.algorithm == "TOTP") {
            otp = generator.generateTOTP(account.secret);
        } else {
            otp = generator.generateHOTP(account.secret, account.counter);
        }

        int row = ui->accountsTableWidget->rowCount();
        ui->accountsTableWidget->insertRow(row);
        ui->accountsTableWidget->setItem(row, 0, new QTableWidgetItem(account.name));
        ui->accountsTableWidget->setItem(row, 1, new QTableWidgetItem(account.algorithm));
        ui->accountsTableWidget->setItem(row, 2, new QTableWidgetItem(otp));
    }
}
