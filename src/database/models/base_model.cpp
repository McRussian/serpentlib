#include "base_model.h"

template<typename T>
void BaseModel<T>::setDatabase(QSqlDatabase *db) {
    m_dbInstance = db;
}

template<typename T>
QSqlDatabase &BaseModel<T>::database() {
    if (!m_dbInstance || !m_dbInstance->isOpen()) {
        throw std::runtime_error("Database is not initialized or not open");
    }
    return *m_dbInstance;
}

template<typename T>
bool BaseModel<T>::createTable() {
    QSqlDatabase& dbRef = database();

    QStringList fieldDefinitions;
    for (auto it = m_fields.constBegin(); it != m_fields.constEnd(); ++it) {
        fieldDefinitions << QString("%1 %2").arg(it.key()).arg(it.value());
    }

    QString queryStr = QString("CREATE TABLE IF NOT EXISTS %1 (%2)")
                           .arg(m_tableName)
                           .arg(fieldDefinitions.join(", "));

    QSqlQuery query(dbRef);
    if (!query.exec(queryStr)) {
        qWarning() << "Failed to create table" << m_tableName << ":" << query.lastError();
        return false;
    }

    qDebug() << "Table" << m_tableName << "created successfully";
    return true;
}

template<typename T>
bool BaseModel<T>::validateTable() {
    QSqlDatabase& dbRef = database();
    QSqlQuery query(dbRef);

    if (!query.exec(QString("PRAGMA table_info(%1)").arg(m_tableName))) {
        qWarning() << "Failed to get table info for" << m_tableName << ":" << query.lastError();
        return false;
    }

    // Собираем информацию о полях в существующей таблице
    QMap<QString, QString> existingFields;
    while (query.next()) {
        QString fieldName = query.value("name").toString();
        QString fieldType = query.value("type").toString();
        existingFields.insert(fieldName, fieldType);
    }

    // Проверяем наличие всех требуемых полей (используем итераторы вместо structured binding)
    for (auto it = m_fields.constBegin(); it != m_fields.constEnd(); ++it) {
        const QString& requiredField = it.key();
        const QString& requiredType = it.value();

        if (!existingFields.contains(requiredField)) {
            qWarning() << "Missing required field:" << requiredField;
            return false;
        }

        // Проверяем совместимость типов
        QString existingType = existingFields[requiredField].toUpper();
        QString simplifiedRequiredType = requiredType.toUpper().split(' ').first();

        if (!existingType.contains(simplifiedRequiredType)) {
            qWarning() << "Type mismatch for field" << requiredField
                       << "(expected:" << simplifiedRequiredType
                       << "actual:" << existingType << ")";
            return false;
        }
    }

    // Проверяем отсутствие лишних полей
    for (auto it = existingFields.constBegin(); it != existingFields.constEnd(); ++it) {
        const QString& existingField = it.key();
        if (!m_fields.contains(existingField)) {
            qWarning() << "Extra field in table that is not in model:" << existingField;
            return false;
        }
    }

    qDebug() << "Table" << m_tableName << "validated successfully - structure matches model definition";
    return true;
}

template<typename T>
QList<T> BaseModel<T>::get() {
    QSqlDatabase& dbRef = database();
    QSqlQuery query(dbRef);
    QList<T> result;

    if (!query.exec(QString("SELECT * FROM %1").arg(m_tableName))) {
        qWarning() << "Failed to fetch records:" << query.lastError();
        return result;
    }

    while (query.next()) {
        QVariantMap record;
        for (const auto& field : m_fields.keys()) {
            record[field] = query.value(field);
        }
        // Создаем объект через фабричный метод
        result.append(createFromMap(record));
    }

    return result;
}

template<typename T>
T BaseModel<T>::getById(unsigned int id) {
    QSqlDatabase& dbRef = database();
    QSqlQuery query(dbRef);
    query.prepare(QString("SELECT * FROM %1 WHERE id = :id").arg(m_tableName));
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Failed to fetch record by id:" << query.lastError();
        throw std::runtime_error("Database query failed");
    }

    if (!query.next()) {
        throw std::runtime_error(QString("Record with id %1 not found").arg(id).toStdString());
    }

    QVariantMap record;
    for (const auto& field : m_fields.keys()) {
        record[field] = query.value(field);
    }

    return createFromMap(record);
}
