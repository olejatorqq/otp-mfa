#include "add_account_dialog.h"
#include "ui_add_account.h"

#include "otp_generator.h"
#include "screenshot_tool.h"
#include "qr_generator.h"

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
    connect(ui->showSecretCheckBox, &QCheckBox::toggled,
            this, &AddAccountDialog::on_showSecretCheckBox_toggled);

    // Радиокнопки
    connect(ui->manualInputRadioButton, &QRadioButton::toggled,
            this, &AddAccountDialog::on_manualInputRadioButton_toggled);
    connect(ui->scanInputRadioButton, &QRadioButton::toggled,
            this, &AddAccountDialog::on_scanInputRadioButton_toggled);

    // Дополнительные параметры
    connect(ui->toggleAdvancedParamsButton, &QPushButton::clicked,
            this, &AddAccountDialog::toggleAdvancedParams);

    // Кнопка «Сканировать»
    connect(ui->scanQrButton, &QPushButton::clicked,
            this, &AddAccountDialog::on_scanQrButton_clicked);

    // Кнопки OK/Cancel
    connect(ui->buttonBox, &QDialogButtonBox::accepted,
            this, &AddAccountDialog::on_buttonBox_accepted);
    connect(ui->buttonBox, &QDialogButtonBox::rejected,
            this, &AddAccountDialog::on_buttonBox_rejected);

    // Тип ключа
    connect(ui->typeComboBox, &QComboBox::currentTextChanged,
            this, &AddAccountDialog::on_typeComboBox_currentIndexChanged);

    // Устанавливаем значения по умолчанию
    ui->algorithmComboBox->setCurrentText("SHA1");
    ui->digitsSpinBox->setValue(6);
    ui->periodSpinBox->setValue(30);
    ui->typeComboBox->setCurrentText("По времени");

    updateTypeSettings("По времени");

    // По умолчанию включена радиокнопка «Ручной ввод»
    ui->inputMethodStackedWidget->setCurrentWidget(ui->manualInputWidget);
}

AddAccountDialog::~AddAccountDialog()
{
    delete ui;
}

// ==================================================================
// ===============     СЛОТЫ для RadioButton    ======================
// ==================================================================

void AddAccountDialog::on_manualInputRadioButton_toggled(bool checked)
{
    if (checked) {
        ui->inputMethodStackedWidget->setCurrentWidget(ui->manualInputWidget);
    }
}

void AddAccountDialog::on_scanInputRadioButton_toggled(bool checked)
{
    if (checked) {
        ui->inputMethodStackedWidget->setCurrentWidget(ui->scanInputWidget);
    }
}

// ==================================================================
// ===============  Сканирование QR  ================================
// ==================================================================
void AddAccountDialog::on_scanQrButton_clicked()
{
    ScreenshotTool *tool = new ScreenshotTool(this);
    tool->setAttribute(Qt::WA_DeleteOnClose);

    QRGenerator *qrGenerator = new QRGenerator(tool);

    // Когда qrGenerator успешно декодирует
    connect(qrGenerator, &QRGenerator::qrDecoded, this,
            [this, tool](const QString &decodedText) {
                tool->close();
                handleDecodedQr(decodedText);
            });

    // Когда не удалось
    connect(qrGenerator, &QRGenerator::qrError, this,
            [this, tool](const QString &err) {
                tool->close();
                QMessageBox::warning(this, "Ошибка", "Не удалось распознать QR: " + err);
            });

    // Когда пользователь выделил область
    connect(tool, &ScreenshotTool::areaSelected,
            qrGenerator, &QRGenerator::processPixmap);

    tool->showFullScreen();
}

void AddAccountDialog::handleDecodedQr(const QString &decodedText)
{
    // Если нужно парсить otpauth://
    if (!decodedText.startsWith("otpauth://", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, "Ошибка", "QR-код не содержит otpauth:// URI");
        return;
    }

    // Автоматически распарсить и заполнить поля
    parseUriAndFillFields(decodedText);

    // Переключаемся обратно на «Ручной ввод»?
    // Или оставляем как есть?
    // Логично оставить на «Ручной ввод», чтобы поля отобразились
    ui->manualInputRadioButton->setChecked(true);
    ui->inputMethodStackedWidget->setCurrentWidget(ui->manualInputWidget);
}

// ==================================================================
// ===============   Дополнительные параметры  =======================
// ==================================================================

void AddAccountDialog::toggleAdvancedParams()
{
    bool isVisible = ui->advancedParamsGroupBox->isVisible();
    ui->advancedParamsGroupBox->setVisible(!isVisible);

    // Меняем текст кнопки
    if (isVisible)
        ui->toggleAdvancedParamsButton->setText("Дополнительные параметры");
    else
        ui->toggleAdvancedParamsButton->setText("Скрыть дополнительные параметры");

    adjustSize();
}

// ==================================================================
// ===============   Основной CRUD диалога  =========================
// ==================================================================

Account AddAccountDialog::getAccount() const
{
    Account account;
    account.name      = ui->nameLineEdit->text().trimmed();
    account.secret    = ui->secretLineEdit->text().trimmed();
    account.algorithm = ui->algorithmComboBox->currentText();
    account.digits    = ui->digitsSpinBox->value();

    QString typeText = ui->typeComboBox->currentText();
    if (typeText == "По времени") {
        account.type   = "TOTP";
        account.period = ui->periodSpinBox->value();
        account.counter = 0; // TOTP не использует счётчик
    } else {
        account.type   = "HOTP";
        account.period = 0;
        account.counter = ui->counterSpinBox->value();
    }
    return account;
}

void AddAccountDialog::setAccount(const Account &account)
{
    ui->nameLineEdit->setText(account.name);
    ui->secretLineEdit->setText(account.secret);
    ui->algorithmComboBox->setCurrentText(account.algorithm);
    ui->digitsSpinBox->setValue(account.digits);

    if (account.type == "TOTP") {
        ui->typeComboBox->setCurrentText("По времени");
        ui->periodSpinBox->setValue(account.period);
        ui->counterSpinBox->setValue(0);
    } else {
        ui->typeComboBox->setCurrentText("По счетчику");
        ui->periodSpinBox->setValue(0);
        ui->counterSpinBox->setValue(account.counter);
    }

    // По умолчанию, показываем форму ручного ввода
    ui->manualInputRadioButton->setChecked(true);
    ui->inputMethodStackedWidget->setCurrentWidget(ui->manualInputWidget);
    ui->advancedParamsGroupBox->setVisible(false);

    adjustSize();
}

void AddAccountDialog::on_buttonBox_accepted()
{
    // Проверяем, какая радиокнопка выбрана
    if (ui->manualInputRadioButton->isChecked()) {
        // Проверяем, что поля не пустые
        if (ui->nameLineEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Введите название ресурса");
            return;
        }
        if (ui->secretLineEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Введите секретный ключ");
            return;
        }
        // Проверка base32
        QString errorMessage;
        QByteArray decodedKey =
            OtpGenerator::base32Decode(ui->secretLineEdit->text().trimmed(), errorMessage);
        if (decodedKey.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", errorMessage);
            return;
        }
    }
    else if (ui->scanInputRadioButton->isChecked()) {
        // Теоретически, в режиме «Сканирование QR»
        // мы уже должны были распознать и заполнить поля
        // Проверяем, не пустое ли поле secret
        if (ui->secretLineEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Сначала просканируйте QR-код");
            return;
        }
    }

    accept();
}

void AddAccountDialog::on_buttonBox_rejected()
{
    reject();
}

// ==================================================================
// ===============   Прочие слоты / методы   ========================
// ==================================================================

void AddAccountDialog::on_showSecretCheckBox_toggled(bool checked)
{
    ui->secretLineEdit->setEchoMode(checked ?
                                        QLineEdit::Normal : QLineEdit::Password);
}

void AddAccountDialog::on_typeComboBox_currentIndexChanged(const QString &type)
{
    updateTypeSettings(type);
}

void AddAccountDialog::updateTypeSettings(const QString &type)
{
    if (type == "По времени") {
        ui->periodSpinBox->setEnabled(true);
        ui->counterSpinBox->setEnabled(false);
    } else {
        // HOTP
        ui->periodSpinBox->setEnabled(false);
        ui->counterSpinBox->setEnabled(true);
    }
}

void AddAccountDialog::parseUriAndFillFields(const QString &uri)
{
    // Например, если QR-код был otpauth://
    if (!uri.startsWith("otpauth://", Qt::CaseInsensitive)) {
        // Можно просто return
        return;
    }
    QUrl url(uri);
    if (!url.isValid()) {
        return;
    }
    QString type = url.host().toLower(); // totp/hotp
    QString path = url.path().mid(1);    // без слеша
    QUrlQuery query(url);

    QString secret    = query.queryItemValue("secret");
    QString issuer    = query.queryItemValue("issuer");
    QString algorithm = query.queryItemValue("algorithm").toUpper();
    int digits        = query.queryItemValue("digits").toInt();
    int period        = query.queryItemValue("period").toInt();
    int counter       = query.queryItemValue("counter").toInt();

    // Простейший разбор поля path: issuer:Account
    // В зависимости от реального формата
    QString fullName;
    if (!issuer.isEmpty()) {
        fullName = issuer + " (" + path + ")";
    } else {
        fullName = path;
    }

    ui->nameLineEdit->setText(fullName);
    ui->secretLineEdit->setText(secret);
    if (!algorithm.isEmpty()) {
        ui->algorithmComboBox->setCurrentText(algorithm);
    }
    if (digits > 0) {
        ui->digitsSpinBox->setValue(digits);
    }
    if (type == "totp") {
        ui->typeComboBox->setCurrentText("По времени");
        if (period > 0) {
            ui->periodSpinBox->setValue(period);
        }
    } else if (type == "hotp") {
        ui->typeComboBox->setCurrentText("По счетчику");
        if (counter > 0) {
            ui->counterSpinBox->setValue(counter);
        }
    }
}
