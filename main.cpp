#include <QApplication>
#include "otp_window.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    OtpWindow window;
    window.show();
    return app.exec();
}
