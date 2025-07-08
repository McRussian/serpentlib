#ifndef TEST_AUTHOR_H
#define TEST_AUTHOR_H

#include <QtTest>
#include "../../src/database/models/author.h"


class TestAuthor : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testEmpty();
    void testGetters();
    void testSetters();
    void testBriefName();
    void testFullName();
    void testToMap();
    void testFromMap();

private:
    Author* m_author;
};


#endif // TEST_AUTHOR_H
