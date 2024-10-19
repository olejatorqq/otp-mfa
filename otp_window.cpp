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
#include <QInputDialog>
#include <QMessageBox>
#include <QTime>

OTPWindow::OTPWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OTPWindow),
    selectedAccountIndex(-1)  // Инициализация индекса выбранного аккаунта
{
    ui->setupUi(this);

    // Запрос пароля при запуске приложения
    bool ok;
    QString password = QInputDialog::getText(this, tr("Введите пароль"),
                                             tr("Пароль:"), QLineEdit::Password,
                                             "", &ok);
    if (ok && !password.isEmpty()) {
        accountManager.setPassword(password);
        accounts = accountManager.getAccounts();
        displayAccounts();
    } else {
        QMessageBox::critical(this, tr("Ошибка"), tr("Пароль обязателен для входа."));
        close();  // Закрываем приложение, если пароль не введён
    }

    connect(ui->addButton, &QPushButton::clicked, this, &OTPWindow::onAddAccountClicked);
    connect(ui->accountsTableWidget, &QTableWidget::cellDoubleClicked, this, &OTPWindow::onAccountDoubleClicked);

    // Таймер для обновления OTP и QR-кода каждые 5 секунд
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &OTPWindow::updateAccountsAndQRCode);
    timer->start(5000);  // Обновляем OTP и QR-код каждые 5 секунд

    // Таймер для плавного обновления прогресс-бара каждую секунду
    QTimer *progressBarTimer = new QTimer(this);
    connect(progressBarTimer, &QTimer::timeout, this, &OTPWindow::updateProgressBar);
    progressBarTimer->start(1000);  // Обновляем прогресс-бар каждую секунду

    // Подключение переключателя темы
    connect(ui->themeToggleButton, &QPushButton::clicked, this, &OTPWindow::toggleTheme);
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

        // Расшифровываем секретный ключ перед генерацией OTP
        QString decryptedSecret = accountManager.decryptSecret(QByteArray::fromHex(account.secret.toUtf8()));

        if (account.algorithm == "TOTP") {
            otp = generator.generateTOTP(decryptedSecret);
        } else {
            otp = generator.generateHOTP(decryptedSecret, account.counter);
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

    // Обновляем прогресс-бар
    updateProgressBar();
}

void OTPWindow::generateQRCodeForSelectedAccount() {
    // Получаем данные аккаунта
    Account selectedAccount = accounts[selectedAccountIndex];

    // Расшифровываем секретный ключ перед генерацией OTP
    QString decryptedSecret = accountManager.decryptSecret(QByteArray::fromHex(selectedAccount.secret.toUtf8()));

    // Генерируем OTP для выбранного аккаунта
    OtpGenerator generator;
    QString otp;
    if (selectedAccount.algorithm == "TOTP") {
        otp = generator.generateTOTP(decryptedSecret);
    } else {
        otp = generator.generateHOTP(decryptedSecret, selectedAccount.counter);
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

void OTPWindow::updateProgressBar() {
    // Обновляем прогресс-бар плавно
    int secondsPassed = QTime::currentTime().second() % 30;
    int percentage = (secondsPassed * 100) / 30;  // Обновляем на основе оставшегося времени для обновления TOTP
    ui->totpProgressBar->setValue(percentage);
}

void OTPWindow::toggleTheme() {
    static bool isDarkTheme = false;
    if (isDarkTheme) {
        // Применить светлую тему
        qApp->setStyleSheet("");
    } else {
        // Применить темную тему
        qApp->setStyleSheet("QWidget { background-color: #2e2e2e; color: #ffffff; } "
                            "QPushButton { background-color: #444444; color: #ffffff; } "
                            "QLineEdit, QTableWidget, QComboBox { background-color: #3e3e3e; color: #ffffff; }");
    }
    isDarkTheme = !isDarkTheme;
}
