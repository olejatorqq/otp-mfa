#include "otp_window.h"
#include "ui_otp_window.h"
#include "add_account_dialog.h"
#include "otp_generator.h"
#include "account_manager.h"
#include "settings_dialog.h"
#include <QLabel>
<<<<<<< HEAD
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
=======
#include <QPixmap>
#include <QTableWidgetItem>
#include <QInputDialog>
#include <QMessageBox>
#include <QTime>
>>>>>>> 9aa011e288283510cac46a946f74ac09ede64cef

OTPWindow::OTPWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OTPWindow),
    timer(new QTimer(this)),
    darkThemeEnabled(false)
{
    ui->setupUi(this);

<<<<<<< HEAD
    // Устанавливаем минимальный и начальный размер окна
    this->setMinimumSize(400, 600);
    this->resize(600, 800); // Начальный размер окна

    QFont titleFont = ui->titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    ui->titleLabel->setFont(titleFont);

    // Устанавливаем размер шрифта для всего приложения
    QFont defaultFont = QApplication::font();
    defaultFont.setPointSize(12);
    QApplication::setFont(defaultFont);

    applyTheme();

    // Загружаем интервал из настроек или устанавливаем значение по умолчанию
    QSettings settings("olejatorqq", "app_olejatorqq");
    interval = settings.value("interval", 30).toInt();

    // Получаем аккаунты из базы данных
    accounts = AccountManager::instance().getAccounts();
    displayAccounts();
=======
    // Запрос пароля при запуске приложения
    bool ok;
    QString password = QInputDialog::getText(this, tr("Введите пароль"),
                                             tr("Пароль:"), QLineEdit::Password,
                                             "", &ok);
    if (ok && !password.isEmpty()) {
        accountManager.setPassword(password);
        accounts = accountManager.getAccounts();
        displayAccounts();
    } else {
        QMessageBox::critical(this, tr("Ошибка"), tr("Пароль обязателен для входа."));
        close();  // Закрываем приложение, если пароль не введён
    }
>>>>>>> 9aa011e288283510cac46a946f74ac09ede64cef

    // Подключаем сигналы и слоты
    connect(ui->addButton, &QPushButton::clicked, this, &OTPWindow::onAddAccountClicked);
<<<<<<< HEAD
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &OTPWindow::filterAccounts);
    connect(timer, &QTimer::timeout, this, &OTPWindow::updateAccounts);

    // Подключаемся к сигналам AccountManager для обработки сетевых запросов
    connect(&AccountManager::instance(), &AccountManager::dataFetched, this, &OTPWindow::onDataFetched);
    connect(&AccountManager::instance(), &AccountManager::fetchError, this, &OTPWindow::onFetchError);

    // Пример вызова HTTPS-запроса (можете разместить это в нужном месте)
    // QUrl url("https://localhost:4443/api/data");
    // AccountManager::instance().fetchDataFromServer(url);

    // Запускаем таймер для обновления OTP
    timer->start(1000);
=======
    connect(ui->accountsTableWidget, &QTableWidget::cellDoubleClicked, this, &OTPWindow::onAccountDoubleClicked);

    // Таймер для обновления OTP и QR-кода каждые 5 секунд
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &OTPWindow::updateAccountsAndQRCode);
    timer->start(5000);  // Обновляем OTP и QR-код каждые 5 секунд

    // Таймер для плавного обновления прогресс-бара каждую секунду
    QTimer *progressBarTimer = new QTimer(this);
    connect(progressBarTimer, &QTimer::timeout, this, &OTPWindow::updateProgressBar);
    progressBarTimer->start(1000);  // Обновляем прогресс-бар каждую секунду

    // Подключение переключателя темы
    connect(ui->themeToggleButton, &QPushButton::clicked, this, &OTPWindow::toggleTheme);
>>>>>>> 9aa011e288283510cac46a946f74ac09ede64cef
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

        // OTP код
        OtpGenerator generator;
<<<<<<< HEAD
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
            progressBar->setVisible(false);
=======

        // Расшифровываем секретный ключ перед генерацией OTP
        QString decryptedSecret = accountManager.decryptSecret(QByteArray::fromHex(account.secret.toUtf8()));

        if (account.algorithm == "TOTP") {
            otp = generator.generateTOTP(decryptedSecret);
        } else {
            otp = generator.generateHOTP(decryptedSecret, account.counter);
>>>>>>> 9aa011e288283510cac46a946f74ac09ede64cef
        }

        // Размещение элементов в layout
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

<<<<<<< HEAD
void OTPWindow::updateAccounts() {
=======
void OTPWindow::onAccountDoubleClicked(int row, int column) {
    selectedAccountIndex = row;  // Устанавливаем индекс выбранного аккаунта
    generateQRCodeForSelectedAccount();  // Генерируем QR-код для выбранного аккаунта
}

void OTPWindow::updateAccountsAndQRCode() {
    displayAccounts();

    // Если выбран аккаунт, обновляем QR-код для него
    if (selectedAccountIndex >= 0 && selectedAccountIndex < accounts.size()) {
        generateQRCodeForSelectedAccount();
    }

    // Обновляем прогресс-бар
    updateProgressBar();
}

void OTPWindow::generateQRCodeForSelectedAccount() {
    // Получаем данные аккаунта
    Account selectedAccount = accounts[selectedAccountIndex];

    // Расшифровываем секретный ключ перед генерацией OTP
    QString decryptedSecret = accountManager.decryptSecret(QByteArray::fromHex(selectedAccount.secret.toUtf8()));

    // Генерируем OTP для выбранного аккаунта
>>>>>>> 9aa011e288283510cac46a946f74ac09ede64cef
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
            // Обновляем прогресс-бар
            int timeLeft = account.period - (currentTime % account.period);
            progressBar->setMaximum(account.period);
            progressBar->setValue(timeLeft);

            // Изменение цвета прогресс-бара в зависимости от оставшегося времени
            QString currentStyle = progressBar->styleSheet();
            if (timeLeft <= 5) {
                progressBar->setStyleSheet(currentStyle.replace("#76C7C0", "#FF6B6B"));
            } else {
                progressBar->setStyleSheet(currentStyle.replace("#FF6B6B", "#76C7C0"));
            }
        }
        // Для HOTP пропускаем обновление
        else if (account.type == "HOTP") {
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
<<<<<<< HEAD

    if (account.type == "TOTP") {
        otp = generator.generateTOTP(account.secret, currentTime, account.period, account.algorithm, account.digits);
    } else if (account.type == "HOTP") {
        otp = generator.generateHOTP(account.secret, account.counter, account.algorithm, account.digits);
        // Увеличиваем счётчик только при использовании HOTP
        account.counter += 1;
        AccountManager::instance().updateAccount(account.name, account);
=======
    if (selectedAccount.algorithm == "TOTP") {
        otp = generator.generateTOTP(decryptedSecret);
    } else {
        otp = generator.generateHOTP(decryptedSecret, selectedAccount.counter);
>>>>>>> 9aa011e288283510cac46a946f74ac09ede64cef
    }

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(otp);

    // Отображение временного сообщения
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

<<<<<<< HEAD
void OTPWindow::toggleTheme() {
    darkThemeEnabled = !darkThemeEnabled;
    applyTheme();
}

void OTPWindow::applyTheme() {
    // Стили для светлой и темной тем
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

    // Стили для кнопки "Добавить"
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

    // Устанавливаем цвет текста для всех QLabel в приложении
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

    // Заполняем диалог текущими данными аккаунта
    dialog.setAccount(account);

    if (dialog.exec() == QDialog::Accepted) {
        // Получаем обновлённый аккаунт из диалога
        Account updatedAccount = dialog.getAccount();

        // Сохраняем изменения в менеджере аккаунтов
        AccountManager::instance().updateAccount(account.name, updatedAccount);

        // Обновляем список аккаунтов из базы данных
        accounts = AccountManager::instance().getAccounts();

        // Обновляем интерфейс
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
        // Удаляем аккаунт из менеджера аккаунтов
        AccountManager::instance().deleteAccount(account.name);

        // Удаляем аккаунт из списка и виджетов
        accounts.removeAt(index);
        QWidget *accountWidget = accountWidgets.takeAt(index);
        delete accountWidget;

        // Обновляем интерфейс
        displayAccounts();
    }
}

// Новые слоты для обработки сетевых запросов
void OTPWindow::onDataFetched(const QByteArray &data) {
    // Обработка полученных данных
    qDebug() << "Получены данные с сервера:" << data;
    QMessageBox::information(this, "Данные получены", "Успешно получили данные с сервера.");
}

void OTPWindow::onFetchError(const QString &errorString) {
    // Обработка ошибок
    qWarning() << "Ошибка при получении данных с сервера:" << errorString;
    QMessageBox::critical(this, "Ошибка", "Не удалось получить данные с сервера: " + errorString);
=======
void OTPWindow::updateProgressBar() {
    // Обновляем прогресс-бар плавно
    int secondsPassed = QTime::currentTime().second() % 30;
    int percentage = (secondsPassed * 100) / 30;  // Обновляем на основе оставшегося времени для обновления TOTP
    ui->totpProgressBar->setValue(percentage);
}

void OTPWindow::toggleTheme() {
    static bool isDarkTheme = false;
    if (isDarkTheme) {
        // Применить светлую тему
        qApp->setStyleSheet("");
    } else {
        // Применить темную тему
        qApp->setStyleSheet("QWidget { background-color: #2e2e2e; color: #ffffff; } "
                            "QPushButton { background-color: #444444; color: #ffffff; } "
                            "QLineEdit, QTableWidget, QComboBox { background-color: #3e3e3e; color: #ffffff; }");
    }
    isDarkTheme = !isDarkTheme;
>>>>>>> 9aa011e288283510cac46a946f74ac09ede64cef
}
