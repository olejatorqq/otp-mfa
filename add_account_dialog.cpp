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
    account.algorithm = ui->algorithmComboBox->currentText();
    account.counter = 0;
    return account;
}

void AddAccountDialog::on_buttonBox_accepted() {
    accept();
}
