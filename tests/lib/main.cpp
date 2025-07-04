#include <QtTest>
#include "test_normalize_string.h"
#include "test_normilize_text.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    int status = 0;

    {
        TestNormalizeName testCase1;
        status |= QTest::qExec(&testCase1, argc, argv);
    }


    {
        TestNormalizeText testCase2;
        status |= QTest::qExec(&testCase2, argc, argv);
    }

    return status;
}
