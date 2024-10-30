#ifndef OTPWINDOW_H
#define OTPWINDOW_H

#include <QMainWindow>
#include <QList>
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
    void onAccountDoubleClicked(int row, int column);  // Обработчик двойного нажатия на аккаунт
    void updateAccounts();  // Метод для обновления списка аккаунтов
    void updateTimeLeft();

private:
    Ui::OTPWindow *ui;
    QList<Account> accounts;  // Список аккаунтов
    AccountManager accountManager;
    int selectedAccountIndex;  // Индекс выбранного аккаунта

    void displayAccounts();  // Метод для отображения аккаунтов
};

#endif // OTPWINDOW_H
