#include "author.h"
#include "../../lib/field.h"

// Визначення полів та їх типів


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

QString Author::tableName() const { return "authors"; }


QMap<QString, QString> Author::fieldTypes() const {
    return {
        {"id", "INTEGER PRIMARY KEY AUTOINCREMENT"},
        {"last_name", "TEXT NOT NULL"},
        {"first_name", "TEXT NOT NULL"},
        {"surname", "TEXT"},
        {"comment", "TEXT"}
    };

}

QMap<QString, QVariant> Author::toMap() const {
    return {
        {"id", m_id},
        {"last_name", m_lastName},
        {"first_name", m_firstName},
        {"surname", m_surname},
        {"comment", m_comment}
    };
}

void Author::fromMap(const QMap<QString, QVariant> &map) {
    m_id = map["id"].toUInt();
    m_lastName = map["last_name"].toString();
    m_firstName = map["first_name"].toString();
    m_surname = map["surname"].toString();
    m_comment = map["comment"].toString();
}


// Геттеры


unsigned int Author::id() const { return m_id; }
QString Author::firstName() const { return m_firstName; }
QString Author::lastName() const { return m_lastName; }
QString Author::surname() const { return m_surname; }
QString Author::comment() const { return m_comment; }


QString Author::briefName() const
{
    QString name = m_lastName + " " + m_firstName[0] + ".";
    if (!m_surname.isEmpty()) {
        name += " " + QString(m_surname[0]) + ".";
    }
    return name;
}

QString Author::fullName() const
{
    QString name = m_lastName + " " + m_firstName;
    if (!m_surname.isEmpty())
        name += " " + m_surname;
    return name;
}

// Сеттеры
void Author::setFirstName(const QString &firstName) { m_firstName = normalizeName(firstName); }
void Author::setLastName(const QString &lastName) { m_lastName = normalizeName(lastName); }
void Author::setSurname(const QString &surname) { m_surname = normalizeName(surname); }
void Author::setComment(const QString &comment) { m_comment = comment; }
void Author::setId(const unsigned int newId) {m_id = newId;}
