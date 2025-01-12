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

    void setSettingMode(bool setting);
    bool isSettingMode() const;

private slots:
    void onOkClicked();
    void onCancelClicked();
    void onShowPasswordToggled(bool checked);

private:
    Ui::MasterPasswordDialog *ui;
    bool settingMode = false; // false = вводим, true = задаём первый раз
};

#endif // MASTER_PASSWORD_DIALOG_H
