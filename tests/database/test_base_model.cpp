#include "test_base_model.h"
#include <QSqlQuery>

TestBaseModel::TestBaseModel() {}
TestBaseModel::~TestBaseModel() {}

void TestBaseModel::initTestCase()
{
    // Настройка тестовой БД в памяти
    m_db = QSqlDatabase::addDatabase("QSQLITE", "test_connection");
    m_db.setDatabaseName(":memory:");
    QVERIFY(m_db.open());

    BaseModel<TestUserModel>::setDatabase(&m_db);
    recreateTestTable();
}

void TestBaseModel::cleanupTestCase()
{
    m_db.close();
    QSqlDatabase::removeDatabase("test_connection");
}

void TestBaseModel::recreateTestTable()
{
    QSqlQuery query(m_db);
    query.exec("DROP TABLE IF EXISTS users");
    TestUserModel::createTable();
}

// === Тесты работы с таблицей ===
void TestBaseModel::testCreateTable()
{
    recreateTestTable();
    QVERIFY(TestUserModel::tableExists());

    // Проверяем структуру таблицы
    QSqlQuery query(m_db);
    QVERIFY(query.exec("PRAGMA table_info(users)"));

    int fieldCount = 0;
    bool hasIdField = false;
    while (query.next()) {
        QString name = query.value("name").toString();
        QString type = query.value("type").toString();

        if (name == "id") {
            hasIdField = true;
            QVERIFY(type.contains("INTEGER"));
            QVERIFY(query.value("pk").toInt() == 1);
        }
        fieldCount++;
    }

    QVERIFY(hasIdField);
    QCOMPARE(fieldCount, 3); // id, username, email
}

void TestBaseModel::testTableExists()
{
    QVERIFY(TestUserModel::tableExists());
    QSqlQuery(m_db).exec("DROP TABLE users");
    QVERIFY(!TestUserModel::tableExists());
    recreateTestTable();
}

void TestBaseModel::testValidateTable()
{
    QVERIFY(TestUserModel::validateTable());

    // Добавляем лишнее поле
    QSqlQuery(m_db).exec("ALTER TABLE users ADD COLUMN extra_field TEXT");
    QVERIFY(!TestUserModel::validateTable());

    recreateTestTable();
}

// === Тесты CRUD операций ===
void TestBaseModel::testSaveNewObject()
{
    TestUserModel user;
    user.setUsername("test_user");
    user.setEmail("test@example.com");

    QVERIFY(user.isNew());
    QVERIFY(user.save());
    QVERIFY(user.id() > 0);
    QVERIFY(user.existsInDb());
}

void TestBaseModel::testUpdateObject()
{
    TestUserModel user;
    user.setUsername("original");
    user.setEmail("original@example.com");
    QVERIFY(user.save());

    unsigned int originalId = user.id();
    user.setUsername("updated");
    user.setEmail("updated@example.com");

    QVERIFY(user.update());
    QCOMPARE(user.id(), originalId);

    // Проверяем что данные обновились в БД
    TestUserModel loadedUser;
    QVERIFY(loadedUser.load(originalId));
    QCOMPARE(loadedUser.username(), QString("updated"));
}

void TestBaseModel::testLoadObject()
{
    // Создаем тестовые данные напрямую через SQL
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO users (username, email) VALUES (?, ?)");
    query.addBindValue("loaded_user");
    query.addBindValue("loaded@example.com");
    QVERIFY(query.exec());

    unsigned int insertedId = query.lastInsertId().toUInt();

    TestUserModel user;
    QVERIFY(user.load(insertedId));
    QCOMPARE(user.username(), QString("loaded_user"));
    QCOMPARE(user.email(), QString("loaded@example.com"));
}

void TestBaseModel::testRemoveObject()
{
    TestUserModel user;
    user.setUsername("to_delete");
    user.setEmail("delete@example.com");
    QVERIFY(user.save());

    unsigned int idToDelete = user.id();
    QVERIFY(user.remove());
    QVERIFY(user.isNew());

    // Проверяем что записи больше нет в БД
    TestUserModel checkUser;
    QVERIFY(!checkUser.load(idToDelete));
}

void TestBaseModel::testGetAllObjects()
{
    recreateTestTable(); // Очищаем таблицу

    // Создаем 3 тестовых пользователя
    for (int i = 1; i <= 3; ++i) {
        TestUserModel user;
        user.setUsername(QString("user%1").arg(i));
        user.setEmail(QString("user%1@example.com").arg(i));
        QVERIFY(user.save());
    }

    QList<TestUserModel> allUsers = TestUserModel::getAll();
    QCOMPARE(allUsers.size(), 3);

    // Проверяем что все объекты загружены корректно
    for (const TestUserModel& user : allUsers) {
        QVERIFY(user.id() > 0);
        QVERIFY(user.username().startsWith("user"));
        QVERIFY(user.email().contains("@example.com"));
    }
}

// === Тесты граничных случаев ===
void TestBaseModel::testInvalidIdOperations()
{
    TestUserModel user;

    // Попытка обновления нового объекта
    QVERIFY(!user.update());

    // Попытка удаления нового объекта
    QVERIFY(!user.remove());

    // Попытка загрузки несуществующего ID
    QVERIFY(!user.load(999999));
}

void TestBaseModel::testDoubleSaveProtection()
{
    TestUserModel user;
    user.setUsername("double_save");
    user.setEmail("double@example.com");
    QVERIFY(user.save());

    // Повторный вызов save() для существующего объекта
    QVERIFY(!user.save()); // Должен вернуть false
}
