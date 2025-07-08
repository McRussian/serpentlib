#ifndef DBEXCEPTION_H
#define DBEXCEPTION_H

#include <QException>
#include <QString>
#include <QSqlError>

class DBException : public QException
{
public:
    explicit DBException(const QString& message, const QSqlError& sqlError)
        : m_message(message), m_sqlError(sqlError)
    {
    }
    explicit DBException(const QString& message)
        : m_message(message), m_sqlError(QSqlError())
    {
    }
    DBException(const DBException& other)
        : m_message(other.m_message), m_sqlError(other.m_sqlError)
    {
    }

    QString message() const
    {
        return m_message;
    }
    QSqlError sqlError() const
    {
        return m_sqlError;
    }

    void raise() const override
    {
        throw *this;
    }
    DBException* clone() const override
    {
        return new DBException(*this);
    }

private:
    QString m_message;
    QSqlError m_sqlError;
};


#endif // DBEXCEPTION_H
