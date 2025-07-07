#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <QVariant>
#include <QMap>

template<typename T>
class BaseModel {
protected:
    static QSqlDatabase* m_dbInstance;
    static QString m_tableName;
    static QMap<QString, QString> m_fields;

    // Чисто виртуальный метод для создания объекта из данных
    virtual T createFromMap(const QVariantMap& data) const = 0;

public:
    // Установка соединения с БД
    static void setDatabase(QSqlDatabase* database);

    // Получение соединения с проверкой
    static QSqlDatabase& database();

    // Проверка существования таблицы
    static bool tableExists();

    // Создание таблицы
    static bool createTable();

    // Проверка структуры таблицы
    static bool validateTable();

    // Получение всех записей как объектов
    static QList<T> get();

    // Получение объекта по ID
    static T getById(unsigned int id);

    virtual ~BaseModel() = default;
};

// Инициализация статических членов шаблона
template<typename T>
QSqlDatabase* BaseModel<T>::m_dbInstance = nullptr;

template<typename T>
QString BaseModel<T>::m_tableName;

template<typename T>
QMap<QString, QString> BaseModel<T>::m_fields;

#endif // BASEMODEL_H
