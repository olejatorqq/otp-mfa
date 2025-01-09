#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QSqlDatabase>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslError>

struct Account {
    QString name;
    QString secret;
    QString algorithm;
    int digits;
    int period;
    QString type;   // "TOTP" или "HOTP"
    quint64 counter;    // Для HOTP
};

class AccountManager : public QObject {
    Q_OBJECT

public:
    static AccountManager& instance();

    // Запрещаем копирование и присваивание
    AccountManager(AccountManager const&) = delete;
    void operator=(AccountManager const&) = delete;

    QList<Account> getAccounts() const;
    void addAccount(const Account& account);
    void deleteAccount(const QString& accountName);
    void updateAccount(const QString& accountName, const Account& updatedAccount);

    bool verifyMasterPassword();

    // Метод для записи логов
    void logEvent(const QString& eventDescription);

    // Функция для выполнения HTTPS-запроса
    void fetchDataFromServer(const QUrl &url);

signals:
    void dataFetched(const QByteArray &data);
    void fetchError(const QString &errorString);

private slots:
    void onNetworkReplyFinished(QNetworkReply* reply);
    void onSslErrors(QNetworkReply* reply, const QList<QSslError> &errors);

private:
    AccountManager();
    ~AccountManager();

    QSqlDatabase db;
    QNetworkAccessManager* networkManager;

    void initializeDatabase();  // Инициализация базы данных

    // Метод для получения обфусцированного хэша
    QByteArray getExpectedCertHash() const;
};

#endif // ACCOUNT_MANAGER_H
