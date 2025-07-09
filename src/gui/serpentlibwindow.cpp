#include "serpentlibwindow.h"
#include "ui_serpentlibwindow.h"

SerpentLibWindow::SerpentLibWindow(SettingsApplication *setting, DataBase *db, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SerpentLibWindow)
    , settings_(setting)
    , db_(db)
{
    ui->setupUi(this);
    createMenus();

    updateMenuState(); // Первоначальное обновление состояния

    // Подключаем сигналы, которые могут изменить состояние
    connect(settings_, &SettingsApplication::databasePathChanged, this, &SerpentLibWindow::updateMenuState);

    if (settings_->getDatabasePath().isEmpty()) {
        QMessageBox::information(this, "SеrpentLib", "Нужно выбрать или создать базу данных");
    }
    else
    {
        try {
            db_->open(settings_->getDatabasePath());
        } catch (const DBException& e) {
            showError(e.message());
        }
    }
}

SerpentLibWindow::~SerpentLibWindow()
{
    delete ui;
}

void SerpentLibWindow::createMenus()
{
    // Меню "Настройки"
    settingsMenu = menuBar()->addMenu("Настройки");
    QMenu *dbSubMenu = settingsMenu->addMenu("База данных");

    QAction *createDBAction = new QAction("Создать", this);
    QAction *openDBAction = new QAction("Открыть", this);
    dbSubMenu->addAction(createDBAction);
    dbSubMenu->addAction(openDBAction);
    connect(createDBAction, &QAction::triggered, this, &SerpentLibWindow::createDatabase);
    connect(openDBAction, &QAction::triggered, this, &SerpentLibWindow::openDatabase);

    // Меню "Библиотека"
    libraryMenu = menuBar()->addMenu("Библиотека");

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

    libraryMenu->setEnabled(false);
}

void SerpentLibWindow::showError(QString msg)
{
    QErrorMessage err(this);
    err.showMessage(msg);
}

// Реализация слотов
void SerpentLibWindow::createDatabase() {
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Open DataBase"), ".", tr("DataBase Files (*.db *.sqlite)"));
    try {
        db_->create(filename);
        settings_->setDatabasePath(filename);
    } catch (const DBException& e) {
        showError(e.message());
    }
}

void SerpentLibWindow::openDatabase() {
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open DataBase"), ".", tr("DataBase Files (*.db *.sqlite)"));

    try {
        db_->open(filename);
        settings_->setDatabasePath(filename);
    } catch (const DBException& e) {
        showError(e.message());
    }
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
    AuthorSelectionWidget* selectAuthorWidget = new AuthorSelectionWidget();

    // Соединяем сигнал закрытия со слотом
    connect(selectAuthorWidget, &QWidget::destroyed, this, [this]() {
        qDebug() << "Виджет уничтожен";
    });

    // Показываем (не блокируя основной поток)
    selectAuthorWidget->show();
}

void SerpentLibWindow::updateMenuState()
{
    // Пример условия - база данных выбрана
    bool dbSelected = !settings_->getDatabasePath().isEmpty() and db_->is_open();

    // Активируем/деактивируем меню
    libraryMenu->setEnabled(dbSelected);
}



