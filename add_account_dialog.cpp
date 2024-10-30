#include "add_account_dialog.h"
#include "ui_add_account.h"

AddAccountDialog::AddAccountDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAccountDialog)
{
    ui->setupUi(this);
}

AddAccountDialog::~AddAccountDialog() {
    delete ui;
}

Account AddAccountDialog::getAccount() const {
    Account account;
    account.name = ui->nameLineEdit->text();
    account.secret = ui->secretLineEdit->text();
    account.algorithm = "TOTP"; // Устанавливаем алгоритм по умолчанию
    account.counter = 0;
    return account;
}

void AddAccountDialog::on_buttonBox_accepted() {
    accept(); // Закрывает диалог и возвращает QDialog::Accepted
}

// Новый обработчик для кнопки "Cancel"
void AddAccountDialog::on_buttonBox_rejected() {
    reject(); // Закрывает диалог и возвращает QDialog::Rejected
}
