#include "test_author.h"


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    TestAuthor tc;
    return QTest::qExec(&tc, argc, argv);
}
