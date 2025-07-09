#ifndef TEST_DATABASE_HOOK_H
#define TEST_DATABASE_HOOK_H

#include <QtTest>
#include "../../src/database/database.h"

class TestDatabaseHooks : public QObject
{
    Q_OBJECT

public:
    TestDatabaseHooks();
    ~TestDatabaseHooks();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testInsertHook();
    void testUpdateHook();
    void testDeleteHook();
    void testMultipleOperations();
    void testInvalidOperations();

private:
    DataBase* m_db;
    QString m_testTable;
    int m_signalCounter;
    QString m_lastTable;
    DataBase::ChangeType m_lastOperation;

private slots:
    void handleTableChange(const QString& tableName, DataBase::ChangeType operation);
};

#endif // TEST_DATABASE_HOOK_H
