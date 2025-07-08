#ifndef SERPENTLIBWINDOW_H
#define SERPENTLIBWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QErrorMessage>
#include <QFileDialog>

#include "author_form.h"
#include "author_selection_widget.h"
#include "../settings_application.h"
#include "../database/database.h"
#include "../database/dbexception.h"
#include "../database/models/author.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SerpentLibWindow;
}
QT_END_NAMESPACE

class SerpentLibWindow : public QMainWindow
{
    Q_OBJECT

public:
    SerpentLibWindow(SettingsApplication* settings, DataBase *db, QWidget *parent = nullptr);
    ~SerpentLibWindow();

private slots:
    void createDatabase();
    void openDatabase();
    void createBook();
    void searchBook();
    void createAuthor();
    void searchAuthor();

    void updateMenuState();

private:
    void createMenus();
    void showError(QString);
    QMenu *settingsMenu;
    QMenu *libraryMenu;
    Ui::SerpentLibWindow *ui;
    SettingsApplication *settings_;
    DataBase *db_;
};
#endif // SERPENTLIBWINDOW_H
