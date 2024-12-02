#include "add_account_dialog.h"
#include "ui_add_account.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QUrl>
#include <QUrlQuery>

AddAccountDialog::AddAccountDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAccountDialog)
{
    ui->setupUi(this);

    // Подключаем сигналы
    connect(ui->showSecretCheckBox, &QCheckBox::toggled, this, &AddAccountDialog::on_showSecretCheckBox_toggled);
    connect(ui->manualInputRadioButton, &QRadioButton::toggled, this, &AddAccountDialog::on_manualInputRadioButton_toggled);
    connect(ui->uriInputRadioButton, &QRadioButton::toggled, this, &AddAccountDialog::on_uriInputRadioButton_toggled);
    connect(ui->uriLineEdit, &QLineEdit::textChanged, this, &AddAccountDialog::on_uriLineEdit_textChanged);
    connect(ui->typeComboBox, &QComboBox::currentTextChanged, this, &AddAccountDialog::on_typeComboBox_currentIndexChanged);

    // Устанавливаем значения по умолчанию
    ui->algorithmComboBox->setCurrentText("SHA1");
    ui->digitsSpinBox->setValue(6);
    ui->periodSpinBox->setValue(30);
    ui->typeComboBox->setCurrentText("TOTP");
    updateTypeSettings("TOTP");
}

AddAccountDialog::~AddAccountDialog() {
    delete ui;
}

Account AddAccountDialog::getAccount() const {
    Account account;
    account.name = ui->nameLineEdit->text().trimmed();
    account.secret = ui->secretLineEdit->text().trimmed();
    account.algorithm = ui->algorithmComboBox->currentText();
    account.digits = ui->digitsSpinBox->value();
    account.type = ui->typeComboBox->currentText();

    if (account.type == "TOTP") {
        account.period = ui->periodSpinBox->value();
        account.counter = 0; // Для TOTP счетчик не используется
    } else if (account.type == "HOTP") {
        account.period = 0; // Для HOTP период не используется
        account.counter = ui->counterSpinBox->value();
    }

    return account;
}

void AddAccountDialog::on_buttonBox_accepted() {
    // Валидация введенных данных
    if (ui->manualInputRadioButton->isChecked()) {
        if (ui->nameLineEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите название ресурса.");
            return;
        }
        if (ui->secretLineEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите секретный ключ.");
            return;
        }
    } else if (ui->uriInputRadioButton->isChecked()) {
        if (ui->uriLineEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите otpauth:// URI.");
            return;
        }
        // Парсим URI и заполняем поля
        parseUriAndFillFields(ui->uriLineEdit->text().trimmed());
        if (ui->nameLineEdit->text().isEmpty() || ui->secretLineEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Не удалось распознать URI.");
            return;
        }
    }
    accept(); // Закрываем диалог с результатом Accepted
}

void AddAccountDialog::on_buttonBox_rejected() {
    reject(); // Закрываем диалог с результатом Rejected
}

void AddAccountDialog::on_showSecretCheckBox_toggled(bool checked) {
    ui->secretLineEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}

void AddAccountDialog::on_manualInputRadioButton_toggled(bool checked) {
    ui->manualInputWidget->setVisible(checked);
    ui->uriInputWidget->setVisible(!checked);
}

void AddAccountDialog::on_uriInputRadioButton_toggled(bool checked) {
    ui->manualInputWidget->setVisible(!checked);
    ui->uriInputWidget->setVisible(checked);
}

void AddAccountDialog::on_uriLineEdit_textChanged(const QString &text) {
    // Можно автоматически парсить URI при изменении текста
    // parseUriAndFillFields(text);
}

void AddAccountDialog::on_typeComboBox_currentIndexChanged(const QString &type) {
    updateTypeSettings(type);
}

void AddAccountDialog::updateTypeSettings(const QString &type) {
    if (type == "TOTP") {
        ui->typeSettingsStackedWidget->setCurrentWidget(ui->totpSettingsPage);
    } else if (type == "HOTP") {
        ui->typeSettingsStackedWidget->setCurrentWidget(ui->hotpSettingsPage);
    }
}

void AddAccountDialog::parseUriAndFillFields(const QString &uri) {
    // Проверяем, начинается ли строка с "otpauth://"
    if (!uri.startsWith("otpauth://")) {
        QMessageBox::warning(this, "Ошибка", "Неверный формат URI.");
        return;
    }

    QUrl url(uri);
    QString type = url.host(); // "totp" или "hotp"
    QString label = url.path().mid(1); // Удаляем начальный '/'
    QUrlQuery query(url);

    QString secret = query.queryItemValue("secret");
    QString issuer = query.queryItemValue("issuer");
    QString algorithm = query.queryItemValue("algorithm").toUpper();
    int digits = query.queryItemValue("digits").toInt();
    int period = query.queryItemValue("period").toInt();
    int counter = query.queryItemValue("counter").toInt();

    // Заполняем поля
    ui->nameLineEdit->setText(issuer.isEmpty() ? label : issuer + " (" + label + ")");
    ui->secretLineEdit->setText(secret);
    ui->algorithmComboBox->setCurrentText(algorithm.isEmpty() ? "SHA1" : algorithm);
    ui->digitsSpinBox->setValue(digits == 0 ? 6 : digits);
    ui->typeComboBox->setCurrentText(type.toUpper());

    if (type.toLower() == "totp") {
        ui->periodSpinBox->setValue(period == 0 ? 30 : period);
    } else if (type.toLower() == "hotp") {
        ui->counterSpinBox->setValue(counter);
    }
}
