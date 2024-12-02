#ifndef OTP_WINDOW_H
#define OTP_WINDOW_H

#include <QMainWindow>
#include <QList>
#include <QWidget>
#include <QTimer>
#include <QSettings>

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

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onAddAccountClicked();
    void filterAccounts(const QString &filter);
    void updateAccounts();
    void toggleTheme();
    void openSettingsDialog();

    // Добавленные слоты для контекстного меню
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
};

#endif // OTP_WINDOW_H
