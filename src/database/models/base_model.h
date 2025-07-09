#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QMap>
#include <QVariant>
#include <QList>
#include <mutex>

template <typename Derived>
class BaseModel {
protected:
    unsigned int m_id = 0; // 0 - новый, >0 - существующий в БД
    static QSqlDatabase* s_db;
    static std::mutex s_dbMutex;

    // Виртуальные методы для наследников
    virtual QString tableName() const = 0;
    virtual QMap<QString, QVariant> toMap() const = 0;
    virtual void fromMap(const QMap<QString, QVariant>& map) = 0;
    virtual QMap<QString, QString> fieldTypes() const = 0;

    QSqlDatabase& database() {
        std::lock_guard<std::mutex> lock(s_dbMutex);
        if (!s_db || !s_db->isOpen()) {
            throw std::runtime_error("Database is not initialized");
        }
        return *s_db;
    }

public:
    virtual ~BaseModel() = default;

    // Управление соединением
    static void setDatabase(QSqlDatabase* db) {
        std::lock_guard<std::mutex> lock(s_dbMutex);
        s_db = db;
    }

    // === Статические методы для работы с таблицей ===
    static bool createTable() {
        QMap<QString, QString> fields = Derived().fieldTypes();
        if (fields.empty()) {
            qWarning() << "No fields defined for table";
            return false;
        }

        QStringList definitions;
        for (auto it = fields.constBegin(); it != fields.constEnd(); ++it) {
            definitions << QString("%1 %2").arg(it.key()).arg(it.value());
        }

        QSqlQuery query(Derived().database());
        return query.exec(
            QString("CREATE TABLE IF NOT EXISTS %1 (%2)")
                .arg(Derived().tableName())
                .arg(definitions.join(", "))
            );
    }

    static bool tableExists() {
        QSqlQuery query(Derived().database());
        query.prepare(
            "SELECT name FROM sqlite_master "
            "WHERE type='table' AND name=?"
            );
        query.addBindValue(Derived().tableName());
        return query.exec() && query.next();
    }

    static bool validateTable() {
        QSqlQuery query(Derived().database());
        if (!query.exec(QString("PRAGMA table_info(%1)").arg(Derived().tableName()))) {
            return false;
        }

        QSet<QString> dbFields;
        while (query.next()) {
            dbFields.insert(query.value("name").toString());
        }

        const auto keys = Derived().fieldTypes().keys();
        QSet<QString> modelFields(keys.begin(), keys.end());
        return dbFields == modelFields;
    }

    // === Статические методы для работы с данными ===
    static QList<Derived> getAll() {
        QList<Derived> result;
        QSqlQuery query(Derived().database());

        if (!query.exec(QString("SELECT * FROM %1").arg(Derived().tableName()))) {
            qWarning() << "Failed to fetch records: " << query.lastError();
            return result;
        }

        while (query.next()) {
            QMap<QString, QVariant> record;
            QSqlRecord sqlRecord = query.record();

            for (int i = 0; i < sqlRecord.count(); ++i) {
                record.insert(sqlRecord.fieldName(i), query.value(i));
            }

            Derived item;
            item.fromMap(record);
            result.append(item);
        }

        return result;
    }

    static int count() {
        QSqlQuery query(Derived().database());
        if (!query.exec(QString("SELECT COUNT(*) FROM %1").arg(Derived().tableName()))) {
            qWarning() << "Failed to count records:" << query.lastError();
            return -1; // Возвращаем -1 при ошибке
        }

        if (query.next()) {
            return query.value(0).toInt();
        }

        return 0;
    }

    // === Методы работы с объектом ===
    bool save() {
        if (m_id != 0) {
            qWarning() << "Use update() for existing objects (ID:" << m_id << ")";
            return false;
        }

        QMap<QString, QVariant> data = toMap();
        QStringList fields, placeholders;
        QList<QVariant> values;

        for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
            if (it.key().compare("id", Qt::CaseInsensitive) == 0) continue;
            fields << it.key();
            placeholders << "?";
            values << it.value();
        }

        QSqlQuery query(database());
        if (!query.prepare(
                QString("INSERT INTO %1 (%2) VALUES (%3)")
                    .arg(tableName())
                    .arg(fields.join(", "))
                    .arg(placeholders.join(", ")))
            ) {
            return false;
        }

        for (const auto& v : values) query.addBindValue(v);
        if (!query.exec()) return false;

        m_id = query.lastInsertId().toUInt();
        return m_id > 0;
    }

    bool update() {
        if (m_id == 0) {
            qWarning() << "Object not saved yet";
            return false;
        }

        QMap<QString, QVariant> data = toMap();
        QStringList updates;
        QList<QVariant> values;

        for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
            if (it.key().compare("id", Qt::CaseInsensitive) == 0) continue;
            updates << QString("%1 = ?").arg(it.key());
            values << it.value();
        }

        QSqlQuery query(database());
        if (!query.prepare(
                QString("UPDATE %1 SET %2 WHERE id = ?")
                    .arg(tableName())
                    .arg(updates.join(", ")))
            ) {
            return false;
        }

        for (const auto& v : values) query.addBindValue(v);
        query.addBindValue(m_id);

        return query.exec();
    }

    bool load(unsigned int id) {
        if (id == 0) return false;

        QSqlQuery query(database());
        query.prepare(QString("SELECT * FROM %1 WHERE id = ?").arg(tableName()));
        query.addBindValue(id);

        if (!query.exec() || !query.next()) return false;

        QMap<QString, QVariant> map;
        QSqlRecord record = query.record();
        for (int i = 0; i < record.count(); ++i) {
            map.insert(record.fieldName(i), query.value(i));
        }

        fromMap(map);
        m_id = id;
        return true;
    }

    bool remove() {
        if (m_id == 0) return false;

        QSqlQuery query(database());
        query.prepare(QString("DELETE FROM %1 WHERE id = ?").arg(tableName()));
        query.addBindValue(m_id);

        if (!query.exec()) return false;

        m_id = 0;
        return true;
    }

    // Геттеры
    unsigned int id() const { return m_id; }
    bool isNew() const { return m_id == 0; }
    bool existsInDb() const { return m_id > 0; }
};

template <typename Derived>
QSqlDatabase* BaseModel<Derived>::s_db = nullptr;

template <typename Derived>
std::mutex BaseModel<Derived>::s_dbMutex;

#endif // BASEMODEL_H
