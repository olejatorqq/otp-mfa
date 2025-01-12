#include "master_password_dialog.h"
#include "ui_master_password_dialog.h"
#include "recovery_manager.h"
#include "account_manager.h"

#include <QMessageBox>
#include <QInputDialog>

MasterPasswordDialog::MasterPasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MasterPasswordDialog)
{
    ui->setupUi(this);
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);

    // Подключение кнопок к слотам
    connect(ui->okButton, &QPushButton::clicked,
            this, &MasterPasswordDialog::onOkClicked);
    connect(ui->cancelButton, &QPushButton::clicked,
            this, &MasterPasswordDialog::onCancelClicked);

    // Чекбокс "Показать пароль"
    connect(ui->showPasswordCheckBox, &QCheckBox::toggled,
            this, &MasterPasswordDialog::onShowPasswordToggled);

    // Кнопка "Сбросить мастер-пароль"
    connect(ui->resetPasswordButton, &QPushButton::clicked,
            this, &MasterPasswordDialog::onResetPasswordButtonClicked);

    // Изначально скрываем Recovery Key и кнопку сброса
    ui->recoveryKeyLabel->setVisible(false);
    ui->resetPasswordButton->setVisible(false);
}

MasterPasswordDialog::~MasterPasswordDialog()
{
    delete ui;
}

QString MasterPasswordDialog::getPassword() const
{
    return ui->passwordLineEdit->text();
}

QString MasterPasswordDialog::getRecoveryKey() const
{
    return ui->recoveryKeyLabel->text();
}

void MasterPasswordDialog::setSettingMode(bool setting)
{
    settingMode = setting;
    if (settingMode) {
        ui->label->setText("Создайте новый мастер-пароль:");
        setWindowTitle("Установка мастер-пароля");
        // Скрываем Recovery Key и кнопку сброса при установке
        ui->recoveryKeyLabel->setVisible(false);
        ui->resetPasswordButton->setVisible(false);
    } else {
        ui->label->setText("Введите мастер-пароль:");
        setWindowTitle("Ввод мастер-пароля");
        // Показываем кнопку сброса пароля
        ui->resetPasswordButton->setVisible(true);
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
                             settingMode ? "Пожалуйста, введите новый мастер-пароль."
                                         : "Пожалуйста, введите мастер-пароль.");
        return;
    }

    if (settingMode) {
        // Генерация Recovery Key при установке нового мастер-пароля
        QString recoveryKey = RecoveryManager::instance().generateRecoveryKey();
        QByteArray hashedRecoveryKey = RecoveryManager::instance().hashRecoveryKey(recoveryKey);

        // Сохранение Recovery Key в базе данных
        if (!AccountManager::instance().setRecoveryKey(recoveryKey)) {
            QMessageBox::critical(this, "Ошибка",
                                  "Не удалось сохранить Recovery Key. Попробуйте снова.");
            return;
        }

        // Сохранение мастер-пароля
        if (!AccountManager::instance().storeMasterPasswordHash(pass)) {
            QMessageBox::critical(this, "Ошибка",
                                  "Не удалось сохранить мастер-пароль. Попробуйте снова.");
            return;
        }

        // Отображение Recovery Key пользователю
        ui->recoveryKeyLabel->setText("Ваш Recovery Key:\n" + recoveryKey);
        ui->recoveryKeyLabel->setVisible(true);

        QMessageBox::information(this, "Recovery Key",
                                 "Ваш Recovery Key:\n" + recoveryKey +
                                     "\n\nСохраните его в безопасном месте. Он понадобится для восстановления мастер-пароля.");
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

void MasterPasswordDialog::onResetPasswordButtonClicked()
{
    // Диалог для ввода Recovery Key и нового мастер-пароля
    bool ok;
    QString recoveryKey = QInputDialog::getText(this, tr("Recovery Key"),
                                                tr("Введите ваш Recovery Key:"),
                                                QLineEdit::Normal, "", &ok);
    if (!ok || recoveryKey.isEmpty()) {
        return;
    }

    // Проверка Recovery Key
    QByteArray storedRecoveryHash = AccountManager::instance().getSettingValue("recoveryKeyHash");
    if (storedRecoveryHash.isEmpty()) {
        QMessageBox::critical(this, "Ошибка",
                              "Recovery Key не установлен. Сброс мастер-пароля невозможен.");
        return;
    }

    if (!RecoveryManager::instance().verifyRecoveryKey(recoveryKey, storedRecoveryHash)) {
        QMessageBox::critical(this, "Ошибка",
                              "Неверный Recovery Key.");
        return;
    }

    // Ввод нового мастер-пароля
    QString newPass = QInputDialog::getText(this, tr("Новый мастер-пароль"),
                                            tr("Введите новый мастер-пароль:"),
                                            QLineEdit::Password, "", &ok);
    if (!ok || newPass.isEmpty()) {
        QMessageBox::warning(this, "Ошибка",
                             "Новый мастер-пароль не введён.");
        return;
    }

    // Сохранение нового мастер-пароля
    if (!AccountManager::instance().storeMasterPasswordHash(newPass)) {
        QMessageBox::critical(this, "Ошибка",
                              "Не удалось сохранить новый мастер-пароль.");
        return;
    }

    QMessageBox::information(this, "Успех",
                             "Мастер-пароль успешно сброшен.");
}
