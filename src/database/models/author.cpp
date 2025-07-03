#include "author.h"

Author::Author() :
    m_id(0),
    m_firstName(""),
    m_lastName(""),
    m_surname(""),
    m_comment("")
{
}

Author::~Author()
{

}

// Геттеры
unsigned int Author::id() const { return m_id; }
QString Author::firstName() const { return m_firstName; }
QString Author::lastName() const { return m_lastName; }
QString Author::surname() const { return m_surname; }
QString Author::comment() const { return m_comment; }

// Сеттеры
void Author::setFirstName(const QString &firstName) { m_firstName = firstName; }
void Author::setLastName(const QString &lastName) { m_lastName = lastName; }
void Author::setSurname(const QString &surname) { m_surname = surname; }
void Author::setComment(const QString &comment) { m_comment = comment; }

// Статические методы
bool Author::createTable()
{
    QSqlDatabase& db = DataBase::instance().database();
    QSqlQuery query(db);
    return query.exec("CREATE TABLE IF NOT EXISTS authors ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "first_name TEXT, "
                      "last_name TEXT NOT NULL, "
                      "surname TEXT, "
                      "comment TEXT)");
}

// Методы экземпляра
bool Author::save()
{
    QSqlDatabase& db = DataBase::instance().database();
    QSqlQuery query(db);

    // Вставка нового автора
    query.prepare("INSERT INTO authors (first_name, last_name, surname, comment) "
                  "VALUES (:first_name, :last_name, :surname, :comment)");

    query.bindValue(":first_name", m_firstName);
    query.bindValue(":last_name", m_lastName);
    query.bindValue(":surname", m_surname);
    query.bindValue(":comment", m_comment);

    if (!query.exec()) {
        qWarning() << "Failed to save author:" << query.lastError();
        return false;
    }

    // Если это новая запись, получаем ID
    m_id = query.lastInsertId().toUInt();

    return true;
}

bool Author::update()
{
    QSqlDatabase& db = DataBase::instance().database();
    QSqlQuery query(db);

    // Обновление существующего
    query.prepare("UPDATE authors SET "
                  "first_name = :first_name, "
                  "last_name = :last_name, "
                  "surname = :surname, "
                  "comment = :comment "
                  "WHERE id = :id");
    query.bindValue(":id", m_id);

    query.bindValue(":first_name", m_firstName);
    query.bindValue(":last_name", m_lastName);
    query.bindValue(":surname", m_surname);
    query.bindValue(":comment", m_comment);

    if (!query.exec()) {
        qWarning() << "Failed to save author:" << query.lastError();
        return false;
    }

    return true;
}
