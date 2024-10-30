#include "otp_window.h"
#include "ui_otp_window.h"
#include "add_account_dialog.h"
#include "otp_generator.h"
#include <QListWidgetItem>
#include <QTimer>
#include <QLineEdit>
#include <QDateTime>

OTPWindow::OTPWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OTPWindow),
    selectedAccountIndex(-1),  // Инициализация индекса выбранного аккаунта
    timer(new QTimer(this))  // Инициализация таймера
{
    ui->setupUi(this);

    accounts = accountManager.getAccounts();
    displayAccounts();

    connect(ui->addButton, &QPushButton::clicked, this, &OTPWindow::onAddAccountClicked);
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &OTPWindow::filterAccounts);  // Подключение фильтрации по тексту

    // Подключаем обработчик двойного нажатия на строку в таблице
    connect(ui->accountsTableWidget, &QTableWidget::cellDoubleClicked, this, &OTPWindow::onAccountDoubleClicked);

    // Таймер для обновления OTP каждую секунду
    connect(timer, &QTimer::timeout, this, &OTPWindow::updateAccounts);
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

        if (!timer->isActive()) {
            timer->start(1000);  // Запускаем таймер, если он не активен
        }
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

        // Установка оставшегося времени (например, 30 секунд для TOTP)
        int timeLeft = 30 - (QDateTime::currentSecsSinceEpoch() % 30);  // Оставшееся время до следующей генерации
        ui->accountsTableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(timeLeft)));  // Время оставшееся
    }

    // Применяем фильтрацию после обновления
    filterAccounts(ui->searchLineEdit->text());
}

void OTPWindow::filterAccounts(const QString &filter) {
    for (int row = 0; row < ui->accountsTableWidget->rowCount(); ++row) {
        bool matches = ui->accountsTableWidget->item(row, 0)->text().contains(filter, Qt::CaseInsensitive);
        ui->accountsTableWidget->setRowHidden(row, !matches);
    }
}

void OTPWindow::onAccountDoubleClicked(int row, int /* column */) {
    selectedAccountIndex = row;  // Устанавливаем индекс выбранного аккаунта
}

void OTPWindow::updateAccounts() {
    displayAccounts();
}
