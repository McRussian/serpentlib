#include "settings_application.h"

const QString SettingsApplication::ORGANIZATION_NAME = "SerpentLib";
const QString SettingsApplication::APPLICATION_NAME = "SerpentLib";
const QString SettingsApplication::DATABASE_PATH_KEY = "database/path";

SettingsApplication::SettingsApplication(QObject *parent)
    : QObject(parent)
    , m_databasePath("")
    , m_databasePathIsSet(false)
{
    m_settings = new QSettings(QSettings::IniFormat,
                               QSettings::UserScope,
                               ORGANIZATION_NAME,
                               APPLICATION_NAME,
                               this);

    loadSettings();
}

SettingsApplication::~SettingsApplication()
{
    saveSettings();
}

QString SettingsApplication::getDatabasePath() const
{
    return m_databasePathIsSet ? m_databasePath : QString();
}

void SettingsApplication::setDatabasePath(const QString &path)
{
    if (!m_databasePathIsSet || m_databasePath != path) {
        m_databasePath = path;
        m_databasePathIsSet = !path.isEmpty();
        saveSettings();
        emit databasePathChanged();
    }
}

bool SettingsApplication::isDatabasePathSet() const
{
    return m_databasePathIsSet;
}

void SettingsApplication::saveSettings()
{
    if (m_databasePathIsSet) {
        m_settings->setValue(DATABASE_PATH_KEY, m_databasePath);
    } else {
        m_settings->remove(DATABASE_PATH_KEY); // Удаляем ключ, если путь не установлен
    }
    m_settings->sync();
}

void SettingsApplication::loadSettings()
{
    if (m_settings->contains(DATABASE_PATH_KEY)) {
        m_databasePath = m_settings->value(DATABASE_PATH_KEY).toString();
        m_databasePathIsSet = !m_databasePath.isEmpty();
    } else {
        m_databasePathIsSet = false;
        m_databasePath.clear();
    }
}
