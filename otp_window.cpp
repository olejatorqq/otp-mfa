#include "otp_window.h"
#include "ui_otp_window.h"
#include "add_account_dialog.h"
#include "otp_generator.h"
#include "account_manager.h"
#include "settings_dialog.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QLineEdit>
#include <QDateTime>
#include <QScrollArea>
#include <QClipboard>
#include <QApplication>
#include <QProgressBar>
#include <QMouseEvent>
#include <QPalette>
#include <QStyle>
#include <QStyleFactory>
#include <QFont>
#include <QDebug>
#include <QScreen>
#include <QSettings>

OTPWindow::OTPWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OTPWindow),
    timer(new QTimer(this)),
    darkThemeEnabled(false)
{
    ui->setupUi(this);

    // Устанавливаем минимальный и начальный размер окна
    this->setMinimumSize(400, 600);
    this->resize(600, 800); // Начальный размер окна

    // Устанавливаем размер шрифта для заголовка
    QFont titleFont = ui->titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    ui->titleLabel->setFont(titleFont);

    // Устанавливаем размер шрифта для всего приложения
    QFont defaultFont = QApplication::font();
    defaultFont.setPointSize(12);
    QApplication::setFont(defaultFont);

    // Применяем начальную тему
    applyTheme();

    // Загружаем интервал из настроек или устанавливаем значение по умолчанию
    QSettings settings("YourCompany", "YourApp");
    interval = settings.value("interval", 30).toInt();

    // Получаем аккаунты из базы данных
    accounts = AccountManager::instance().getAccounts();
    displayAccounts();

    // Подключаем сигналы и слоты
    connect(ui->addButton, &QPushButton::clicked, this, &OTPWindow::onAddAccountClicked);
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &OTPWindow::filterAccounts);
    connect(ui->settingsButton, &QPushButton::clicked, this, &OTPWindow::openSettingsDialog); // Добавлено подключение
    connect(timer, &QTimer::timeout, this, &OTPWindow::updateAccounts);

    // Запускаем таймер для обновления OTP
    timer->start(1000);
}

OTPWindow::~OTPWindow()
{
    delete ui;
}

void OTPWindow::onAddAccountClicked() {
    AddAccountDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Account newAccount = dialog.getAccount();
        AccountManager::instance().addAccount(newAccount);

        // Обновляем список аккаунтов из базы данных
        accounts = AccountManager::instance().getAccounts();
        displayAccounts();
    }
}

void OTPWindow::displayAccounts() {
    // Очищаем предыдущие виджеты
    QLayoutItem *child;
    while ((child = ui->accountsLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    accountWidgets.clear();

    // Получаем коэффициент масштабирования DPI
    qreal scaleFactor = QGuiApplication::primaryScreen()->logicalDotsPerInch() / 96.0;

    // Получаем текущее время один раз
    quint64 currentTime = QDateTime::currentSecsSinceEpoch();

    for (Account &account : accounts) {
        QWidget *accountWidget = new QWidget();
        accountWidget->setObjectName("accountWidget");

        QString accountBackgroundColor = darkThemeEnabled ? "#3E3E3E" : "#FFFFFF";
        QString accountBorderColor = darkThemeEnabled ? "#555555" : "#cccccc";

        accountWidget->setStyleSheet(
            QString(
                "#accountWidget {"
                "   background-color: %1;"
                "   border: 1px solid %2;"
                "   border-radius: 8px;"
                "   padding: 8px;"
                "}"
                ).arg(accountBackgroundColor, accountBorderColor)
            );

        accountWidget->setToolTip("Дважды щёлкните, чтобы скопировать код");

        QHBoxLayout *layout = new QHBoxLayout(accountWidget);
        layout->setContentsMargins(10, 5, 10, 5);
        layout->setSpacing(10);

        // Имя аккаунта
        QLabel *nameLabel = new QLabel(account.name);
        QFont nameFont = nameLabel->font();
        nameFont.setPointSizeF(14 * scaleFactor);
        nameFont.setBold(true);
        nameLabel->setFont(nameFont);
        nameLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

        // OTP код
        OtpGenerator generator;
        QString otp;
        if (account.type == "TOTP") {
            otp = generator.generateTOTP(account.secret, currentTime, account.period, account.algorithm, account.digits);
        } else if (account.type == "HOTP") {
            otp = generator.generateHOTP(account.secret, account.counter, account.algorithm, account.digits);
        }
        QLabel *otpLabel = new QLabel(otp);
        QFont otpFont = otpLabel->font();
        otpFont.setPointSizeF(24 * scaleFactor);
        otpFont.setBold(true);
        otpLabel->setFont(otpFont);
        otpLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

        // Прогресс-бар для оставшегося времени
        QProgressBar *progressBar = new QProgressBar();
        progressBar->setTextVisible(false);
        progressBar->setFixedWidth(100 * scaleFactor);
        progressBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

        // Стилизация прогресс-бара
        QString inputBorderColor = darkThemeEnabled ? "#555555" : "#cccccc";
        QString backgroundColor = darkThemeEnabled ? "#2E2E2E" : "#f0f0f0";
        QString progressBarStyle = QString(
                                       "QProgressBar {"
                                       "   border: 1px solid %1;"
                                       "   border-radius: 5px;"
                                       "   background-color: %2;"
                                       "}"
                                       "QProgressBar::chunk {"
                                       "   background-color: %3;"
                                       "}"
                                       ).arg(inputBorderColor, backgroundColor, "#76C7C0");
        progressBar->setStyleSheet(progressBarStyle);

        if (account.type == "TOTP") {
            int timeLeft = account.period - (currentTime % account.period);
            progressBar->setMinimum(0);
            progressBar->setMaximum(account.period);
            progressBar->setValue(timeLeft);
        } else {
            // Для HOTP скрываем прогресс-бар
            progressBar->setVisible(false);
        }

        // Размещение элементов в layout
        layout->addWidget(nameLabel, 2);
        layout->addWidget(otpLabel, 1);
        layout->addWidget(progressBar, 0);

        // Добавляем виджет аккаунта в основной layout
        ui->accountsLayout->addWidget(accountWidget);

        // Добавляем разделитель
        QFrame *separator = new QFrame();
        separator->setFrameShape(QFrame::HLine);
        separator->setFrameShadow(QFrame::Sunken);
        ui->accountsLayout->addWidget(separator);

        // Сохраняем виджет для обновления
        accountWidgets.append(accountWidget);

        // Устанавливаем фильтр событий для двойного щелчка
        accountWidget->installEventFilter(this);
    }

    // Добавляем растяжку в конец для выравнивания вверх
    ui->accountsLayout->addStretch();

    filterAccounts(ui->searchLineEdit->text());
}





void OTPWindow::filterAccounts(const QString &filter) {
    for (int i = 0; i < accounts.size(); ++i) {
        const Account &account = accounts[i];
        QWidget *accountWidget = accountWidgets[i];
        bool matches = account.name.contains(filter, Qt::CaseInsensitive);
        accountWidget->setVisible(matches);
    }
}

void OTPWindow::updateAccounts() {
    OtpGenerator generator;

    // Получаем текущее время один раз
    quint64 currentTime = QDateTime::currentSecsSinceEpoch();

    for (int i = 0; i < accounts.size(); ++i) {
        Account &account = accounts[i];
        QWidget *accountWidget = accountWidgets[i];

        if (!accountWidget) continue;

        QHBoxLayout *layout = qobject_cast<QHBoxLayout*>(accountWidget->layout());
        if (!layout) continue;

        QLabel *nameLabel = qobject_cast<QLabel*>(layout->itemAt(0)->widget());
        QLabel *otpLabel = qobject_cast<QLabel*>(layout->itemAt(1)->widget());
        QProgressBar *progressBar = qobject_cast<QProgressBar*>(layout->itemAt(2)->widget());

        if (!otpLabel || !progressBar) continue;

        QString otp;
        if (account.type == "TOTP") {
            otp = generator.generateTOTP(account.secret, currentTime, account.period, account.algorithm, account.digits);
        } else if (account.type == "HOTP") {
            otp = generator.generateHOTP(account.secret, account.counter, account.algorithm, account.digits);
            // Увеличиваем счетчик для HOTP
            account.counter += 1;
        }

        otpLabel->setText(otp);

        if (account.type == "TOTP") {
            int timeLeft = account.period - (currentTime % account.period);
            progressBar->setMaximum(account.period);
            progressBar->setValue(timeLeft);

            // Изменение цвета прогресс-бара в зависимости от оставшегося времени
            if (timeLeft <= 5) {
                progressBar->setStyleSheet(progressBar->styleSheet().replace("#76C7C0", "#FF6B6B"));
            } else {
                progressBar->setStyleSheet(progressBar->styleSheet().replace("#FF6B6B", "#76C7C0"));
            }
        } else {
            // Для HOTP скрываем прогресс-бар
            progressBar->setVisible(false);
        }
    }
}




bool OTPWindow::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::MouseButtonDblClick) {
        for (int i = 0; i < accountWidgets.size(); ++i) {
            if (watched == accountWidgets[i]) {
                copyCodeToClipboard(i);
                return true;
            }
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void OTPWindow::copyCodeToClipboard(int index) {
    if (index < 0 || index >= accounts.size()) return;

    OtpGenerator generator;
    quint64 currentTime = QDateTime::currentSecsSinceEpoch(); // Получаем текущее время
    QString otp = generator.generateTOTP(accounts[index].secret, currentTime, interval); // Передаём currentTime

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(otp);

    // Отображение временного сообщения
    QLabel *copiedLabel = new QLabel("Скопировано в буфер обмена");
    copiedLabel->setStyleSheet("background-color: rgba(0, 0, 0, 0.7); color: white; padding: 10px; border-radius: 5px; font-size: 14px;");
    copiedLabel->setAlignment(Qt::AlignCenter);
    copiedLabel->setWindowFlags(Qt::ToolTip);
    copiedLabel->setAttribute(Qt::WA_DeleteOnClose);
    QPoint pos = QCursor::pos();
    copiedLabel->move(pos.x() - copiedLabel->width()/2, pos.y() - copiedLabel->height() - 10);
    copiedLabel->show();
    QTimer::singleShot(1000, copiedLabel, &QLabel::close);
}

void OTPWindow::toggleTheme() {
    darkThemeEnabled = !darkThemeEnabled;
    applyTheme();
}

void OTPWindow::applyTheme() {
    // Стили для светлой и темной тем
    QString buttonColor = darkThemeEnabled ? "#2E2E2E" : "#007BFF";
    QString buttonHoverColor = darkThemeEnabled ? "#444444" : "#0056b3";
    QString textColor = darkThemeEnabled ? "white" : "black"; // Изменено здесь
    QString inputBorderColor = darkThemeEnabled ? "#555555" : "#cccccc";
    QString backgroundColor = darkThemeEnabled ? "#2E2E2E" : "#f0f0f0";
    QString textInputColor = darkThemeEnabled ? "#ffffff" : "#000000";

    // Устанавливаем палитру для основного окна
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(backgroundColor));
    palette.setColor(QPalette::WindowText, QColor(textColor));
    this->setPalette(palette);

    // Стили для кнопки "Добавить"
    QString buttonStyle = QString(
                              "QPushButton {"
                              "   background-color: %1;"
                              "   color: white;" // Цвет текста кнопки оставляем белым для контраста
                              "   border: none;"
                              "   border-radius: 8px;"
                              "   padding: 12px;"
                              "   font-size: 16px;"
                              "}"
                              "QPushButton:hover {"
                              "   background-color: %2;"
                              "}"
                              ).arg(buttonColor, buttonHoverColor);

    ui->addButton->setStyleSheet(buttonStyle);

    // Стили для поля поиска
    QString searchStyle = QString(
                              "QLineEdit {"
                              "   padding: 8px;"
                              "   border: 1px solid %1;"
                              "   border-radius: 8px;"
                              "   font-size: 14px;"
                              "   color: %2;"
                              "   background-color: %3;"
                              "}"
                              ).arg(inputBorderColor, textInputColor, backgroundColor);

    ui->searchLineEdit->setStyleSheet(searchStyle);

    // Стили для заголовка
    QString titleStyle = QString(
                             "QLabel {"
                             "   color: %1;"
                             "}"
                             ).arg(textColor);

    ui->titleLabel->setStyleSheet(titleStyle);

    // Стили для кнопки настроек
    QString settingsButtonStyle = QString(
                                      "QPushButton {"
                                      "   background-color: transparent;"
                                      "   color: %1;"
                                      "   border: none;"
                                      "   font-size: 18px;"
                                      "}"
                                      "QPushButton:hover {"
                                      "   color: %2;"
                                      "}"
                                      ).arg(textColor, buttonHoverColor);

    ui->settingsButton->setStyleSheet(settingsButtonStyle);

    // Дополнительно: устанавливаем цвет текста для всех QLabel в приложении
    QPalette labelPalette;
    labelPalette.setColor(QPalette::WindowText, QColor(textColor));
    QList<QLabel*> labels = this->findChildren<QLabel*>();
    for (QLabel* label : labels) {
        label->setPalette(labelPalette);
    }
}


void OTPWindow::openSettingsDialog()
{
    SettingsDialog dialog(this);
    dialog.setInterval(interval);

    if (dialog.exec() == QDialog::Accepted) {
        interval = dialog.getInterval();

        // Сохраняем значение в настройках
        QSettings settings("YourCompany", "YourApp");
        settings.setValue("interval", interval);

        // Обновляем отображение
        displayAccounts();
    }
}
