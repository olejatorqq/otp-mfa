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

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_showSecretCheckBox_toggled(bool checked);

private:
    Ui::AddAccountDialog *ui;
};

#endif // ADD_ACCOUNT_DIALOG_H
