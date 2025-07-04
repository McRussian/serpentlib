#include "author_form.h"
#include "ui_author_form.h"

AuthorForm::AuthorForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AuthorForm)
{
    ui->setupUi(this);
    connect(ui->btn_ok, &QPushButton::clicked, this, &AuthorForm::handleButtonOK);
    connect(ui->btn_cancel, &QPushButton::clicked, this, &AuthorForm::handleButtonCancel);
}

AuthorForm::~AuthorForm()
{
    delete ui;
}

void AuthorForm::handleButtonOK()
{
    Author author;
    author.setFirstName(normalizeName(ui->led_firstname->text()));
    author.setLastName(normalizeName(ui->led_lastname->text()));
    author.setSurname(normalizeName(ui->led_surname->text()));
    author.setComment("");

    author.save();
    close();
}

void AuthorForm::handleButtonCancel()
{
    close();
}
