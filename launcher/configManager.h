#pragma once

#include <QString>

class ConfigManager {
public:
    static ConfigManager& instance() {
        static ConfigManager inst;
        return inst;
    }

    void loadFromRegistry();

    QString baseUrl() const { return m_baseUrl; }
    QString versionFileUrl() const { return m_versionFileUrl; }
    QString extractPath() const { return m_extractPath; }
    QString appExecutableName() const { return m_appExecutable; }

    QString readLocalVersion() const;
    void writeLocalVersion(const QString& version) const;

private:
    QString m_baseUrl;
    QString m_versionFileUrl;
    QString m_extractPath;
    QString m_appExecutable;
};
