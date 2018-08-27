#include "databaseedit.h"
#include "ui_databaseedit.h"

DatabaseEdit::DatabaseEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseEdit)
{
    ui->setupUi(this);

    Qt::WindowFlags  flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    setWindowTitle(tr("Database info Setting"));

    connect(ui->cancel,SIGNAL(pressed()),this,SLOT(cancel()));
    connect(ui->confirm,SIGNAL(pressed()),this,SLOT(confirm()));
}

DatabaseEdit::~DatabaseEdit()
{
    delete ui;
}

void DatabaseEdit::cancel()
{

}

void DatabaseEdit::confirm()
{

}
