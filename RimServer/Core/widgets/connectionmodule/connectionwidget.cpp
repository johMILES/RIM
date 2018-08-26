#include "connectionwidget.h"
#include "ui_connectionwidget.h"

#include <QDebug>
#include <QHeaderView>
#include <QTimer>

#include "mode/connectionmonitormodel.h"
#include "Network/connection/tcpclient.h"

ConnectionWidget::ConnectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionWidget)
{
    ui->setupUi(this);

    initView();

    clientFreshTimer = new QTimer;
    clientFreshTimer->setInterval(2000);
    connect(clientFreshTimer,SIGNAL(timeout()),this,SLOT(udpateClientInfo()));
    clientFreshTimer->start();
}

ConnectionWidget::~ConnectionWidget()
{
    if(clientFreshTimer){
        if(clientFreshTimer->isActive())
            clientFreshTimer->stop();

        delete clientFreshTimer;
    }
    delete ui;
}

void ConnectionWidget::udpateClientInfo()
{
    ServerNetwork::TcpClientManager::ClientInfoList list = ServerNetwork::TcpClientManager::instance()->getAnchorPoint();
    ui->connectedNum->setText(QString::number(list.size()));
    tableModel->updateData(list);
}

void ConnectionWidget::initView()
{
    tableModel = new ConnectionMonitorModel;
    ui->connectTableView->setModel(tableModel);
    ui->connectTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->connectTableView->horizontalHeader()->setStretchLastSection(true);
    ui->connectTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->connectTableView->setSelectionMode(QAbstractItemView::SingleSelection);
}
