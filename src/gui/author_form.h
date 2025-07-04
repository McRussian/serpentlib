#ifndef AUTHOR_FORM_H
#define AUTHOR_FORM_H

#include <QWidget>

#include "../lib/field.h"
#include "../database/models/author.h"


namespace Ui {
class AuthorForm;
}

class AuthorForm : public QWidget
{
    Q_OBJECT

public:
    explicit AuthorForm(QWidget *parent = nullptr);
    ~AuthorForm();

private slots:
    void handleButtonOK();
    void handleButtonCancel();

private:
    Ui::AuthorForm *ui;
};

#endif // AUTHOR_FORM_H
