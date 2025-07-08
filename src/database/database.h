#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>

#include "models/base_model.h"
#include "models/author.h"

class DataBase
{
public:
    DataBase();
    ~DataBase();
    void create(QString filename);
    void open(QString filename);
    bool is_open();
    // Запрещаем копирование
    DataBase(const DataBase&) = delete;
    DataBase& operator=(const DataBase&) = delete;

private:
    void initialize(QString filename);
    void initModels();
    void checkTables();

    QSqlDatabase m_db; // Не статическое поле
};

#endif // DATABASE_H
