#include "test_database_hook.h"
#include "../../src/database/dbexception.h"


#define QVERIFY_NO_THROW(expression) \
do { \
    try { \
            expression; \
            QVERIFY(true); \
    } catch (const std::exception& e) { \
            QFAIL(qPrintable(QString("Unexpected exception: %1").arg(e.what()))); \
    } catch (...) { \
            QFAIL("Unexpected exception thrown"); \
    } \
} while (false)


TestDatabaseHooks::TestDatabaseHooks() :
    m_db(nullptr),
    m_testTable("test_hooks_table"),
    m_signalCounter(0)
{
}

TestDatabaseHooks::~TestDatabaseHooks()
{
}

void TestDatabaseHooks::initTestCase()
{
    m_db = new DataBase();
    QVERIFY_NO_THROW(m_db->open(":memory:"));

    // Создаем тестовую таблицу
    QSqlQuery q(m_db->db());
    QVERIFY(q.exec(QString("CREATE TABLE %1 (id INTEGER PRIMARY KEY, data TEXT)").arg(m_testTable)));

    // Подключаем обработчик сигналов
    connect(m_db, &DataBase::tableChanged, this, &TestDatabaseHooks::handleTableChange);
}

void TestDatabaseHooks::cleanupTestCase()
{
    if (m_db->is_open()) {
        delete m_db;
    }
}

void TestDatabaseHooks::handleTableChange(const QString& tableName, DataBase::ChangeType operation)
{
    m_signalCounter++;
    m_lastTable = tableName;
    m_lastOperation = operation;
}

// Тест 1: Проверка хука на INSERT
void TestDatabaseHooks::testInsertHook()
{
    m_signalCounter = 0;
    QSqlQuery q(m_db->db());

    QVERIFY(q.exec(QString("INSERT INTO %1(data) VALUES('test')").arg(m_testTable)));

    QTRY_COMPARE(m_signalCounter, 1);
    QCOMPARE(m_lastTable, m_testTable);
    QCOMPARE(m_lastOperation, DataBase::Insert);
}

// Тест 2: Проверка хука на UPDATE
void TestDatabaseHooks::testUpdateHook()
{
    m_signalCounter = 0;
    QSqlQuery q(m_db->db());

    QVERIFY(q.exec(QString("UPDATE %1 SET data = 'updated' WHERE id = 1").arg(m_testTable)));

    QTRY_COMPARE(m_signalCounter, 1);
    QCOMPARE(m_lastTable, m_testTable);
    QCOMPARE(m_lastOperation, DataBase::Update);
}

// Тест 3: Проверка хука на DELETE
void TestDatabaseHooks::testDeleteHook()
{
    m_signalCounter = 0;
    QSqlQuery q(m_db->db());

    QVERIFY(q.exec(QString("DELETE FROM %1 WHERE id = 1").arg(m_testTable)));

    QTRY_COMPARE(m_signalCounter, 1);
    QCOMPARE(m_lastTable, m_testTable);
    QCOMPARE(m_lastOperation, DataBase::Delete);
}

// Тест 4: Проверка нескольких операций подряд
void TestDatabaseHooks::testMultipleOperations()
{
    m_signalCounter = 0;
    QSqlQuery q(m_db->db());

    // Выполняем 3 операции
    QVERIFY(q.exec(QString("INSERT INTO %1(data) VALUES('multi1')").arg(m_testTable)));
    QVERIFY(q.exec(QString("UPDATE %1 SET data = 'multi2' WHERE id = 2").arg(m_testTable)));
    QVERIFY(q.exec(QString("DELETE FROM %1 WHERE id = 2").arg(m_testTable)));

    QTRY_COMPARE(m_signalCounter, 3);
}

// Тест 5: Проверка невалидных операций
void TestDatabaseHooks::testInvalidOperations()
{
    m_signalCounter = 0;
    QSqlQuery q(m_db->db());

    // Несуществующая таблица - не должно быть сигнала
    QVERIFY(!q.exec("UPDATE nonexistent_table SET x = 1"));
    QTest::qWait(100);
    QCOMPARE(m_signalCounter, 0);
}

