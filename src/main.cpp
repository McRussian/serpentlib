#include <QApplication>

#include "gui/serpentlibwindow.h"
#include "settings_application.h"
#include "database/database.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SettingsApplication *settings = new SettingsApplication();
    DataBase *db = new DataBase();
    SerpentLibWindow w(settings, db);
    w.show();
    return a.exec();
}
