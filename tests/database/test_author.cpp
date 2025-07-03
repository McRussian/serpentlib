#include "test_author.h"
#include "database/database.h"

void TestAuthor::initTestCase()
{
    // Инициализируем тестовую базу в памяти
    DataBase::instance().database().close();
    DataBase::instance().database().setDatabaseName(":memory:");
    QVERIFY(DataBase::instance().database().open());
}

void TestAuthor::cleanupTestCase()
{
    // Закрываем соединение
    DataBase::instance().database().close();
}

void TestAuthor::testCreateTable()
{
    QVERIFY(Author::createTable());

    // Проверяем, что таблица создана
    QSqlQuery query(DataBase::instance().database());
    QVERIFY(query.exec("SELECT 1 FROM authors LIMIT 1"));
}

void TestAuthor::testSaveAuthor()
{
    Author author;
    author.setFirstName("Лев");
    author.setLastName("Толстой");

    QVERIFY(author.save());
    QVERIFY(author.id() > 0);

    // Проверяем, что автор сохранился в БД
    QSqlQuery query(DataBase::instance().database());
    query.prepare("SELECT first_name FROM authors WHERE id = ?");
    query.addBindValue(author.id());
    QVERIFY(query.exec());
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toString(), QString("Лев"));
}

void TestAuthor::testAuthorFields()
{
    Author author;

    // Проверка сеттеров/геттеров
    author.setFirstName("Фёдор");
    author.setLastName("Достоевский");
    author.setSurname("Михайлович");
    author.setComment("Русский классик");

    QCOMPARE(author.firstName(), QString("Фёдор"));
    QCOMPARE(author.lastName(), QString("Достоевский"));
    QCOMPARE(author.surname(), QString("Михайлович"));
    QCOMPARE(author.comment(), QString("Русский классик"));
}
