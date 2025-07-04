#include "author.h"

// Визначення полів та їх типів
const QMap<QString, QString> Author::FieldDefinitions = {
    {"id", "INTEGER PRIMARY KEY AUTOINCREMENT"},
    {"last_name", "TEXT NOT NULL"},
    {"first_name", "TEXT NOT NULL"},
    {"surname", "TEXT"},
    {"comment", "TEXT"}
};


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
    QStringList fields;
    for (auto it = FieldDefinitions.constBegin(); it != FieldDefinitions.constEnd(); ++it) {
        fields << QString("%1 %2").arg(it.key()).arg(it.value());
    }

    QString sql = QString("CREATE TABLE IF NOT EXISTS authors (%1)").arg(fields.join(", "));

    if (!query.exec(sql)) {
        qCritical() << "Failed to create table:" << query.lastError();
        return false;
    }
    return true;
}

bool Author::verifyTableStructure()
{
    if (!tableExists()) {
        qDebug() << "Table 'authors' does not exist";
        return false;
    }

    QSqlDatabase& db = DataBase::instance().database();
    QSqlQuery query(db);

    if (!query.exec("PRAGMA table_info(authors)")) {
        qCritical() << "Failed to get table info:" << query.lastError();
        return false;
    }

    QMap<QString, QString> dbFields;
    while (query.next()) {
        QString name = query.value("name").toString();
        QString type = query.value("type").toString();
        QString notnull = query.value("notnull").toString();
        QString pk = query.value("pk").toString();

        // Формуємо тип як в FieldDefinitions
        QString fullType = type;
        if (pk == "1") fullType.prepend("PRIMARY KEY ");
        if (notnull == "1") fullType += " NOT NULL";

        dbFields.insert(name, fullType);
    }

    for (auto it = FieldDefinitions.constBegin(); it != FieldDefinitions.constEnd(); ++it) {
        if (!dbFields.contains(it.key())) {
            qDebug() << "Missing field:" << it.key();
            return false;
        }

        QString expectedType = it.value().toUpper()
                                   .replace("PRIMARY KEY", "").replace("AUTOINCREMENT", "").trimmed();
        QString actualType = dbFields[it.key()].toUpper()
                                 .replace("PRIMARY KEY", "").replace("AUTOINCREMENT", "").trimmed();

        if (actualType != expectedType) {
            qDebug() << "Type mismatch for field" << it.key()
            << "Expected:" << expectedType
            << "Actual:" << actualType;
            return false;
        }
    }

    // 2. Перевіряємо, що немає зайвих полів
    for (auto it = dbFields.constBegin(); it != dbFields.constEnd(); ++it) {
        if (!FieldDefinitions.contains(it.key())) {
            qDebug() << "Extra field in database:" << it.key();
            return false;
        }
    }

    return true;
}


bool Author::tableExists()
{
    QSqlDatabase& db = DataBase::instance().database();
    QSqlQuery query(db);
    return query.exec("SELECT 1 FROM sqlite_master WHERE type='table' AND name='authors'")
           && query.next();
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

