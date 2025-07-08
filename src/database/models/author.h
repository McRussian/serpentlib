#ifndef AUTHOR_H
#define AUTHOR_H

#include <QString>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "base_model.h"


class Author : public BaseModel<Author>
{
public:
    Author();
    ~Author();

    QString tableName() const override;
    QMap<QString, QString> fieldTypes() const override;

    QMap<QString, QVariant> toMap() const override;

    void fromMap(const QMap<QString, QVariant>& map) override;

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

private:
    unsigned int m_id;
    QString m_firstName;
    QString m_lastName;
    QString m_surname;
    QString m_comment;
};

#endif // AUTHOR_H
