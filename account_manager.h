#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include <QObject>
#include <QList>
#include <QNetworkAccessManager>
#include <QSqlDatabase>
#include <QUrl>

// Структура, описывающая аккаунт
struct Account {
    QString name;
    QString secret;
    QString algorithm;
    int digits;
    int period;
    QString type;     // "TOTP" или "HOTP"
    quint64 counter;  // Для HOTP
};

class AccountManager : public QObject
{
    Q_OBJECT

public:
    // Получение единственного экземпляра AccountManager (Singleton)
    static AccountManager& instance();

    // Запрещаем копирование и присваивание
    AccountManager(AccountManager const&) = delete;
    void operator=(AccountManager const&) = delete;

    // Методы работы с аккаунтами
    QList<Account> getAccounts() const;
    void addAccount(const Account& account);
    void deleteAccount(const QString& accountName);
    void updateAccount(const QString& accountName, const Account& updatedAccount);

    // Методы для работы с мастер-паролем
    bool loadStoredMasterPasswordHash();                   // Загрузка хэша из базы данных
    bool storeMasterPasswordHash(const QString &newPass);  // Сохранение хэша в базе данных
    bool verifyMasterPassword(const QString &inputPass);    // Проверка введённого пароля

    // Методы для Recovery Key
    bool setRecoveryKey(const QString &recoveryKey);
    bool resetMasterPasswordUsingRecoveryKey(const QString &recoveryKey, const QString &newMasterPassword);

    // Логирование событий
    void logEvent(const QString& eventDescription);

    // Сетевой функционал
    void fetchDataFromServer(const QUrl &url);

    // Получение значения из settings
    QByteArray getSettingValue(const QString &key) const; // Перемещено в public

signals:
    void dataFetched(const QByteArray &data);
    void fetchError(const QString &errorString);

private slots:
    void onNetworkReplyFinished(QNetworkReply* reply);
    void onSslErrors(QNetworkReply* reply, const QList<QSslError> &errors);

private:
    // Приватный конструктор для Singleton
    AccountManager();
    ~AccountManager();

    // Инициализация базы данных
    void initializeDatabase();

    // Работа с таблицей settings
    QByteArray getSettingValueInternal(const QString &key) const;
    bool setSettingValue(const QString &key, const QByteArray &value);

    // Сертификатный пиннинг (пример)
    QByteArray getExpectedCertHash() const;

    // Хэш мастер-пароля
    QByteArray storedHash;

    // Флаг, указывающий установлен ли мастер-пароль
    bool isMasterPasswordSet = false;

    // База данных
    QSqlDatabase db;

    // Менеджер сетевых запросов
    QNetworkAccessManager* networkManager;

    // Статические поля для "обфускации" хэша сертификата
    static const char* hashPart1;
    static const char* hashPart2;
    static const char* hashPart3;
    static const char* hashPart4;
    static const char* hashPart5;
    static const char* hashPart6;
    static const char* hashPart7;
    static const char* hashPart8;
    static const char xorKey;
};

#endif // ACCOUNT_MANAGER_H
