#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}

DataBase::~DataBase()
{
    closeDatabase();
}

bool DataBase::createDatabase(const QString &path)
{
    if (QFile::exists(path)) {
        qWarning() << "Database file already exists:" << path;
        return false;
    }

    m_db.setDatabaseName(path);
    if (!m_db.open()) {
        qCritical() << "Failed to create database:" << m_db.lastError();
        return false;
    }
 }

bool DataBase::openDatabase(const QString &path)
{
    if (!QFile::exists(path)) {
        qWarning() << "Database file not found:" << path;
        return false;
    }

    m_db.setDatabaseName(path);
    if (!m_db.open()) {
        qCritical() << "Failed to open database:" << m_db.lastError();
        return false;
    }

    qDebug() << "Database opened successfully:" << path;
    return true;
}

void DataBase::closeDatabase()
{
    if (m_db.isOpen()) {
        m_db.close();
        qDebug() << "Database closed";
    }
}

bool DataBase::isOpen() const
{
    return m_db.isOpen();
}

QSqlError DataBase::lastError() const
{
    return m_db.lastError();
}
