#ifndef AUTHORSELECTIONWIDGET_H
#define AUTHORSELECTIONWIDGET_H

#include <QWidget>
#include <QVector>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QCheckBox>
#include <QDebug>
#include "../database/models/author.h"


class AuthorSelectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AuthorSelectionWidget(QWidget *parent = nullptr);

signals:
    void authorsSelected(const QVector<int>& selectedAuthorIds);
    void addAuthorRequested();
    void editAuthorRequested(int authorId);
    void searchRequested(const QString& searchTerm);

private slots:
    void onOkClicked();
    void onAddClicked();
    void onEditClicked();
    void onSearchClicked();

private:
    void populateTable(const QVector<Author>& authors);

    QTableWidget *m_table;
    QPushButton *m_okButton;
    QPushButton *m_addButton;
    QPushButton *m_editButton;
    QPushButton *m_searchButton;
    QLineEdit *m_searchEdit;
    int getSelectedAuthorId() const;
};

#endif // AUTHORSELECTIONWIDGET_H
