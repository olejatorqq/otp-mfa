#ifndef OTP_WINDOW_H
#define OTP_WINDOW_H

#include <QMainWindow>
<<<<<<< HEAD
#include <QList>
#include <QWidget>
#include <QTimer>
#include <QSettings>

=======
>>>>>>> 9aa011e288283510cac46a946f74ac09ede64cef
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

    void onDataFetched(const QByteArray &data);
    void onFetchError(const QString &errorString);
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onAddAccountClicked();
<<<<<<< HEAD
    void filterAccounts(const QString &filter);
    void updateAccounts();
    void toggleTheme();
    void openSettingsDialog();

    // Слоты для контекстного меню
    void showContextMenuForAccount(int index, const QPoint &globalPos);
    void editAccount(int index);
    void deleteAccount(int index);

private:
    void displayAccounts();
    void copyCodeToClipboard(int index);
    void applyTheme();

    Ui::OTPWindow *ui;
    QList<Account> accounts;
    QList<QWidget*> accountWidgets;
    QTimer *timer;
    bool darkThemeEnabled;
    int interval;
=======
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
>>>>>>> 9aa011e288283510cac46a946f74ac09ede64cef
};

#endif // OTP_WINDOW_H
