#include <QtTest>
#include "test_normalize_string.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    TestNormalizeName tm;
    return QTest::qExec(&tm, argc, argv);
}
