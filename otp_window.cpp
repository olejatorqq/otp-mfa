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
    selectedAccountIndex(-1),
    timer(new QTimer(this))
{
    ui->setupUi(this);

    accounts = accountManager.getAccounts();
    displayAccounts();

    connect(ui->addButton, &QPushButton::clicked, this, &OTPWindow::onAddAccountClicked);
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &OTPWindow::filterAccounts);
    connect(ui->accountsTableWidget, &QTableWidget::cellDoubleClicked, this, &OTPWindow::onAccountDoubleClicked);
    connect(timer, &QTimer::timeout, this, &OTPWindow::updateAccounts);

    if (!timer->isActive()) {
        timer->start(1000);  // Запускаем таймер на обновление OTP и времени
    }
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
    ui->accountsTableWidget->setRowCount(0);

    for (const Account& account : accounts) {
        QString otp;
        OtpGenerator generator;
        otp = generator.generateTOTP(account.secret);

        int row = ui->accountsTableWidget->rowCount();
        ui->accountsTableWidget->insertRow(row);
        ui->accountsTableWidget->setItem(row, 0, new QTableWidgetItem(account.name));
        ui->accountsTableWidget->setItem(row, 1, new QTableWidgetItem(otp));

        int timeLeft = 30 - (QDateTime::currentSecsSinceEpoch() % 30);
        ui->accountsTableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(timeLeft)));

        if (timeLeft <= 5) {
            ui->accountsTableWidget->item(row, 1)->setBackgroundColor(Qt::red);
        } else {
            ui->accountsTableWidget->item(row, 1)->setBackgroundColor(Qt::white);
        }
    }

    filterAccounts(ui->searchLineEdit->text());
}


void OTPWindow::filterAccounts(const QString &filter) {
    for (int row = 0; row < ui->accountsTableWidget->rowCount(); ++row) {
        bool matches = ui->accountsTableWidget->item(row, 0)->text().contains(filter, Qt::CaseInsensitive);
        ui->accountsTableWidget->setRowHidden(row, !matches);
    }
}

void OTPWindow::onAccountDoubleClicked(int row, int /* column */) {
    selectedAccountIndex = row;
}

void OTPWindow::updateAccounts() {
    displayAccounts();
}
