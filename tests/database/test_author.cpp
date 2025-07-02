#include "test_author.h"

void TestAuthor::initTestCase()
{
    // Создаем тестовую БД в памяти (или файле)
    m_testDb = QSqlDatabase::addDatabase("QSQLITE", "test_connection");
    m_testDb.setDatabaseName(":memory:"); // или ":memory:" для временной БД

    if (!m_testDb.open()) {
        QFAIL("Failed to open test database");
    }

    Author::setDatabase(&m_testDb);
}

void TestAuthor::cleanupTestCase()
{
    m_testDb.close();
    QSqlDatabase::removeDatabase("test_connection");
    QFile::remove(":memory:"); // Удаляем тестовый файл БД
}

void TestAuthor::testCreateTable()
{
    QVERIFY(Author::createTable());

    // Проверяем, что таблица существует
    QSqlQuery query(m_testDb);
    QVERIFY(query.exec("SELECT id, first_name, last_name FROM authors"));
}

void TestAuthor::testSaveNewAuthor()
{
    Author author;
    author.setFirstName("Антон");
    author.setLastName("Чехов");
    author.setSurname("Павлович");
    author.setComment("Русский писатель");

    QVERIFY(author.save());
    QVERIFY(author.id() > 0); // Проверяем, что ID установлен

    // Проверяем запись в БД
    QSqlQuery query(m_testDb);
    QVERIFY(query.exec(QString("SELECT * FROM authors WHERE id = %1").arg(author.id())));
    QVERIFY(query.next());

    QCOMPARE(query.value("first_name").toString(), QString("Антон"));
    QCOMPARE(query.value("last_name").toString(), QString("Чехов"));
}

void TestAuthor::testUpdateAuthor()
{
    Author author;
    author.setLastName("Достоевский");
    author.setFirstName("Фёдор");
    QVERIFY(author.save());

    unsigned int id = author.id();
    author.setComment("Автор 'Преступления и наказания'");
    QVERIFY(author.save());

    // Проверяем обновление
    QSqlQuery query(m_testDb);
    QVERIFY(query.exec(QString("SELECT comment FROM authors WHERE id = %1").arg(id)));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toString(), QString("Автор 'Преступления и наказания'"));
}

void TestAuthor::testFieldValues()
{
    Author author;

    // Проверка сеттеров/геттеров
    author.setFirstName("  Лев  ");
    author.setLastName(" Толстой ");
    author.setSurname("Николаевич");

    QCOMPARE(author.firstName(), QString("  Лев  "));
    QCOMPARE(author.lastName(), QString(" Толстой "));
    QCOMPARE(author.surname(), QString("Николаевич"));

    // Проверка начального ID
    QCOMPARE(author.id(), 0u);
}
