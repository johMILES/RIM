#include "baseinfoedit.h"
#include "ui_baseinfoedit.h"

BaseInfoEdit::BaseInfoEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BaseInfoEdit)
{
    ui->setupUi(this);

    Qt::WindowFlags  flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    setWindowTitle(tr("Baseinfo Setting"));

    connect(ui->cancel,SIGNAL(pressed()),this,SLOT(cancel()));
    connect(ui->confirm,SIGNAL(pressed()),this,SLOT(confirm()));
}

BaseInfoEdit::~BaseInfoEdit()
{
    delete ui;
}

void BaseInfoEdit::cancel()
{

}

void BaseInfoEdit::confirm()
{

}
