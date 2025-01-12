#ifndef RECOVERY_MANAGER_H
#define RECOVERY_MANAGER_H

#include <QString>
#include <QByteArray>

class RecoveryManager
{
public:
    // Получение единственного экземпляра (Singleton)
    static RecoveryManager& instance();

    // Генерация случайного Recovery Key (например, UUID)
    QString generateRecoveryKey() const;

    // Хэширование Recovery Key (SHA-256)
    QByteArray hashRecoveryKey(const QString &recoveryKey) const;

    // Проверка Recovery Key
    bool verifyRecoveryKey(const QString &inputKey, const QByteArray &storedHash) const;

private:
    // Приватный конструктор для Singleton
    RecoveryManager();
};

#endif // RECOVERY_MANAGER_H
