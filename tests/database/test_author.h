#ifndef TEST_AUTHOR_H
#define TEST_AUTHOR_H

#include <QtTest>
#include "../../src/database/models/author.h"

class TestAuthor : public QObject
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

#endif // TEST_AUTHOR_H
