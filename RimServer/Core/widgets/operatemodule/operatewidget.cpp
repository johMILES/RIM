#include "operatewidget.h"
#include "ui_operatewidget.h"

#include <QScrollBar>

#include "Base/util/rsingleton.h"
#include "Base/util/rutil.h"
#include "messdispatch.h"

OperateWidget::OperateWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OperateWidget)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);

    ui->logEdit->setReadOnly(true);
    connect(RSingleton<MessDispatch>::instance(),SIGNAL(realRecord(QString)),this,SLOT(showRealRecord(QString)),Qt::QueuedConnection);
}

OperateWidget::~OperateWidget()
{
    delete ui;
}

QSize OperateWidget::sizeHint() const
{
    return QSize(300,800);
}

void OperateWidget::showRealRecord(QString content)
{
    ui->logEdit->append(QString("[%1]:%2").arg(RUtil::getTimeStamp()).arg(content));

    QScrollBar *scrollbar = ui->logEdit->verticalScrollBar();
    if (scrollbar)
        scrollbar->setSliderPosition(scrollbar->maximum());
}
