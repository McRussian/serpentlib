#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QDebug>

class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    bool createDatabase(const QString& path);
    bool openDatabase(const QString& path);
    void closeDatabase();
    bool isOpen() const;
    QSqlError lastError() const;

private:
    QSqlDatabase m_db;
};

#endif // DATABASE_H
