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

    connect(ui->okButton, &QPushButton::clicked,
            this, &MasterPasswordDialog::onOkClicked);
    connect(ui->cancelButton, &QPushButton::clicked,
            this, &MasterPasswordDialog::onCancelClicked);

    connect(ui->showPasswordCheckBox, &QCheckBox::toggled,
            this, &MasterPasswordDialog::onShowPasswordToggled);
}

MasterPasswordDialog::~MasterPasswordDialog()
{
    delete ui;
}

QString MasterPasswordDialog::getPassword() const
{
    return ui->passwordLineEdit->text();
}

void MasterPasswordDialog::setSettingMode(bool setting)
{
    settingMode = setting;
    if (settingMode) {
        ui->label->setText("Создайте новый мастер-пароль:");
        setWindowTitle("Установка мастер-пароля");
    } else {
        ui->label->setText("Введите мастер-пароль:");
        setWindowTitle("Ввод мастер-пароля");
    }
}

bool MasterPasswordDialog::isSettingMode() const
{
    return settingMode;
}

void MasterPasswordDialog::onOkClicked()
{
    QString pass = ui->passwordLineEdit->text().trimmed();
    if (pass.isEmpty()) {
        QMessageBox::warning(this, "Ошибка",
                             settingMode ? "Введите новый мастер-пароль."
                                         : "Введите мастер-пароль.");
        return;
    }
    accept();
}

void MasterPasswordDialog::onCancelClicked()
{
    reject();
}

void MasterPasswordDialog::onShowPasswordToggled(bool checked)
{
    ui->passwordLineEdit->setEchoMode(checked ? QLineEdit::Normal
                                              : QLineEdit::Password);
}
