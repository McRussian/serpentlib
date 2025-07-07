#include "serpentlibwindow.h"
#include "ui_serpentlibwindow.h"

SerpentLibWindow::SerpentLibWindow(SettingsApplication *setting, QWidget *parent)
    : settings_(setting)
    , QMainWindow(parent)
    , ui(new Ui::SerpentLibWindow)
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
        DataBase::instance(settings_->getDatabasePath()).database().setDatabaseName(settings_->getDatabasePath());
        Author::createTable();
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

// Реализация слотов
void SerpentLibWindow::createDatabase() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Open DataBase"), ".", tr("DataBase Files (*.db *.sqlite)"));
    DataBase::instance(fileName).database().setDatabaseName(fileName);
    Author::createTable();
    settings_->setDatabasePath(fileName);
}

void SerpentLibWindow::openDatabase() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open DataBase"), ".", tr("DataBase Files (*.db *.sqlite)"));
    DataBase::instance(fileName).database().setDatabaseName(fileName);
    if (Author::tableExists()) {
        if (!Author::verifyTableStructure()) {
            QErrorMessage errorDialog;
            errorDialog.showMessage("Неверный формат базы данных, создайте новую.");
            errorDialog.exec();
        }
    }
    else {
        Author::createTable();
    }
    settings_->setDatabasePath(fileName);
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
    bool dbSelected = !settings_->getDatabasePath().isEmpty();

    // Активируем/деактивируем меню
    libraryMenu->setEnabled(dbSelected);
}



