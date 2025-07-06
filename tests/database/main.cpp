#include <QtTest>
#include "test_author.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    int status = 0;

    {
        TestAuthorCorrectTable test_case1;

        status |= QTest::qExec(&test_case1, argc, argv);
    }

    {
        TestAuthorCheckStructureTable test_case2;
        status |= QTest::qExec(&test_case2, argc, argv);
    }

    {
        TestGetAuthor test_case3;
        status |= QTest::qExec(&test_case3, argc, argv);
    }

    return status;
}
