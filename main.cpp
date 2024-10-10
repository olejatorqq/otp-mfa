#include <QApplication>
#include "otp_window.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    OTPWindow window;
    window.show();

    return a.exec();
}
