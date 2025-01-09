#include "otp_window.h"
#include "master_password_dialog.h"
#include "encryption_utils.h"
#include "account_manager.h" // Добавлено
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication a(argc, argv);

    const int maxAttempts = 3;
    int attempts = 0;

    while (attempts < maxAttempts) {
        MasterPasswordDialog passwordDialog;
        if (passwordDialog.exec() == QDialog::Accepted) {
            QString masterPassword = passwordDialog.getPassword();

            if (AccountManager::instance().verifyMasterPassword(masterPassword)) {
                EncryptionUtils::instance().setMasterPassword(masterPassword);

                OTPWindow window;
                window.show();

                QObject::connect(&a, &QApplication::aboutToQuit, []() {
                    AccountManager::instance().logEvent("Пользователь вышел из приложения");
                });

                return a.exec();
            } else {
                attempts++;
                QMessageBox::warning(nullptr, "Ошибка",
                                     QString("Неверный мастер-пароль. Попыток осталось: %1").arg(maxAttempts - attempts));
            }
        } else {
            return 0; // Выход из приложения
        }
    }

    QMessageBox::critical(nullptr, "Ошибка", "Превышено количество попыток ввода мастер-пароля. Приложение будет закрыто.");
    return 0;
}

