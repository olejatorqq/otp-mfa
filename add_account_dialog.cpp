#include "add_account_dialog.h"
#include "ui_add_account.h"
#include "otp_generator.h"
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
    connect(ui->toggleAdvancedParamsButton, &QPushButton::clicked, this, &AddAccountDialog::toggleAdvancedParams);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AddAccountDialog::on_buttonBox_accepted);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &AddAccountDialog::on_buttonBox_rejected);

    // Устанавливаем значения по умолчанию
    ui->algorithmComboBox->setCurrentText("SHA1");
    ui->digitsSpinBox->setValue(6);
    ui->periodSpinBox->setValue(30);
    ui->typeComboBox->setCurrentText("По времени");
    updateTypeSettings("По времени");

    // Устанавливаем минимальный размер окна
    setMinimumSize(400, 300);
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

    QString typeText = ui->typeComboBox->currentText();
    if (typeText == "По времени") {
        account.type = "TOTP";
        account.period = ui->periodSpinBox->value();
        account.counter = 0; // Для TOTP счетчик не используется
    } else if (typeText == "По счетчику") {
        account.type = "HOTP";
        account.period = 0; // Для HOTP период не используется
        account.counter = ui->counterSpinBox->value();
    }

    return account;
}

void AddAccountDialog::setAccount(const Account &account) {
    // Устанавливаем данные в поля диалога
    ui->nameLineEdit->setText(account.name);
    ui->secretLineEdit->setText(account.secret);
    ui->algorithmComboBox->setCurrentText(account.algorithm);
    ui->digitsSpinBox->setValue(account.digits);

    if (account.type == "TOTP") {
        ui->typeComboBox->setCurrentText("По времени");
        ui->periodSpinBox->setValue(account.period);
    } else if (account.type == "HOTP") {
        ui->typeComboBox->setCurrentText("По счетчику");
        ui->counterSpinBox->setValue(account.counter);
    }

    // Устанавливаем метод ввода на "Ручной ввод"
    ui->manualInputRadioButton->setChecked(true);
    ui->inputMethodStackedWidget->setCurrentWidget(ui->manualInputWidget);
    ui->advancedParamsGroupBox->setVisible(false);

    // Автоматически подстраиваем размер окна
    adjustSize();
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

        // Валидация секретного ключа
        QString errorMessage;
        QByteArray decodedKey = OtpGenerator::base32Decode(ui->secretLineEdit->text().trimmed(), errorMessage);
        if (decodedKey.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", errorMessage);
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

        // Валидация секретного ключа после парсинга URI
        QString errorMessage;
        QByteArray decodedKey = OtpGenerator::base32Decode(ui->secretLineEdit->text().trimmed(), errorMessage);
        if (decodedKey.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", errorMessage);
            return;
        }
    }
    accept();
}

void AddAccountDialog::on_buttonBox_rejected() {
    reject();
}

void AddAccountDialog::on_showSecretCheckBox_toggled(bool checked) {
    ui->secretLineEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}

void AddAccountDialog::on_manualInputRadioButton_toggled(bool checked) {
    if (checked) {
        ui->inputMethodStackedWidget->setCurrentWidget(ui->manualInputWidget);
    }
}

void AddAccountDialog::on_uriInputRadioButton_toggled(bool checked) {
    if (checked) {
        ui->inputMethodStackedWidget->setCurrentWidget(ui->uriInputWidget);
    }
}

void AddAccountDialog::on_uriLineEdit_textChanged(const QString &text) {
    parseUriAndFillFields(text);
}

void AddAccountDialog::on_typeComboBox_currentIndexChanged(const QString &type) {
    updateTypeSettings(type);
}

void AddAccountDialog::toggleAdvancedParams() {
    bool isVisible = ui->advancedParamsGroupBox->isVisible();
    ui->advancedParamsGroupBox->setVisible(!isVisible);

    if (isVisible) {
        ui->toggleAdvancedParamsButton->setText("Дополнительные параметры");
    } else {
        ui->toggleAdvancedParamsButton->setText("Скрыть дополнительные параметры");
    }

    adjustSize();
}

void AddAccountDialog::updateTypeSettings(const QString &type) {
    if (type == "По времени") {
        ui->periodSpinBox->setEnabled(true);
        ui->counterSpinBox->setEnabled(false);
    } else if (type == "По счетчику") {
        ui->periodSpinBox->setEnabled(false);
        ui->counterSpinBox->setEnabled(true);
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
    QString label = QUrl::fromPercentEncoding(url.path().mid(1).toUtf8()); // Удаляем начальный '/' и декодируем
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

    if (type.toLower() == "totp") {
        ui->typeComboBox->setCurrentText("По времени");
        ui->periodSpinBox->setValue(period == 0 ? 30 : period);
    } else if (type.toLower() == "hotp") {
        ui->typeComboBox->setCurrentText("По счетчику");
        ui->counterSpinBox->setValue(counter);
    } else {
        QMessageBox::warning(this, "Ошибка", "Неизвестный тип алгоритма в URI.");
        return;
    }
}
