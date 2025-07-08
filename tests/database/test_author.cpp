#include "test_author.h"

void TestAuthor::initTestCase()
{
    m_author = new Author();
    m_author->setId(1);
    m_author->setFirstName("Иван");
    m_author->setLastName("Петров");
    m_author->setSurname("Сергеевич");
    m_author->setComment("Тестовый автор");
}

void TestAuthor::cleanupTestCase()
{
    delete m_author;
}

void TestAuthor::testEmpty()
{
    Author* author = new Author();
    QCOMPARE(author->id(), 0);
    QCOMPARE(author->firstName(), QString(""));
    QCOMPARE(author->lastName(), QString(""));
    QCOMPARE(author->surname(), QString(""));
    QCOMPARE(author->comment(), QString(""));
    delete author;
}

void TestAuthor::testGetters()
{
    QCOMPARE(m_author->id(), 1u);
    QCOMPARE(m_author->firstName(), QString("Иван"));
    QCOMPARE(m_author->lastName(), QString("Петров"));
    QCOMPARE(m_author->surname(), QString("Сергеевич"));
    QCOMPARE(m_author->comment(), QString("Тестовый автор"));
}

void TestAuthor::testSetters()
{
    m_author->setId(2);
    QCOMPARE(m_author->id(), 2u);

    m_author->setFirstName("Алексей");
    QCOMPARE(m_author->firstName(), QString("Алексей"));

    m_author->setLastName("Сидоров");
    QCOMPARE(m_author->lastName(), QString("Сидоров"));

    m_author->setSurname("Иванович");
    QCOMPARE(m_author->surname(), QString("Иванович"));

    m_author->setComment("Другой комментарий");
    QCOMPARE(m_author->comment(), QString("Другой комментарий"));

    // Возвращаем исходные значения
    m_author->setId(1);
    m_author->setFirstName("Иван");
    m_author->setLastName("Петров");
    m_author->setSurname("Сергеевич");
    m_author->setComment("Тестовый автор");
}

void TestAuthor::testBriefName()
{
    QCOMPARE(m_author->briefName(), QString("Петров И. С."));

    // Проверка без отчества
    Author authorWithoutSurname;
    authorWithoutSurname.setLastName("Сидоров");
    authorWithoutSurname.setFirstName("Алексей");
    QCOMPARE(authorWithoutSurname.briefName(), QString("Сидоров А."));
}

void TestAuthor::testFullName()
{
    QCOMPARE(m_author->fullName(), QString("Петров Иван Сергеевич"));

    // Проверка без отчества
    Author authorWithoutSurname;
    authorWithoutSurname.setLastName("Сидоров");
    authorWithoutSurname.setFirstName("Алексей");
    QCOMPARE(authorWithoutSurname.fullName(), QString("Сидоров Алексей"));
}

void TestAuthor::testToMap()
{
    QMap<QString, QVariant> expectedMap = {
        {"id", 1},
        {"last_name", "Петров"},
        {"first_name", "Иван"},
        {"surname", "Сергеевич"},
        {"comment", "Тестовый автор"}
    };

    QCOMPARE(m_author->toMap(), expectedMap);
}

void TestAuthor::testFromMap()
{
    QMap<QString, QVariant> testMap = {
        {"id", 5},
        {"last_name", "Иванов"},
        {"first_name", "Сергей"},
        {"surname", "Петрович"},
        {"comment", "Новый автор"}
    };

    Author testAuthor;
    testAuthor.fromMap(testMap);

    QCOMPARE(testAuthor.id(), 5u);
    QCOMPARE(testAuthor.lastName(), QString("Иванов"));
    QCOMPARE(testAuthor.firstName(), QString("Сергей"));
    QCOMPARE(testAuthor.surname(), QString("Петрович"));
    QCOMPARE(testAuthor.comment(), QString("Новый автор"));
}
