#include "otp_window.h"
// #include "encryption_utils.h"
#include <QApplication>
#include <QMessageBox>
#include <QSettings> // Добавьте, если используются настройки

int main(int argc, char *argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication a(argc, argv);

    // Если вы используете EncryptionUtils и AccountManager без мастер-пароля,
    // убедитесь, что они инициализируются корректно.
    // Например:
    // EncryptionUtils::instance().initialize(); // Ваш метод инициализации

    OTPWindow window;
    window.show();

    // Подключаемся к сигналу aboutToQuit для логирования выхода
    QObject::connect(&a, &QApplication::aboutToQuit, []() {
        AccountManager::instance().logEvent("Пользователь вышел из приложения");
    });

    return a.exec();
}
