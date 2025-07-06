#ifndef AUTHOR_H
#define AUTHOR_H

#include <QString>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "../database.h"


class Author
{
public:
    // Конструктор
    Author();
    ~Author();

    // Геттеры
    unsigned int id() const;
    QString firstName() const;
    QString lastName() const;
    QString surname() const;
    QString comment() const;

    QString briefName() const;
    QString fullName() const;

    // Сеттеры
    void setId(const unsigned int newId);
    void setFirstName(const QString &firstName);
    void setLastName(const QString &lastName);
    void setSurname(const QString &surname);
    void setComment(const QString &comment);

    // Статические методы для работы с БД
    static bool createTable();
    static bool verifyTableStructure();
    static bool tableExists();
    // Статические методы для работы с БД
    static QList<Author> get();
    static Author getById(unsigned int id);

    // Исключение для случая, когда автор не найден
    class NotFoundException : public std::runtime_error {
    public:
        NotFoundException() : std::runtime_error("Author not found") {}
    };

    static const QMap<QString, QString> FieldDefinitions;

    // Методы экземпляра
    bool save();
    bool update();

#ifdef QT_TESTLIB_LIB
    static void resetDatabaseConnection() {
        DataBase::instance().database().close();
    }
#endif



private:
    unsigned int m_id;
    QString m_firstName;
    QString m_lastName;
    QString m_surname;
    QString m_comment;
};

#endif // AUTHOR_H
