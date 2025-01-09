#include "otp_window.h"
#include "ui_otp_window.h"

#include "add_account_dialog.h"
#include "otp_generator.h"
#include "account_manager.h"
#include "settings_dialog.h"

// ====== Новые include ======
#include "screenshot_tool.h"
#include "qr_generator.h"

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
#include <QMenu>
#include <QMessageBox>
#include <QContextMenuEvent>
#include <QUrl>
#include <QUrlQuery>     // Для парсинга otpauth:// URI

#include <ReadBarcode.h>
#include <BarcodeFormat.h>
#include <ReaderOptions.h>

OTPWindow::OTPWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OTPWindow),
    timer(new QTimer(this)),
    darkThemeEnabled(false),
    // ====== Инициализация нового поля ======
    qrGenerator(new QRGenerator(this))
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
    connect(timer, &QTimer::timeout, this, &OTPWindow::updateAccounts);

    // Подключаемся к сигналам AccountManager для обработки сетевых запросов
    connect(&AccountManager::instance(), &AccountManager::dataFetched, this, &OTPWindow::onDataFetched);
    connect(&AccountManager::instance(), &AccountManager::fetchError, this, &OTPWindow::onFetchError);

    // ====== Подключаем сигналы QRGenerator ======
    connect(qrGenerator, &QRGenerator::qrDecoded, this, &OTPWindow::handleQrDecoded);
    connect(qrGenerator, &QRGenerator::qrError,   this, &OTPWindow::handleQrError);

    // Пример вызова HTTPS-запроса (можете разместить это в нужном месте)
    // QUrl url("https://localhost:4443/api/data"); // Замените на ваш URL
    // AccountManager::instance().fetchDataFromServer(url);

    // Запускаем таймер для обновления OTP
    timer->start(1000);
}

OTPWindow::~OTPWindow()
{
    delete ui;
}

// ====== Слот для нажатия кнопки "Добавить новый аккаунт" ======
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

    for (int i = 0; i < accounts.size(); ++i) {
        Account &account = accounts[i];
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

        // OTP-код
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

        // Прогресс-бар для TOTP
        QProgressBar *progressBar = new QProgressBar();
        progressBar->setTextVisible(false);
        progressBar->setFixedWidth(100 * scaleFactor);
        progressBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

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
            progressBar->setVisible(false);
        }

        layout->addWidget(nameLabel, 2);
        layout->addWidget(otpLabel, 1);
        layout->addWidget(progressBar, 0);

        ui->accountsLayout->addWidget(accountWidget);

        QFrame *separator = new QFrame();
        separator->setFrameShape(QFrame::HLine);
        separator->setFrameShadow(QFrame::Sunken);
        ui->accountsLayout->addWidget(separator);

        accountWidgets.append(accountWidget);
        accountWidget->installEventFilter(this);
    }

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
            int timeLeft = account.period - (currentTime % account.period);
            progressBar->setMaximum(account.period);
            progressBar->setValue(timeLeft);

            QString currentStyle = progressBar->styleSheet();
            if (timeLeft <= 5) {
                progressBar->setStyleSheet(currentStyle.replace("#76C7C0", "#FF6B6B"));
            } else {
                progressBar->setStyleSheet(currentStyle.replace("#FF6B6B", "#76C7C0"));
            }
        } else if (account.type == "HOTP") {
            // HOTP не обновляется по таймеру
            continue;
        }

        otpLabel->setText(otp);
    }
}

bool OTPWindow::eventFilter(QObject *watched, QEvent *event) {
    for (int i = 0; i < accountWidgets.size(); ++i) {
        if (watched == accountWidgets[i]) {
            if (event->type() == QEvent::MouseButtonDblClick) {
                copyCodeToClipboard(i);
                return true;
            } else if (event->type() == QEvent::ContextMenu) {
                QContextMenuEvent *contextEvent = static_cast<QContextMenuEvent*>(event);
                showContextMenuForAccount(i, contextEvent->globalPos());
                return true;
            }
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void OTPWindow::copyCodeToClipboard(int index) {
    if (index < 0 || index >= accounts.size()) return;

    Account &account = accounts[index];
    OtpGenerator generator;
    quint64 currentTime = QDateTime::currentSecsSinceEpoch();
    QString otp;

    if (account.type == "TOTP") {
        otp = generator.generateTOTP(account.secret, currentTime, account.period, account.algorithm, account.digits);
    } else if (account.type == "HOTP") {
        otp = generator.generateHOTP(account.secret, account.counter, account.algorithm, account.digits);
        // Увеличиваем счётчик только при использовании HOTP
        account.counter += 1;
        AccountManager::instance().updateAccount(account.name, account);
    }

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(otp);

    QLabel *copiedLabel = new QLabel("Скопировано в буфер обмена");
    copiedLabel->setStyleSheet("background-color: rgba(0, 0, 0, 0.7);"
                               "color: white; padding: 10px; border-radius: 5px;"
                               "font-size: 14px;");
    copiedLabel->setAlignment(Qt::AlignCenter);
    copiedLabel->setWindowFlags(Qt::ToolTip);
    copiedLabel->setAttribute(Qt::WA_DeleteOnClose);
    QPoint pos = QCursor::pos();
    copiedLabel->move(pos.x() - copiedLabel->width()/2, pos.y() - copiedLabel->height() - 10);
    copiedLabel->show();
    QTimer::singleShot(1000, copiedLabel, &QLabel::close);
}

// ====== Слот для переключения светлой/тёмной темы ======
void OTPWindow::toggleTheme() {
    darkThemeEnabled = !darkThemeEnabled;
    applyTheme();
}

void OTPWindow::applyTheme() {
    QString buttonColor = darkThemeEnabled ? "#2E2E2E" : "#007BFF";
    QString buttonHoverColor = darkThemeEnabled ? "#444444" : "#0056b3";
    QString textColor = darkThemeEnabled ? "white" : "black";
    QString inputBorderColor = darkThemeEnabled ? "#555555" : "#cccccc";
    QString backgroundColor = darkThemeEnabled ? "#2E2E2E" : "#f0f0f0";
    QString textInputColor = darkThemeEnabled ? "#ffffff" : "#000000";

    QPalette palette;
    palette.setColor(QPalette::Window, QColor(backgroundColor));
    palette.setColor(QPalette::WindowText, QColor(textColor));
    this->setPalette(palette);

    QString buttonStyle = QString(
                              "QPushButton {"
                              "   background-color: %1;"
                              "   color: white;"
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

    QString titleStyle = QString(
                             "QLabel {"
                             "   color: %1;"
                             "}"
                             ).arg(textColor);

    ui->titleLabel->setStyleSheet(titleStyle);

    // Применяем новый цвет текста ко всем QLabel
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

        QSettings settings("YourCompany", "YourApp");
        settings.setValue("interval", interval);

        displayAccounts();
    }
}

void OTPWindow::showContextMenuForAccount(int index, const QPoint &globalPos) {
    QMenu contextMenu;

    QAction *editAction = new QAction("Редактировать", &contextMenu);
    QAction *deleteAction = new QAction("Удалить", &contextMenu);

    contextMenu.addAction(editAction);
    contextMenu.addAction(deleteAction);

    connect(editAction, &QAction::triggered, [this, index]() {
        editAccount(index);
    });
    connect(deleteAction, &QAction::triggered, [this, index]() {
        deleteAccount(index);
    });

    contextMenu.exec(globalPos);
}

void OTPWindow::editAccount(int index) {
    if (index < 0 || index >= accounts.size()) return;

    Account &account = accounts[index];

    AddAccountDialog dialog(this);
    dialog.setWindowTitle("Редактировать аккаунт");
    dialog.setAccount(account);

    if (dialog.exec() == QDialog::Accepted) {
        Account updatedAccount = dialog.getAccount();
        AccountManager::instance().updateAccount(account.name, updatedAccount);

        accounts = AccountManager::instance().getAccounts();
        displayAccounts();
    }
}

void OTPWindow::deleteAccount(int index) {
    if (index < 0 || index >= accounts.size()) return;

    Account &account = accounts[index];

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Удаление аккаунта",
                                  "Вы действительно хотите удалить этот аккаунт?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        AccountManager::instance().deleteAccount(account.name);
        accounts.removeAt(index);
        QWidget *accountWidget = accountWidgets.takeAt(index);
        delete accountWidget;
        displayAccounts();
    }
}

// ====== Обработка ответов от сервера ======
void OTPWindow::onDataFetched(const QByteArray &data) {
    qDebug() << "Получены данные с сервера:" << data;
    QMessageBox::information(this, "Данные получены", "Успешно получили данные с сервера.");
}

void OTPWindow::onFetchError(const QString &errorString) {
    qWarning() << "Ошибка при получении данных с сервера:" << errorString;
    QMessageBox::critical(this, "Ошибка", "Не удалось получить данные с сервера: " + errorString);
}

// ====== Слот для кнопки "Сканировать QR" ======
void OTPWindow::on_scanQrButton_clicked()
{
    // Создаём окно выбора области экрана
    ScreenshotTool *tool = new ScreenshotTool();
    tool->setAttribute(Qt::WA_DeleteOnClose);

    // Когда пользователь закончит выделение,
    // сигнал areaSelected вернёт QPixmap выделенной области
    connect(tool, &ScreenshotTool::areaSelected, qrGenerator, &QRGenerator::processPixmap);
}

// ====== Обработка успешного декодирования ======
void OTPWindow::handleQrDecoded(const QString &decodedText)
{
    // Проверим, действительно ли это otpauth://
    if (!decodedText.startsWith("otpauth://", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, "Ошибка", "Сканированный QR-код не содержит otpauth:// URI.");
        return;
    }

    // Парсим URI
    QUrl url(decodedText);
    if (!url.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Некорректный формат QR-кода.");
        return;
    }

    QString path = url.path(); // Может быть /SomeIssuer:Account
    QStringList pathParts = path.mid(1).split(":"); // Убираем '/'
    if (pathParts.size() < 2) {
        QMessageBox::warning(this, "Ошибка", "Некорректный формат пути в QR-коде.");
        return;
    }
    QString issuer = pathParts[0].trimmed();
    QString accountName = pathParts[1].trimmed();

    QUrlQuery query(url);
    QString secret = query.queryItemValue("secret");
    QString algorithm = query.queryItemValue("algorithm").isEmpty() ? "SHA1" : query.queryItemValue("algorithm");
    int digits = query.queryItemValue("digits").isEmpty() ? 6 : query.queryItemValue("digits").toInt();
    int period = query.queryItemValue("period").isEmpty() ? 30 : query.queryItemValue("period").toInt();

    if (secret.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось найти secret в QR-коде.");
        return;
    }

    // Создаём аккаунт
    Account newAccount;
    newAccount.name      = accountName;
    newAccount.secret    = secret;
    newAccount.algorithm = algorithm.toUpper(); // Часто в otpauth указывается uppercase
    newAccount.digits    = digits;
    newAccount.period    = period;
    newAccount.type      = "TOTP";   // Предполагаем TOTP
    newAccount.counter   = 0;        // Для HOTP не используется

    // Добавляем аккаунт
    AccountManager::instance().addAccount(newAccount);

    // Обновляем список на экране
    accounts = AccountManager::instance().getAccounts();
    displayAccounts();

    QMessageBox::information(this, "Успех", "Аккаунт из QR-кода успешно добавлен.");
}

// ====== Обработка ошибки при декодировании ======
void OTPWindow::handleQrError(const QString &errorString)
{
    QMessageBox::warning(this, "Ошибка при декодировании QR", errorString);
}
