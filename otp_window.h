#ifndef OTPWINDOW_H
#define OTPWINDOW_H

#include <QMainWindow>
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
    void displayAccounts();
    void onAccountDoubleClicked(int row, int column);
    void updateAccountsAndQRCode();
    void generateQRCodeForSelectedAccount();
    void toggleTheme();  // Переключение между светлой и темной темами
    void updateProgressBar();

private:
    Ui::OTPWindow *ui;
    AccountManager accountManager;
    QList<Account> accounts;
    int selectedAccountIndex;
};

#endif // OTPWINDOW_H
