#include "recovery_manager.h"
#include <QCryptographicHash>
#include <QUuid>

RecoveryManager& RecoveryManager::instance()
{
    static RecoveryManager mgr;
    return mgr;
}

RecoveryManager::RecoveryManager()
{

}

QString RecoveryManager::generateRecoveryKey() const
{
    // Генерация UUID без фигурных скобок
    QString rawUuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    return rawUuid;
}

QByteArray RecoveryManager::hashRecoveryKey(const QString &recoveryKey) const
{
    return QCryptographicHash::hash(recoveryKey.toUtf8(), QCryptographicHash::Sha256);
}

bool RecoveryManager::verifyRecoveryKey(const QString &inputKey, const QByteArray &storedHash) const
{
    QByteArray inputHash = QCryptographicHash::hash(inputKey.toUtf8(), QCryptographicHash::Sha256);
    return (inputHash == storedHash);
}
