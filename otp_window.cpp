#include "otp_window.h"
#include "ui_otp_window.h"
#include "add_account_dialog.h"
#include "otp_generator.h"
#include "account_manager.h"
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
#include <QDebug> // Добавлено для отладки

OTPWindow::OTPWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OTPWindow),
    timer(new QTimer(this)),
    darkThemeEnabled(false)
{
    ui->setupUi(this);

    // Устанавливаем размер шрифта для всего приложения
    QFont defaultFont = QApplication::font();
    defaultFont.setPointSize(12);
    QApplication::setFont(defaultFont);

    // Применяем начальную тему
    applyTheme();

    // Получаем аккаунты из базы данных
    accounts = AccountManager::instance().getAccounts();
    displayAccounts();

    // Подключаем сигналы и слоты
    connect(ui->addButton, &QPushButton::clicked, this, &OTPWindow::onAddAccountClicked);
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &OTPWindow::filterAccounts);
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

    for (const Account& account : accounts) {
        QWidget *accountWidget = new QWidget();
        accountWidget->setObjectName("accountWidget");
        accountWidget->setStyleSheet("#accountWidget { background-color: transparent; border-radius: 8px; padding: 16px; }");
        QHBoxLayout *layout = new QHBoxLayout(accountWidget);

        // Имя аккаунта
        QLabel *nameLabel = new QLabel(account.name);
        QFont nameFont = nameLabel->font();
        nameFont.setPointSize(16); // Увеличено с 14
        nameFont.setBold(true);
        nameLabel->setFont(nameFont);

        // OTP код
        OtpGenerator generator;
        QString otp = generator.generateTOTP(account.secret);
        QLabel *otpLabel = new QLabel(otp);
        QFont otpFont = otpLabel->font();
        otpFont.setPointSize(28); // Увеличено с 24
        otpFont.setBold(true);
        otpLabel->setFont(otpFont);

        // Прогресс-бар для оставшегося времени
        QProgressBar *progressBar = new QProgressBar();
        progressBar->setMinimum(0);
        progressBar->setMaximum(30);
        int timeLeft = 30 - (QDateTime::currentSecsSinceEpoch() % 30);
        progressBar->setValue(timeLeft);
        progressBar->setTextVisible(false);
        progressBar->setFixedWidth(120); // Увеличена ширина

        // Стилизация прогресс-бара
        progressBar->setStyleSheet(R"(
            QProgressBar {
                border: 1px solid #CCCCCC;
                border-radius: 5px;
                background-color: #E0E0E0;
            }
            QProgressBar::chunk {
                background-color: #76C7C0;
            }
        )");

        // Размещение элементов в layout
        layout->addWidget(nameLabel);
        layout->addStretch();
        layout->addWidget(otpLabel);
        layout->addWidget(progressBar);

        // Добавляем виджет аккаунта в основной layout
        ui->accountsLayout->addWidget(accountWidget);

        // Сохраняем виджет для обновления
        accountWidgets.append(accountWidget);

        // Устанавливаем фильтр событий для двойного щелчка
        accountWidget->installEventFilter(this);
    }

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

    for (int i = 0; i < accounts.size(); ++i) {
        const Account &account = accounts[i];
        QWidget *accountWidget = accountWidgets[i];

        QHBoxLayout *layout = qobject_cast<QHBoxLayout*>(accountWidget->layout());
        if (!layout) continue;

        QLabel *nameLabel = qobject_cast<QLabel*>(layout->itemAt(0)->widget());
        QLabel *otpLabel = qobject_cast<QLabel*>(layout->itemAt(2)->widget());
        QProgressBar *progressBar = qobject_cast<QProgressBar*>(layout->itemAt(3)->widget());

        if (!nameLabel || !otpLabel || !progressBar) continue;

        QString otp = generator.generateTOTP(account.secret);
        otpLabel->setText(otp);

        int timeLeft = 30 - (QDateTime::currentSecsSinceEpoch() % 30);
        progressBar->setValue(timeLeft);

        // Изменение цвета прогресс-бара в зависимости от оставшегося времени
        if (timeLeft <= 5) {
            progressBar->setStyleSheet(R"(
                QProgressBar {
                    border: 1px solid #CCCCCC;
                    border-radius: 5px;
                    background-color: #E0E0E0;
                }
                QProgressBar::chunk {
                    background-color: #FF6B6B;
                }
            )");
        } else {
            progressBar->setStyleSheet(R"(
                QProgressBar {
                    border: 1px solid #CCCCCC;
                    border-radius: 5px;
                    background-color: #E0E0E0;
                }
                QProgressBar::chunk {
                    background-color: #76C7C0;
                }
            )");
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
    QString otp = generator.generateTOTP(accounts[index].secret);

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
    QPalette palette;

    if (darkThemeEnabled) {
        qApp->setStyle(QStyleFactory::create("Fusion"));
        palette.setColor(QPalette::Window, QColor(53,53,53));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Base, QColor(25,25,25));
        palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
        palette.setColor(QPalette::ToolTipBase, Qt::white);
        palette.setColor(QPalette::ToolTipText, Qt::white);
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Button, QColor(53,53,53));
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::BrightText, Qt::red);
        palette.setColor(QPalette::Highlight, QColor(142,45,197).lighter());
        palette.setColor(QPalette::HighlightedText, Qt::black);
    } else {
        qApp->setStyle(QStyleFactory::create("Fusion"));
        palette = qApp->style()->standardPalette();
    }

    qApp->setPalette(palette);

    // Обновляем стили для кнопок и других элементов
    QString buttonStyle = darkThemeEnabled ?
                              "QPushButton { background-color: #5A5A5A; color: white; border-radius: 8px; padding: 12px; font-size: 18px; }"
                              "QPushButton:hover { background-color: #777777; }"
                                           :
                              "QPushButton { background-color: #007AFF; color: white; border-radius: 8px; padding: 12px; font-size: 18px; }"
                              "QPushButton:hover { background-color: #005BB5; }";

    ui->addButton->setStyleSheet(buttonStyle);
    ui->searchLineEdit->setStyleSheet("QLineEdit { padding: 8px; border-radius: 8px; font-size: 16px; }");
}
