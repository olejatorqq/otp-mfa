#include "otp_window.h"
#include "ui_otp_window.h"
#include "add_account_dialog.h"
#include "otp_generator.h"
#include "qr_generator.h"
#include <QListWidgetItem>
#include <QTimer>
#include <QLabel>
#include <QPixmap>
#include <QTableWidgetItem>

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

    // Таймер для обновления OTP и QR-кода каждые 5 секунд
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &OTPWindow::updateAccountsAndQRCode);
    timer->start(5000);  // Обновляем OTP и QR-код каждые 5 секунд
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
        ui->accountsTableWidget->setItem(row, 0, new QTableWidgetItem(account.name));  // Имя аккаунта
        ui->accountsTableWidget->setItem(row, 1, new QTableWidgetItem(account.algorithm));  // Алгоритм
        ui->accountsTableWidget->setItem(row, 2, new QTableWidgetItem(otp));  // Сгенерированный OTP
    }
}


void OTPWindow::onAccountDoubleClicked(int row, int column) {
    selectedAccountIndex = row;  // Устанавливаем индекс выбранного аккаунта
    generateQRCodeForSelectedAccount();  // Генерируем QR-код для выбранного аккаунта
}


void OTPWindow::updateAccountsAndQRCode() {
    displayAccounts();

    // Если выбран аккаунт, обновляем QR-код для него
    if (selectedAccountIndex >= 0 && selectedAccountIndex < accounts.size()) {
        generateQRCodeForSelectedAccount();
    }
}

void OTPWindow::generateQRCodeForSelectedAccount() {
    // Получаем данные аккаунта
    Account selectedAccount = accounts[selectedAccountIndex];

    // Генерируем OTP для выбранного аккаунта
    OtpGenerator generator;
    QString otp;
    if (selectedAccount.algorithm == "TOTP") {
        otp = generator.generateTOTP(selectedAccount.secret);
    } else {
        otp = generator.generateHOTP(selectedAccount.secret, selectedAccount.counter);
    }

    // Генерируем QR-код с использованием OTP
    QString qrData = QString("Account: %1\nOTP: %2").arg(selectedAccount.name).arg(otp);
    QPixmap qrPixmap = QRGenerator::generate(qrData);

    // Увеличиваем размер QR-кода и выводим его в layout
    QLabel *qrLabel = new QLabel(this);
    qrLabel->setPixmap(qrPixmap.scaled(200, 200));

    // Очищаем предыдущее содержимое и добавляем новый QR-код
    QLayoutItem *item;
    while ((item = ui->qrCodeDisplayLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    ui->qrCodeDisplayLayout->addWidget(qrLabel);  // Добавляем QLabel в layout для отображения QR-кода
}

void OTPWindow::onGenerateQRButtonClicked() {
    if (selectedAccountIndex < 0 || selectedAccountIndex >= accounts.size()) {
        return;  // Если нет выбранного аккаунта, ничего не делаем
    }

    const Account& account = accounts[selectedAccountIndex];
    QString data = QString("otpauth://totp/%1?secret=%2").arg(account.name).arg(account.secret);  // Пример данных для QR-кода

    // Генерация QR-кода и его отображение
    QLabel *qrLabel = new QLabel(this);
    QPixmap qrPixmap = QRGenerator::generate(data);  // Генерация QR-кода как QPixmap
    qrLabel->setPixmap(qrPixmap.scaled(200, 200));  // Увеличиваем размер QR-кода

    // Очищаем предыдущие QR-коды и добавляем новый
    qDeleteAll(ui->qrCodeDisplayLayout->children());
    ui->qrCodeDisplayLayout->addWidget(qrLabel);
}

