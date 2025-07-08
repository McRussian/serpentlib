#ifndef TEST_MODEL_H
#define TEST_MODEL_H

#include "../../src/database/models/base_model.h"

class TestUserModel : public BaseModel<TestUserModel>
{
public:
    // Делаем методы public для тестов
    QString tableName() const override { return "users"; }

    QMap<QString, QString> fieldTypes() const override {
        return {
            {"id", "INTEGER PRIMARY KEY AUTOINCREMENT"},
            {"username", "TEXT NOT NULL"},
            {"email", "TEXT NOT NULL"}
        };
    }

    QMap<QString, QVariant> toMap() const override {
        return {
            {"id", m_id},
            {"username", m_username},
            {"email", m_email}
        };
    }

    void fromMap(const QMap<QString, QVariant>& map) override {
        m_id = map["id"].toUInt();
        m_username = map["username"].toString();
        m_email = map["email"].toString();
    }

    // Геттеры и сеттеры
    QString username() const { return m_username; }
    QString email() const { return m_email; }
    void setUsername(const QString& username) { m_username = username; }
    void setEmail(const QString& email) { m_email = email; }

private:
    QString m_username;
    QString m_email;
};

#endif // TEST_MODEL_H

