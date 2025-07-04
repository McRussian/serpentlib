#ifndef TEST_AUTHOR_H
#define TEST_AUTHOR_H

#include <QtTest>
#include "../../src/database/models/author.h"

class TestAuthorCorrectTable : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();     // Инициализация перед всеми тестами
    void cleanupTestCase();  // Очистка после всех тестов
    void testCreateTable();  // Тест создания таблицы
    void testSaveAuthor();   // Тест сохранения автора
    void testAuthorFields(); // Тест работы с полями

private:
    const QString TEST_DB = ":memory:"; // Используем временную базу в памяти
};


class TestAuthorCheckStructureTable : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();    // Выполняется перед всеми тестами
    void cleanupTestCase(); // Выполняется после всех тестов

    void testValidStructure();
    void testMissingTable();
    void testMissingField();
    void testTypeMismatch();
    void testExtraField();

private:
    QSqlDatabase m_db;
    void createValidTable();
    void dropTable();
};

#endif // TEST_AUTHOR_H
