#include "otp_window.h"
#include "master_password_dialog.h"
#include "account_manager.h"
#include "encryption_utils.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication a(argc, argv);

    // (1) Выясняем, есть ли уже установленный пароль
    bool isSet = AccountManager::instance().loadStoredMasterPasswordHash();
    if (!isSet) {
        // Пароль не установлен => нужно задать
        MasterPasswordDialog setDialog;
        setDialog.setSettingMode(true); // режим "создания"
        if (setDialog.exec() == QDialog::Accepted) {
            QString newPass = setDialog.getPassword();
            AccountManager::instance().storeMasterPasswordHash(newPass);
            QMessageBox::information(nullptr, "Мастер-пароль",
                                     "Новый мастер-пароль успешно установлен!");
        } else {
            // Пользователь отменил установку => выходим
            return 0;
        }
    }

    // (2) Если пароль уже есть, даём ввести (до 3 попыток)
    // Или, если только что установили, он тоже "есть", так что
    // при новом запуске снова будет ввод.
    int attemptCount = 0;
    const int maxAttempts = 3;

    while (true) {
        MasterPasswordDialog inputDialog;
        inputDialog.setSettingMode(false); // режим "ввода"
        int res = inputDialog.exec();
        if (res == QDialog::Accepted) {
            QString pass = inputDialog.getPassword();
            bool ok = AccountManager::instance().verifyMasterPassword(pass);
            if (ok) {
                // Успех: запускаем окно
                OTPWindow w;
                w.show();

                QObject::connect(&a, &QApplication::aboutToQuit, []() {
                    AccountManager::instance().logEvent("Пользователь вышел из приложения");
                });
                return a.exec();
            } else {
                attemptCount++;
                if (attemptCount < maxAttempts) {
                    QMessageBox::warning(nullptr, "Неверный пароль",
                                         QString("Осталось попыток: %1")
                                             .arg(maxAttempts - attemptCount));
                } else {
                    // 3-я ошибка => выходим
                    QMessageBox::critical(nullptr, "Ошибка",
                                          "Вы 3 раза ввели неверный пароль. Приложение будет закрыто.");
                    return 0;
                }
            }
        } else {
            // Нажал Cancel => выходим
            return 0;
        }
    }
}
