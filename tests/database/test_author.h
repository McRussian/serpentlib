#ifndef TEST_AUTHOR_H
#define TEST_AUTHOR_H

#include <QtTest>
#include <QSqlDatabase>
#include "../../src/database/models/author.h"

class TestAuthor : public QObject
{
    Q_OBJECT

private slots:
    // Выполняется перед всеми тестами
    void initTestCase();

    // Выполняется после всех тестов
    void cleanupTestCase();

    // Тестовые случаи
    void testCreateTable();
    void testSaveNewAuthor();
    void testUpdateAuthor();
    void testFieldValues();

private:
    QSqlDatabase m_testDb;
    const QString m_testDbPath = "test_author.db";
};

#endif // TEST_AUTHOR_H
