#ifndef SETTINGSAPPLICATION_H
#define SETTINGSAPPLICATION_H

#include <QObject>
#include <QSettings>
#include <QString>

class SettingsApplication : public QObject
{
    Q_OBJECT

public:
    explicit SettingsApplication(QObject *parent = nullptr);
    ~SettingsApplication();

    // Путь к базе данных
    QString getDatabasePath() const;
    void setDatabasePath(const QString &path);
    bool isDatabasePathSet() const; // Переименованный метод для ясности

    // Сохранение и загрузка настроек
    void saveSettings();
    void loadSettings();

signals:
    void databasePathChanged();

private:
    QSettings *m_settings;
    QString m_databasePath;
    bool m_databasePathIsSet; // Флаг, указывающий, установлен ли путь

    static const QString ORGANIZATION_NAME;
    static const QString APPLICATION_NAME;
    static const QString DATABASE_PATH_KEY;
};

#endif // SETTINGSAPPLICATION_H
