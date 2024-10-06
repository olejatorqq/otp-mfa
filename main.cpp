#include "otp_window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    otp_window w;
    w.show();
    return a.exec();
}
