#ifndef OTP_WINDOW_H
#define OTP_WINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "account_manager.h"

namespace Ui {
class OTPWindow;
}

class OTPWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OTPWindow(QWidget *parent = nullptr);
    ~OTPWindow();

private slots:
    void onAddAccountClicked();

private:
    Ui::OTPWindow *ui;
    AccountManager accountManager;
    QList<Account> accounts;

    void displayAccounts();
};

#endif // OTP_WINDOW_H
