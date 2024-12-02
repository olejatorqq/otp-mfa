#include "settings_dialog.h"
#include "ui_settings_dialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    // Устанавливаем значения по умолчанию или загружаем из настроек
    ui->intervalSpinBox->setMinimum(10);
    ui->intervalSpinBox->setMaximum(300);
    ui->intervalSpinBox->setValue(30); // Значение по умолчанию

    // Подключаем кнопки
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::reject);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

int SettingsDialog::getInterval() const
{
    return ui->intervalSpinBox->value();
}

void SettingsDialog::setInterval(int interval)
{
    ui->intervalSpinBox->setValue(interval);
}
