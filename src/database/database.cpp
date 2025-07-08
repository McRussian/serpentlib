#include "database.h"
#include "dbexception.h"
#include <QDebug>
#include <QFileInfo>

DataBase::DataBase()
{

}

DataBase::~DataBase()
{
    if (is_open())
        m_db.close();
}

void DataBase::create(QString filename)
{
    initialize(filename);
    initModels();
}

void DataBase::open(QString filename)
{
    initialize(filename);
    checkTables();
}

bool DataBase::is_open()
{
    return m_db.isOpen();
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
