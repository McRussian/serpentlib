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
    SerpentLibWindow(SettingsApplication* settings, QWidget *parent = nullptr);
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
    QMenu *settingsMenu;
    QMenu *libraryMenu;
    Ui::SerpentLibWindow *ui;
    SettingsApplication *settings_;
};
#endif // SERPENTLIBWINDOW_H
