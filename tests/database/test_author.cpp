#include "test_author.h"
#include "database/database.h"

void TestAuthorCorrectTable::initTestCase()
{
    // Инициализируем тестовую базу в памяти
    DataBase::instance().database().close();
    DataBase::instance().database().setDatabaseName(":memory:");
    QVERIFY(DataBase::instance().database().open());
}

void TestAuthorCorrectTable::cleanupTestCase()
{
    // Закрываем соединение
    DataBase::instance().database().close();
}

void TestAuthorCorrectTable::testCreateTable()
{
    QVERIFY(Author::createTable());

    // Проверяем, что таблица создана
    QSqlQuery query(DataBase::instance().database());
    QVERIFY(query.exec("SELECT 1 FROM authors LIMIT 1"));
}

void TestAuthorCorrectTable::testSaveAuthor()
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

void TestAuthorCorrectTable::testAuthorFields()
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



void TestAuthorCheckStructureTable::initTestCase()
{
    // Инициализируем тестовую базу в памяти
    DataBase::instance().database().close();
    DataBase::instance().database().setDatabaseName(":memory:");
    QVERIFY(DataBase::instance().database().open());
}

void TestAuthorCheckStructureTable::cleanupTestCase()
{
    // Закрываем соединение
    DataBase::instance().database().close();
}

void TestAuthorCheckStructureTable::dropTable()
{
    QSqlQuery query(m_db);
    query.exec("DROP TABLE IF EXISTS authors");
}

void TestAuthorCheckStructureTable::testValidStructure()
{
    dropTable();
    QVERIFY(Author::createTable());
    QVERIFY(Author::verifyTableStructure());
}

void TestAuthorCheckStructureTable::testMissingTable()
{
    dropTable();
    QVERIFY(!Author::verifyTableStructure());
}

void TestAuthorCheckStructureTable::testMissingField()
{
    dropTable();
    QSqlQuery query(m_db);
    QVERIFY(query.exec(
        "CREATE TABLE authors ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "lastname TEXT NOT NULL, "
        // Нет поля firstname
        "surname TEXT, "
        "comment TEXT)"
        ));
    QVERIFY(!Author::verifyTableStructure());
}

void TestAuthorCheckStructureTable::testTypeMismatch()
{
    dropTable();
    QSqlQuery query(m_db);
    QVERIFY(query.exec(
        "CREATE TABLE authors ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "last_name INTEGER NOT NULL, " // Неправильный тип (должен быть TEXT)
        "first_name TEXT NOT NULL, "
        "surname TEXT, "
        "comment TEXT)"
        ));
    QVERIFY(!Author::verifyTableStructure());
}

void TestAuthorCheckStructureTable::testExtraField()
{
    dropTable();
    QSqlQuery query(m_db);
    QVERIFY(query.exec(
        "CREATE TABLE authors ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "last_name TEXT NOT NULL, "
        "first_name TEXT NOT NULL, "
        "surname TEXT, "
        "comment TEXT, "
        "extra_field TEXT)" // Лишнее поле
        ));
    QVERIFY(!Author::verifyTableStructure());
}


TestGetAuthor::TestGetAuthor() {}
TestGetAuthor::~TestGetAuthor() {}

void TestGetAuthor::initTestCase()
{
    // Инициализируем тестовую базу в памяти
    DataBase::instance().database().close();
    DataBase::instance().database().setDatabaseName(":memory:");
    QVERIFY(DataBase::instance().database().open());

}

void TestGetAuthor::cleanupTestCase()
{
    // Очищаем тестовые данные
    clearTestData();
}

void TestGetAuthor::init()
{
    // Вставляем тестовые данные перед каждым тестом
    QVERIFY(Author::createTable());

    Author tolstoy;
    tolstoy.setFirstName("Лев");
    tolstoy.setLastName("Толстой");
    tolstoy.setSurname("Николаевич");

    QVERIFY(tolstoy.save());
    QVERIFY(tolstoy.id() > 0);

    Author pushkin;
    pushkin.setLastName("Пушкин");
    pushkin.setFirstName("Александр");
    pushkin.setSurname("");
    QVERIFY(pushkin.save());
    QVERIFY(pushkin.id() > 0);

    m_testAuthorId = tolstoy.id();
}

void TestGetAuthor::cleanup()
{
    // Очищаем данные после каждого теста
    clearTestData();
}

void TestGetAuthor::clearTestData()
{
    QSqlQuery query(DataBase::instance().database());
    QVERIFY(query.exec("DELETE FROM authors"));
}

void TestGetAuthor::testGetAllAuthors()
{
    QList<Author> authors = Author::get();

    QCOMPARE(authors.size(), 2);

    // Проверяем порядок (должен быть отсортирован по фамилии)
    QCOMPARE(authors[0].lastName(), QString("Пушкин"));
    QCOMPARE(authors[1].lastName(), QString("Толстой"));

    // Проверяем данные первого автора
    QCOMPARE(authors[1].firstName(), QString("Лев"));
    QCOMPARE(authors[1].surname(), QString("Николаевич"));
}

void TestGetAuthor::testGetByIdSuccess()
{
    try {
        Author author = Author::getById(m_testAuthorId);

        QCOMPARE(author.id(), m_testAuthorId);
        QCOMPARE(author.lastName(), QString("Толстой"));
        QCOMPARE(author.firstName(), QString("Лев"));
        QCOMPARE(author.surname(), QString("Николаевич"));
    } catch (const Author::NotFoundException&) {
        QFAIL("Author should be found");
    }
}

void TestGetAuthor::testGetByIdNotFound()
{
    try {
        Author::getById(-1); // Несуществующий ID
        QFAIL("Exception should be thrown");
    } catch (const Author::NotFoundException&) {
        // Ожидаемое поведение
        QVERIFY(true);
    } catch (...) {
        QFAIL("Wrong exception type");
    }
}

void TestGetAuthor::testGetEmptyTable()
{
    // Очищаем таблицу перед этим тестом
    clearTestData();

    QList<Author> authors = Author::get();
    QVERIFY(authors.isEmpty());
}
