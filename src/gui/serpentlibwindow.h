#ifndef SERPENTLIBWINDOW_H
#define SERPENTLIBWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class SerpentLibWindow;
}
QT_END_NAMESPACE

class SerpentLibWindow : public QMainWindow
{
    Q_OBJECT

public:
    SerpentLibWindow(QWidget *parent = nullptr);
    ~SerpentLibWindow();

private slots:
    void createDatabase();
    void openDatabase();
    void createBook();
    void searchBook();
    void createAuthor();
    void searchAuthor();

private:
    void createMenus();

    Ui::SerpentLibWindow *ui;

};
#endif // SERPENTLIBWINDOW_H
