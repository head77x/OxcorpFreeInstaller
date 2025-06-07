#include "ConfigManager.h"
#include <QSettings>
#include <QFile>
#include <QTextStream>

void ConfigManager::loadFromRegistry() {
    m_extractPath = qEnvironmentVariable("OXCORP_INSTALL_PATH");
    m_appExecutable = qEnvironmentVariable("OXCORP_EXECUTABLE");
    m_baseUrl = qEnvironmentVariable("OXCORP_BASE_URL");
    m_versionFileUrl = qEnvironmentVariable("OXCORP_VERSION_URL");
}

QString ConfigManager::readLocalVersion() const {
    QFile file(m_extractPath + "/version.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString();
    QTextStream in(&file);
    return in.readLine().trimmed();
}

void ConfigManager::writeLocalVersion(const QString& version) const {
    QFile file(m_extractPath + "/version.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << version << "\n";
    }
}
