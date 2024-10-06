#include "otp_window.h"
#include "./ui_otp_window.h"

otp_window::otp_window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::otp_window)
{
    ui->setupUi(this);
}

otp_window::~otp_window()
{
    delete ui;
}

