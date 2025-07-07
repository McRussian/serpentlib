#include "gui/serpentlibwindow.h"
#include "settings_application.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SettingsApplication *settings = new SettingsApplication();
    SerpentLibWindow w(settings);
    w.show();
    return a.exec();
}
