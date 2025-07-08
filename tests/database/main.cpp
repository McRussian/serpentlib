#include <QtTest>
#include "test_base_model.h"
#include "test_author.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    int status = 0;

    {
        TestBaseModel test_case0;
        status |= QTest::qExec(&test_case0, argc, argv);
    }

    {
        TestAuthor test_case1;

        status |= QTest::qExec(&test_case1, argc, argv);
    }

    return status;
}
