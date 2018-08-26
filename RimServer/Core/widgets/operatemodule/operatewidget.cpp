#include "operatewidget.h"
#include "ui_operatewidget.h"

OperateWidget::OperateWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OperateWidget)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
}

OperateWidget::~OperateWidget()
{
    delete ui;
}

QSize OperateWidget::sizeHint() const
{
    return QSize(300,800);
}
