#include "database.h"
#include <QDebug>

// Определяем статический экземпляр
DataBase& DataBase::instance()
{
    static DataBase instance; // Создается при первом вызове
    return instance;
}

DataBase::DataBase(const QString& path)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if (!m_db.open()) {
        qCritical() << "Failed to open database:" << m_db.lastError();
    }
}

DataBase::~DataBase()
{
    m_db.close();
}

QSqlDatabase& DataBase::database()
{
    return m_db;
}
