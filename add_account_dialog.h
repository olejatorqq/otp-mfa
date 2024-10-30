#ifndef ADD_ACCOUNT_DIALOG_H
#define ADD_ACCOUNT_DIALOG_H

#include <QDialog>
#include "account_manager.h"

namespace Ui {
class AddAccountDialog;
}

class AddAccountDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddAccountDialog(QWidget *parent = nullptr);
    ~AddAccountDialog();

    Account getAccount() const;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected(); // Добавлено


private:
    Ui::AddAccountDialog *ui;
    Account account;
};

#endif // ADD_ACCOUNT_DIALOG_H
