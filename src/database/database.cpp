#include "database.h"
#include "dbexception.h"
#include <QDebug>
#include <QSqlDriver>
#include <QFileInfo>

// Проверка наличия SQLite заголовков
#ifndef SQLITE_INSERT
#define SQLITE_INSERT 18
#define SQLITE_UPDATE 23
#define SQLITE_DELETE 9
#endif



DataBase::DataBase()
{

}

DataBase::~DataBase()
{
    if (is_open()) {
        removeUpdateHook();
        m_db.close();
    }
}

void DataBase::create(QString filename)
{
    initialize(filename);
    initModels();
    installUpdateHook();
}

void DataBase::open(QString filename)
{
    initialize(filename);
    checkTables();
    installUpdateHook();
}

bool DataBase::is_open()
{
    return m_db.isOpen();
}

QSqlDatabase DataBase::db() const
{
    return m_db;
}

void DataBase::updateHook(void *userData, int op, const char *dbName, const char *tableName, sqlite3_int64 rowid)
{
    // Статический метод обработки хука
    Q_UNUSED(dbName)
    Q_UNUSED(rowid)

    DataBase *db = static_cast<DataBase*>(userData);
    if (db) {
        emit db->tableChanged(QString(tableName), static_cast<ChangeType>(op));
    }
}

void DataBase::installUpdateHook()
{
    if (!m_db.isOpen()) {
        qWarning() << "Database is not open";
        return;
    }

    // Получаем низкоуровневый хэндл SQLite
    QVariant v = m_db.driver()->handle();
    if (!v.isValid() || qstrcmp(v.typeName(), "sqlite3*") != 0) {
        qWarning() << "Invalid SQLite handle";
        return;
    }

    sqlite3* handle = *static_cast<sqlite3**>(v.data());
    if (!handle) {
        qWarning() << "Null SQLite handle";
        return;
    }

    // Устанавливаем хук
    sqlite3_update_hook(handle, [](void *userData, int op, char const *, char const *table, sqlite3_int64) {
        DataBase *db = static_cast<DataBase*>(userData);
        if (db) {
            emit db->tableChanged(QString(table), static_cast<DataBase::ChangeType>(op));
        }
    }, this);
}

void DataBase::removeUpdateHook()
{
    if (!m_db.isOpen()) return;

    sqlite3 *handle = nullptr;
    QVariant v = m_db.driver()->handle();
    if (v.isValid() && qstrcmp(v.typeName(), "sqlite3*") == 0) {
        handle = *static_cast<sqlite3 **>(v.data());
    }

    if (handle) {
        sqlite3_update_hook(handle, nullptr, nullptr);
    }
}

void DataBase::initialize(QString filename)
{
    if (m_db.isOpen()) {
        return;
    }

    QFileInfo fileInfo(filename);
    if (!fileInfo.exists() || !fileInfo.isFile()) {
            throw DBException(QString("Failed to create database directory: %1").arg(filename));
    }

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(filename);

    if (!m_db.open()) {
        throw DBException("Cannot open database", m_db.lastError());
    }

    QSqlQuery query("PRAGMA foreign_keys = ON;", m_db);
    if (!query.exec()) {
        throw DBException("Failed to enable foreign keys", query.lastError());
    }
}

void DataBase::initModels()
{
    BaseModel<Author>::setDatabase(&m_db);
    if (!Author::createTable())
        throw DBException(QString("Failed to create table: %1").arg(Author().tableName()));
}

void DataBase::checkTables()
{
    BaseModel<Author>::setDatabase(&m_db);

    if (!Author::validateTable())
        throw DBException(QString("Uncorrect format table: %1").arg(Author().tableName()));
}
