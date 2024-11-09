#include "add_account_dialog.h"
#include "ui_add_account.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QCheckBox>

AddAccountDialog::AddAccountDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAccountDialog)
{
    ui->setupUi(this);

    // Подключаем сигнал чекбокса к слоту
    connect(ui->showSecretCheckBox, &QCheckBox::toggled, this, &AddAccountDialog::on_showSecretCheckBox_toggled);
}

AddAccountDialog::~AddAccountDialog() {
    delete ui;
}

Account AddAccountDialog::getAccount() const {
    Account account;
    account.name = ui->nameLineEdit->text().trimmed();
    account.secret = ui->secretLineEdit->text().trimmed();
    account.algorithm = "TOTP"; // Устанавливаем алгоритм по умолчанию
    account.counter = 0;
    return account;
}

void AddAccountDialog::on_buttonBox_accepted() {
    // Валидация введенных данных
    if (ui->nameLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите название ресурса.");
        return;
    }
    if (ui->secretLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите секретный ключ.");
        return;
    }
    accept(); // Закрываем диалог с результатом Accepted
}

void AddAccountDialog::on_buttonBox_rejected() {
    reject(); // Закрываем диалог с результатом Rejected
}

void AddAccountDialog::on_showSecretCheckBox_toggled(bool checked) {
    ui->secretLineEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}
