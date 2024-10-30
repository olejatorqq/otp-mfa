#ifndef OTPWINDOW_H
#define OTPWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTimer>
#include "account_manager.h"
#include <QDateTime>

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
    void filterAccounts(const QString &filter);  // Метод для фильтрации аккаунтов

private:
    Ui::OTPWindow *ui;
    QList<Account> accounts;  // Список аккаунтов
    AccountManager accountManager;
    int selectedAccountIndex;  // Индекс выбранного аккаунта
    QTimer *timer;  // Указатель на таймер

    void displayAccounts();  // Метод для отображения аккаунтов
};

#endif // OTPWINDOW_H
