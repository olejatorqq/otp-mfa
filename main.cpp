#include "otp_window.h"
#include "master_password_dialog.h"
#include "encryption_utils.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication a(argc, argv);

    while (true) {
        MasterPasswordDialog passwordDialog;
        if (passwordDialog.exec() == QDialog::Accepted) {
            QString masterPassword = passwordDialog.getPassword();
            EncryptionUtils::instance().setMasterPassword(masterPassword);

            if (AccountManager::instance().verifyMasterPassword()) {
                OTPWindow window;
                window.show();

                // Подключаемся к сигналу aboutToQuit для логирования выхода
                QObject::connect(&a, &QApplication::aboutToQuit, []() {
                    AccountManager::instance().logEvent("Пользователь вышел из приложения");
                });

                return a.exec();
            } else {
                QMessageBox::warning(nullptr, "Ошибка", "Неверный мастер-пароль. Попробуйте снова.");
            }
        } else {
            return 0; // Выход из приложения, если пользователь отменил ввод пароля
        }
    }
}
