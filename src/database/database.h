#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QString>


class DataBase
{
public:
    static DataBase& instance(const QString& path = ":memory:");
    QSqlDatabase& database();

    // Запрещаем копирование
    DataBase(const DataBase&) = delete;
    DataBase& operator=(const DataBase&) = delete;

private:
    DataBase(const QString& path = ":memory:"); // private конструктор
    ~DataBase();

    QSqlDatabase m_db; // Не статическое поле
};

#endif // DATABASE_H
