#ifndef OTP_WINDOW_H
#define OTP_WINDOW_H

#include <QMainWindow>
#include <QList>
#include <QWidget>
#include <QTimer>
#include <QSettings>
#include "account_manager.h"

// Предварительное объявление классов
class QRGenerator;
class ScreenshotTool;

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
    void filterAccounts(const QString &filter);
    void updateAccounts();
    void toggleTheme();
    void openSettingsDialog();

    // Меню по правому клику
    void showContextMenuForAccount(int index, const QPoint &globalPos);
    void editAccount(int index);
    void deleteAccount(int index);

    // ====== Новые слоты для работы с QR-кодом ======
    void on_scanQrButton_clicked();      // Слот, вызываемый при нажатии кнопки "Сканировать QR"
    void handleQrDecoded(const QString &decodedText);
    void handleQrError(const QString &errorString);

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

    // ====== Новое поле класса ======
    QRGenerator *qrGenerator;  // Указатель на класс, обрабатывающий декодирование
};

#endif // OTP_WINDOW_H
