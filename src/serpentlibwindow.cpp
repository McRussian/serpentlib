#include "serpentlibwindow.h"
#include "ui_serpentlibwindow.h"

SerpentLibWindow::SerpentLibWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SerpentLibWindow)
{
    ui->setupUi(this);
}

SerpentLibWindow::~SerpentLibWindow()
{
    delete ui;
}
