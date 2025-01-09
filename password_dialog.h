#ifndef PASSWORD_DIALOG_H
#define PASSWORD_DIALOG_H

#include <QDialog>

namespace Ui {
class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordDialog(QWidget *parent = nullptr);
    ~PasswordDialog();

    QString getPassword() const;

private slots:
    void on_acceptButton_clicked();

private:
    Ui::PasswordDialog *ui;
    QString password;
};

#endif // PASSWORD_DIALOG_H
