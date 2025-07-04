#include <QtTest>
#include "test_author.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    int status = 0;

    {
        TestAuthorCorrectTable tavt;

        status |= QTest::qExec(&tavt, argc, argv);
    }

    {
        TestAuthorCheckStructureTable tacst;
        status |= QTest::qExec(&tacst, argc, argv);
    }

    return status;
}
