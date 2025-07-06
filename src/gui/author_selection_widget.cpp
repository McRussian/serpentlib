#include "author_selection_widget.h"

AuthorSelectionWidget::AuthorSelectionWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedWidth(500);
    // Создаем таблицу
    m_table = new QTableWidget(this);
    m_table->setColumnCount(3);
    m_table->setHorizontalHeaderLabels({"", "ID", "Автор"});
    m_table->verticalHeader()->setVisible(false);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Настраиваем ширину столбцов
    m_table->setColumnWidth(0, 30);    // CheckBox
    m_table->setColumnHidden(1, true); // Скрываем столбец с ID
    m_table->setColumnWidth(2, 300);   // Имя автора

    // Фиксируем общую ширину таблицы
    m_table->setFixedWidth(350);

    // Создаем кнопки
    m_okButton = new QPushButton("OK", this);
    m_addButton = new QPushButton("Добавить", this);
    m_editButton = new QPushButton("Редактировать", this);
    m_searchButton = new QPushButton("Поиск", this);
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("Поиск авторов...");

    // Подключаем сигналы
    connect(m_okButton, &QPushButton::clicked, this, &AuthorSelectionWidget::onOkClicked);
    connect(m_addButton, &QPushButton::clicked, this, &AuthorSelectionWidget::onAddClicked);
    connect(m_editButton, &QPushButton::clicked, this, &AuthorSelectionWidget::onEditClicked);
    connect(m_searchButton, &QPushButton::clicked, this, &AuthorSelectionWidget::onSearchClicked);

    // Компоновка кнопок справа
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(m_okButton);
    rightLayout->addWidget(m_addButton);
    rightLayout->addWidget(m_editButton);
    rightLayout->addWidget(m_searchEdit);
    rightLayout->addWidget(m_searchButton);
    rightLayout->addStretch();

    // Основная компоновка
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(m_table);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    populateTable(Author::get());
}

void AuthorSelectionWidget::populateTable(const QVector<Author>& authors)
{
    m_table->setRowCount(authors.size());

    for (int i = 0; i < authors.size(); ++i) {
        const Author& author = authors[i];

        // Столбец 0: CheckBox
        QCheckBox *checkBox = new QCheckBox();
        checkBox->setChecked(false);
        m_table->setCellWidget(i, 0, checkBox);

        // Столбец 1: ID (скрытый)
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(author.id()));
        m_table->setItem(i, 1, idItem);

        // Столбец 2: Имя автора
        QTableWidgetItem *nameItem = new QTableWidgetItem(author.briefName());
        m_table->setItem(i, 2, nameItem);
    }
}

void AuthorSelectionWidget::onOkClicked()
{
    QVector<int> selectedIds;

    for (int i = 0; i < m_table->rowCount(); ++i) {
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(m_table->cellWidget(i, 0));
        if (checkBox && checkBox->isChecked()) {
            selectedIds.append(m_table->item(i, 1)->text().toInt());
        }
    }

    emit authorsSelected(selectedIds);
}

void AuthorSelectionWidget::onAddClicked()
{
    emit addAuthorRequested();
}

void AuthorSelectionWidget::onEditClicked()
{
    int authorId = getSelectedAuthorId();
    if (authorId != -1) {
        emit editAuthorRequested(authorId);
    } else {
        QMessageBox::warning(this, "Ошибка", "Выберите автора для редактирования");
    }
}

void AuthorSelectionWidget::onSearchClicked()
{
    emit searchRequested(m_searchEdit->text());
}

int AuthorSelectionWidget::getSelectedAuthorId() const
{
    int row = m_table->currentRow();
    if (row >= 0) {
        return m_table->item(row, 1)->text().toInt();
    }
    return -1;
}
