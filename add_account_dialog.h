#ifndef ADD_ACCOUNT_DIALOG_H
#define ADD_ACCOUNT_DIALOG_H

#include <QDialog>
#include "account_manager.h"

namespace Ui {
class AddAccountDialog;
}

class AddAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAccountDialog(QWidget *parent = nullptr);
    ~AddAccountDialog();

    Account getAccount() const;
    void setAccount(const Account &account);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_showSecretCheckBox_toggled(bool checked);

    // Переключатели
    void on_manualInputRadioButton_toggled(bool checked);
    void on_scanInputRadioButton_toggled(bool checked);

    // Для кнопки «Дополнительные параметры»
    void toggleAdvancedParams();

    // Для кнопки «Сканировать»
    void on_scanQrButton_clicked();

    // Изменение типа (TOTP/HOTP)
    void on_typeComboBox_currentIndexChanged(const QString &type);

private:
    Ui::AddAccountDialog *ui;

    void updateTypeSettings(const QString &type);
    void parseUriAndFillFields(const QString &uri);  // М.б. пригодится, если после сканирования приходит otpauth://
    void handleDecodedQr(const QString &decodedText); // Обработчик результата сканирования
};

#endif // ADD_ACCOUNT_DIALOG_H
