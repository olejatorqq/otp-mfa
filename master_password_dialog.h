#ifndef MASTER_PASSWORD_DIALOG_H
#define MASTER_PASSWORD_DIALOG_H

#include <QDialog>

namespace Ui {
class MasterPasswordDialog;
}

class MasterPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MasterPasswordDialog(QWidget *parent = nullptr);
    ~MasterPasswordDialog();

    QString getPassword() const;
    QString getRecoveryKey() const;

    void setSettingMode(bool setting); // true = установка, false = ввод
    bool isSettingMode() const;

private slots:
    void onOkClicked();
    void onCancelClicked();
    void onShowPasswordToggled(bool checked);
    void onResetPasswordButtonClicked();

private:
    Ui::MasterPasswordDialog *ui;
    bool settingMode = false; // false = ввод, true = установка
};

#endif // MASTER_PASSWORD_DIALOG_H
