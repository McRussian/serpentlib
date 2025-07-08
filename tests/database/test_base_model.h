#ifndef TESTBASEMODEL_H
#define TESTBASEMODEL_H

#include <QtTest>
#include "test_model.h"

class TestBaseModel : public QObject
{
    Q_OBJECT

public:
    TestBaseModel();
    ~TestBaseModel();

private slots:
    // Тесты инициализации
    void initTestCase();
    void cleanupTestCase();

    // Тесты работы с таблицей
    void testCreateTable();
    void testTableExists();
    void testValidateTable();

    // Тесты CRUD операций
    void testSaveNewObject();
    void testUpdateObject();
    void testLoadObject();
    void testRemoveObject();
    void testGetAllObjects();

    // Тесты граничных случаев
    void testInvalidIdOperations();
    void testDoubleSaveProtection();

private:
    QSqlDatabase m_db;
    void recreateTestTable();
};

#endif // TESTBASEMODEL_H
