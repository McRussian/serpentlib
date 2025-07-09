#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <sqlite3.h>

#include "models/base_model.h"
#include "models/author.h"

class DataBase: public QObject
{
    Q_OBJECT
public:
    DataBase();
    ~DataBase();
    void create(QString filename);
    void open(QString filename);
    bool is_open();
    // Запрещаем копирование
    DataBase(const DataBase&) = delete;
    DataBase& operator=(const DataBase&) = delete;

    enum ChangeType { Insert = SQLITE_INSERT, Update = SQLITE_UPDATE, Delete = SQLITE_DELETE };
    Q_ENUM(ChangeType)


    QSqlDatabase db() const;

signals:
    void tableChanged(const QString &tableName, ChangeType operation);

private:
    QSqlDatabase m_db;

    static void updateHook(void *userData, int op, char const *dbName,
                           char const *tableName, sqlite3_int64 rowid);
    void installUpdateHook();
    void removeUpdateHook();

    void initialize(QString filename);
    void initModels();
    void checkTables();
};

#endif // DATABASE_H
