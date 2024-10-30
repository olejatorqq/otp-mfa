#include "otp_window.h"
#include "ui_otp_window.h"
#include "add_account_dialog.h"
#include "otp_generator.h"
#include <QListWidgetItem>
#include <QTimer>
#include <QTime>

const int OTP_VALID_DURATION = 30; // Длительность действия OTP в секундах

OTPWindow::OTPWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OTPWindow),
    selectedAccountIndex(-1)  // Инициализация индекса выбранного аккаунта
{
    ui->setupUi(this);

    accounts = accountManager.getAccounts();
    displayAccounts();

    connect(ui->addButton, &QPushButton::clicked, this, &OTPWindow::onAddAccountClicked);

    // Подключаем обработчик двойного нажатия на строку в таблице
    connect(ui->accountsTableWidget, &QTableWidget::cellDoubleClicked, this, &OTPWindow::onAccountDoubleClicked);

    // Таймер для обновления OTP каждые 5 секунд
    QTimer *otpTimer = new QTimer(this);
    connect(otpTimer, &QTimer::timeout, this, &OTPWindow::updateAccounts);
    otpTimer->start(5000);  // Обновляем OTP каждые 5 секунд

    // Таймер для обновления оставшегося времени каждую секунду
    QTimer *timeLeftTimer = new QTimer(this);
    connect(timeLeftTimer, &QTimer::timeout, this, &OTPWindow::updateTimeLeft);
    timeLeftTimer->start(1000);  // Обновляем оставшееся время каждую секунду
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
        otp = generator.generateTOTP(account.secret);  // Используем только TOTP

        int row = ui->accountsTableWidget->rowCount();
        ui->accountsTableWidget->insertRow(row);
        ui->accountsTableWidget->setItem(row, 0, new QTableWidgetItem(account.name));  // Имя аккаунта
        ui->accountsTableWidget->setItem(row, 1, new QTableWidgetItem(otp));  // Сгенерированный OTP

        // Вычисляем оставшееся время до обновления
        int timeLeft = OTP_VALID_DURATION - (QTime::currentTime().second() % OTP_VALID_DURATION);
        ui->accountsTableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(timeLeft) + "s"));  // Время до обновления
    }
}

void OTPWindow::onAccountDoubleClicked(int row, int /* column */) {
    selectedAccountIndex = row;  // Устанавливаем индекс выбранного аккаунта
}

void OTPWindow::updateAccounts() {
    displayAccounts();
}

void OTPWindow::updateTimeLeft() {
    for (int row = 0; row < ui->accountsTableWidget->rowCount(); ++row) {
        int timeLeft = OTP_VALID_DURATION - (QTime::currentTime().second() % OTP_VALID_DURATION);
        ui->accountsTableWidget->item(row, 2)->setText(QString::number(timeLeft) + "s");  // Обновляем оставшееся время
    }
}
