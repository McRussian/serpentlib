#include "serpentlibwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SerpentLibWindow w;
    w.show();
    return a.exec();
}
