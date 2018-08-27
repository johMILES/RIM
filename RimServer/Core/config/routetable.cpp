#include "routetable.h"
#include "ui_routetable.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QLabel>
#include <QWidget>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QMessageBox>

#include "routetableservermodel.h"
#include "routetableclientmodel.h"
#include "routetableview.h"
#include "../protocol/datastruct.h"
#include "routetableclientdelegate.h"
#include "routetableserverdelegate.h"
#include "routesettings.h"
#include "../global.h"
#include "../widgets/actionmanager/command.h"
#include "../widgets/actionmanager/actioncontainer.h"
#include "../widgets/actionmanager/actionmanager.h"
#include "../widgets/actionmanager/action.h"
#include "Util/rsingleton.h"
#include "../constants.h"
#include "routetablehead.h"
#include "../file/xmlparse.h"

RouteTable::RouteTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RouteTable),currRouteSettings(NULL)
{
    ui->setupUi(this);

    setWindowTitle(tr("Route table"));

    Qt::WindowFlags  flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    initContextMenu();
    initView();

    connect(ui->confirm,SIGNAL(pressed()),this,SLOT(saveFile()));
    connect(ui->cancel,SIGNAL(pressed()),this,SLOT(cancelSave()));
}

RouteTable::~RouteTable()
{
    if(currRouteSettings)
        delete currRouteSettings;

    ActionManager::instance()->releaseMenu(Constant::MENU_ROUTE_TABLE_SERVER);
    ActionManager::instance()->releaseMenu(Constant::MENU_ROUTE_TABLE_CLIENT);

    delete ui;
}

void RouteTable::initView()
{
    currRouteSettings = new RouteSettings;
    currRouteSettings->initSettings(RGlobal::G_RouteSettings);

    //Server
    QWidget * serverWidget = new QWidget(ui->widget_3);

    QLabel * serverLabel = new QLabel(serverWidget);
    serverLabel->setText(tr("Server node:"));

    RouteTableServerDelegate * serverDelegate = new RouteTableServerDelegate;
    serverModel = new RouteTableServerModel;
    serverModel->setRouteData(currRouteSettings);
    serverView = new RouteTableView(V_SERVER);
    serverView->horizontalHeader()->resizeSection(0,120);
    serverView->horizontalHeader()->resizeSection(1,150);

    serverView->setItemDelegate(serverDelegate);
    serverView->setModel(serverModel);

    QVBoxLayout * serverLayout = new QVBoxLayout;
    serverLayout->setContentsMargins(1,1,1,1);
    serverLayout->setSpacing(4);
    serverLayout->addWidget(serverLabel);
    serverLayout->addWidget(serverView);

    serverWidget->setLayout(serverLayout);

    //Client
    QWidget * clientWidget = new QWidget(ui->widget_3);

    QLabel * clientLabel = new QLabel(clientWidget);
    clientLabel->setText(tr("Client node:"));

    RouteTableClientDelegate * clientDelegate = new RouteTableClientDelegate;
    clientModel = new RouteTableClientModel;
    clientModel->setRouteData(currRouteSettings);
    clientView = new RouteTableView(V_CLIENT);

    clientView->setItemDelegate(clientDelegate);
    clientView->setModel(clientModel);

    QVBoxLayout * clientLayout = new QVBoxLayout;
    clientLayout->setContentsMargins(1,1,1,1);
    clientLayout->setSpacing(4);
    clientLayout->addWidget(clientLabel);
    clientLayout->addWidget(clientView);

    clientWidget->setLayout(clientLayout);

    QSplitter * verticalSplitter = new QSplitter(Qt::Vertical);
    verticalSplitter->addWidget(serverWidget);
    verticalSplitter->addWidget(clientWidget);

    connect(serverView,SIGNAL(clicked(QModelIndex)),this,SLOT(switchServerIndex(QModelIndex)));

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(4,4,4,4);
    layout->addWidget(verticalSplitter);
    ui->widget_3->setLayout(layout);

    ui->cancel->setText(tr("Cancel"));
    ui->confirm->setText(tr("Save"));
}

void RouteTable::initContextMenu()
{
    ActionContainer * serverMenu = ActionManager::instance()->createMenu(Constant::MENU_ROUTE_TABLE_SERVER);
    serverMenu->appendGroup(Constant::MENU_ROUTE_TABLE_SERVER);

    QAction * serverNewAction = new QAction(this);
    serverNewAction->setText(tr("New"));
    connect(serverNewAction,SIGNAL(triggered()),this,SLOT(addNewServerGroup()));
    Action * serverNew = ActionManager::instance()->registAction(Constant::ACTION_ROUTE_SERVER_NEW,serverNewAction);

    QAction * serverDeleteAction = new QAction(this);
    serverDeleteAction->setText(tr("Delete"));
    connect(serverDeleteAction,SIGNAL(triggered()),this,SLOT(deleteServerGroup()));
    Action * serverDelete = ActionManager::instance()->registAction(Constant::ACTION_ROUTE_SERVER_DELETE,serverDeleteAction);

    serverMenu->addAction(serverNew,Constant::MENU_ROUTE_TABLE_SERVER);
    serverMenu->addAction(serverDelete,Constant::MENU_ROUTE_TABLE_SERVER);

    ActionContainer * clientMenu = ActionManager::instance()->createMenu(Constant::MENU_ROUTE_TABLE_CLIENT);
    clientMenu->appendGroup(Constant::MENU_ROUTE_TABLE_CLIENT);

    QAction * clientNewAction = new QAction(this);
    clientNewAction->setText(tr("New"));
    Action * clientNew = ActionManager::instance()->registAction(Constant::ACTION_ROUTE_CLIENT_NEW,clientNewAction);

    QAction * clientMoveAction = new QAction(this);
    clientMoveAction->setText(tr("Move"));
    Action * clientMove = ActionManager::instance()->registAction(Constant::ACTION_ROUTE_CLIENT_MOVE,clientMoveAction);

    QAction * clientDeleteAction = new QAction(this);
    clientDeleteAction->setText(tr("Delete"));
    Action * clientDelete = ActionManager::instance()->registAction(Constant::ACTION_ROUTE_CLIENT_DELETE,clientDeleteAction);

    clientMenu->addAction(clientNew,Constant::MENU_ROUTE_TABLE_CLIENT);
    clientMenu->addAction(clientMove,Constant::MENU_ROUTE_TABLE_CLIENT);
    clientMenu->addAction(clientDelete,Constant::MENU_ROUTE_TABLE_CLIENT);
}

void RouteTable::switchServerIndex(QModelIndex index)
{
    ParameterSettings::NodeServer * server = static_cast<ParameterSettings::NodeServer *>(index.internalPointer());
    if(server){
       clientModel->setCurrServer(server->nodeId);
    }
}

/*!
 * @brief  服务器端添加新的分组
 */
void RouteTable::addNewServerGroup()
{

}

/*!
 * @brief  服务器端添删除分组
 */
void RouteTable::deleteServerGroup()
{

}

/*!
 * @brief  客户端添加分组
 */
void RouteTable::addNewClientGroup()
{

}

/*!
 * @brief  移动客户端分组至其它服务器节点
 */
void RouteTable::moveClientGroup()
{

}

/*!
 * @brief  客户端删除分组
 */
void RouteTable::deleteClientGroup()
{

}

/*!
 * @brief 将当前信息保存更新路由表
 */
void RouteTable::saveFile()
{
    bool flag = RSingleton<XMLParse>::instance()->saveRouteSettings(qApp->property(Constant::LOCAL_ROUTE_CONFIG_FILE).toString(),currRouteSettings->settings());
    if(flag){
        QMessageBox::information(this,tr("information"),tr("Save successfully! Modifications will take effect after reboot."),QMessageBox::Yes);
    }else{
        QMessageBox::information(this,tr("information"),tr("Save failed!"),QMessageBox::Yes);
    }
}

void RouteTable::cancelSave()
{
    int result = QMessageBox::information(this,tr("inforamtion"),tr("This action will discard the modification,Whether to continue!"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(result == QMessageBox::No)
        return;
    close();
}
