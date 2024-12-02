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
    void setAccount(const Account &account); // Добавленный метод

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_showSecretCheckBox_toggled(bool checked);
    void on_manualInputRadioButton_toggled(bool checked);
    void on_uriInputRadioButton_toggled(bool checked);
    void on_uriLineEdit_textChanged(const QString &text);
    void on_typeComboBox_currentIndexChanged(const QString &type);

private:
    Ui::AddAccountDialog *ui;

    void parseUriAndFillFields(const QString &uri);
    void updateTypeSettings(const QString &type);
};

#endif // ADD_ACCOUNT_DIALOG_H
