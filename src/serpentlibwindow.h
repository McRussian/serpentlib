#ifndef SERPENTLIBWINDOW_H
#define SERPENTLIBWINDOW_H

#include <QMainWindow>

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

private:
    Ui::SerpentLibWindow *ui;
};
#endif // SERPENTLIBWINDOW_H
