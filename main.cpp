#include <QApplication>
#include "otp_window.h"

int main(int argc, char *argv[]) {
    // Устанавливаем атрибуты до создания QApplication
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication a(argc, argv);
    OTPWindow window;
    window.show();
    return a.exec();
}
