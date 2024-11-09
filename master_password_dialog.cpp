#include "master_password_dialog.h"
#include "ui_master_password_dialog.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QCheckBox>

MasterPasswordDialog::MasterPasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MasterPasswordDialog)
{
    ui->setupUi(this);
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);

    // Подключение кнопок к слотам
    connect(ui->okButton, &QPushButton::clicked, this, &MasterPasswordDialog::onOkClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &MasterPasswordDialog::onCancelClicked);

    // Подключение чекбокса к слоту для показа/скрытия пароля
    connect(ui->showPasswordCheckBox, &QCheckBox::toggled, this, &MasterPasswordDialog::onShowPasswordToggled);
}

MasterPasswordDialog::~MasterPasswordDialog()
{
    delete ui;
}

QString MasterPasswordDialog::getPassword() const {
    return ui->passwordLineEdit->text();
}

void MasterPasswordDialog::onOkClicked() {
    if (ui->passwordLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите мастер-пароль.");
        return;
    }
    accept();
}

void MasterPasswordDialog::onCancelClicked() {
    reject();
}

void MasterPasswordDialog::onShowPasswordToggled(bool checked) {
    ui->passwordLineEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}
