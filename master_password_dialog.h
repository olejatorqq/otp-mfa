#ifndef MASTER_PASSWORD_DIALOG_H
#define MASTER_PASSWORD_DIALOG_H

#include <QDialog>
#include <QObject>

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

private slots:
    void onOkClicked();
    void onCancelClicked();
    void onShowPasswordToggled(bool checked);

private:
    Ui::MasterPasswordDialog *ui;
};

#endif // MASTER_PASSWORD_DIALOG_H
