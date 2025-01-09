#include "password_dialog.h"
#include "ui_password_dialog.h"

PasswordDialog::PasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordDialog)
{
    ui->setupUi(this);
}

PasswordDialog::~PasswordDialog()
{
    delete ui;
}

QString PasswordDialog::getPassword() const {
    return password;
}

void PasswordDialog::on_acceptButton_clicked() {
    password = ui->passwordEdit->text();
    accept();
}
