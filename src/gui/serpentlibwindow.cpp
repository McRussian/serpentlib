#include "serpentlibwindow.h"
#include "ui_serpentlibwindow.h"

SerpentLibWindow::SerpentLibWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SerpentLibWindow)
{
    ui->setupUi(this);
    createMenus();
}

SerpentLibWindow::~SerpentLibWindow()
{
    delete ui;
}

void SerpentLibWindow::createMenus()
{
    // Меню "Настройки"
    QMenu *settingsMenu = menuBar()->addMenu("Настройки");
    QMenu *dbSubMenu = settingsMenu->addMenu("База данных");

    QAction *createDBAction = new QAction("Создать", this);
    QAction *openDBAction = new QAction("Открыть", this);
    dbSubMenu->addAction(createDBAction);
    dbSubMenu->addAction(openDBAction);
    connect(createDBAction, &QAction::triggered, this, &SerpentLibWindow::createDatabase);
    connect(openDBAction, &QAction::triggered, this, &SerpentLibWindow::openDatabase);

    // Меню "Библиотека"
    QMenu *libraryMenu = menuBar()->addMenu("Библиотека");

    QMenu *booksMenu = libraryMenu->addMenu("Книги");
    QAction *createBookAction = new QAction("Создать", this);
    QAction *searchBookAction = new QAction("Поиск", this);
    booksMenu->addAction(createBookAction);
    booksMenu->addAction(searchBookAction);
    connect(createBookAction, &QAction::triggered, this, &SerpentLibWindow::createBook);
    connect(searchBookAction, &QAction::triggered, this, &SerpentLibWindow::searchBook);

    QMenu *authorsMenu = libraryMenu->addMenu("Авторы");
    QAction *createAuthorAction = new QAction("Создать", this);
    QAction *searchAuthorAction = new QAction("Поиск", this);
    authorsMenu->addAction(createAuthorAction);
    authorsMenu->addAction(searchAuthorAction);
    connect(createAuthorAction, &QAction::triggered, this, &SerpentLibWindow::createAuthor);
    connect(searchAuthorAction, &QAction::triggered, this, &SerpentLibWindow::searchAuthor);
}

// Реализация слотов
void SerpentLibWindow::createDatabase() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Open DataBase"), ".", tr("DataBase Files (*.db *.sqlite)"));
    DataBase::instance(fileName).database().setDatabaseName(fileName);
    Author::createTable();
}

void SerpentLibWindow::openDatabase() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open DataBase"), ".", tr("DataBase Files (*.db *.sqlite)"));
    DataBase::instance(fileName).database().setDatabaseName(fileName);
}

void SerpentLibWindow::createBook() {
    QMessageBox::information(this, "Книги", "Создание новой книги");
}

void SerpentLibWindow::searchBook() {
    QMessageBox::information(this, "Книги", "Поиск книг");
}

void SerpentLibWindow::createAuthor() {
    AuthorForm *childWidget = new AuthorForm();

    // Соединяем сигнал закрытия со слотом
    connect(childWidget, &QWidget::destroyed, this, [this]() {
        qDebug() << "Виджет уничтожен";
    });

    // Показываем (не блокируя основной поток)
    childWidget->show();
}

void SerpentLibWindow::searchAuthor() {
    QMessageBox::information(this, "Авторы", "Поиск авторов");
}



