#include "routetable.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QLabel>
#include <QWidget>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QHeaderView>
#include <QCursor>
#include <functional>

#include "routetableservermodel.h"
#include "routetableclientmodel.h"
#include "routetableclientdelegate.h"
#include "routetableserverdelegate.h"
#include "routetableview.h"
#include "routesettings.h"
#include "routetablehead.h"
#include "newservernode.h"
#include "newclientnode.h"

#include "Base/global.h"
#include "Base/constants.h"
#include "Base/file/xmlparse.h"
#include "Base/protocol/datastruct.h"
#include "Base/actionmanager/command.h"
#include "Base/actionmanager/actioncontainer.h"
#include "Base/actionmanager/actionmanager.h"
#include "Base/actionmanager/action.h"
#include "Base/util/rsingleton.h"

class RouteTablePrivate : public QObject
{
    Q_DECLARE_PUBLIC(RouteTable)
public:
    RouteTablePrivate(RouteTable * q):q_ptr(q),currRouteSettings(NULL),currServer(NULL){
        initView();
    }

    void initView();

private:
    RouteTable * q_ptr;

    QWidget * mainWidget;

    RouteTableServerModel * serverModel;
    RouteTableView * serverView;

    RouteTableView * clientView;
    RouteTableClientModel * clientModel;

    ParameterSettings::NodeServer * currServer;

    RouteSettings * currRouteSettings;
};

void RouteTablePrivate::initView()
{
    mainWidget = new QWidget();
    QSplitter * verticalSplitter = new QSplitter(Qt::Vertical);

    currRouteSettings = new RouteSettings;
    currRouteSettings->initSettings(RGlobal::G_RouteSettings);

    //Server
    QWidget * serverWidget = new QWidget(mainWidget);

    QLabel * serverLabel = new QLabel(serverWidget);
    serverLabel->setText(QObject::tr("Server node:"));

    RouteTableServerDelegate * serverDelegate = new RouteTableServerDelegate;
    serverModel = new RouteTableServerModel;
    serverModel->setRouteData(currRouteSettings);
    serverView = new RouteTableView(V_SERVER,serverWidget);
    serverView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    serverView->verticalHeader()->setVisible(false);

    serverView->setItemDelegate(serverDelegate);
    serverView->setModel(serverModel);

    QVBoxLayout * serverLayout = new QVBoxLayout;
    serverLayout->setContentsMargins(1,1,1,1);
    serverLayout->setSpacing(4);
    serverLayout->addWidget(serverLabel);
    serverLayout->addWidget(serverView);

    serverWidget->setLayout(serverLayout);

    //Client
    QWidget * clientWidget = new QWidget(mainWidget);

    QLabel * clientLabel = new QLabel(clientWidget);
    clientLabel->setText(QObject::tr("Client node:"));

    RouteTableClientDelegate * clientDelegate = new RouteTableClientDelegate;
    clientModel = new RouteTableClientModel;
    clientModel->setRouteData(currRouteSettings);
    clientView = new RouteTableView(V_CLIENT,clientWidget);
    clientView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    clientView->verticalHeader()->setVisible(false);

    clientView->setItemDelegate(clientDelegate);
    clientView->setModel(clientModel);

    QVBoxLayout * clientLayout = new QVBoxLayout;
    clientLayout->setContentsMargins(1,1,1,1);
    clientLayout->setSpacing(4);
    clientLayout->addWidget(clientLabel);
    clientLayout->addWidget(clientView);

    clientWidget->setLayout(clientLayout);

    verticalSplitter->addWidget(serverWidget);
    verticalSplitter->addWidget(clientWidget);

    connect(serverView,SIGNAL(clicked(QModelIndex)),q_ptr,SLOT(switchServerIndex(QModelIndex)));

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(4,4,4,4);
    layout->addWidget(verticalSplitter);
    mainWidget->setLayout(layout);
}


RouteTable::RouteTable(QWidget *parent) :
    DialogProxy(parent),d_ptr(new RouteTablePrivate(this))
{
    setButton(DialogProxy::Cancel|DialogProxy::Save);
    setContentWidget(d_ptr->mainWidget);

    setWindowTitle(tr("Route table"));

    initContextMenu();
}

RouteTable::~RouteTable()
{
    Q_D(RouteTable);
    if(d->currRouteSettings)
        delete d->currRouteSettings;

    ActionManager::instance()->releaseMenu(Constant::MENU_ROUTE_TABLE_SERVER);
    ActionManager::instance()->releaseMenu(Constant::MENU_ROUTE_TABLE_CLIENT);
}

QSize RouteTable::sizeHint() const
{
    return QSize(950,700);
}

void RouteTable::respButtClicked(DialogProxy::StandardButton butt)
{
    switch(butt){
        case DialogProxy::Cancel:
                cancelSave();
            break;
        case DialogProxy::Save:
                saveFile();
            break;
        default:
            break;
    }
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
    connect(clientNewAction,SIGNAL(triggered()),this,SLOT(addNewClientGroup()));
    Action * clientNew = ActionManager::instance()->registAction(Constant::ACTION_ROUTE_CLIENT_NEW,clientNewAction);

    QAction * clientMoveAction = new QAction(this);
    clientMoveAction->setText(tr("Move"));
    connect(clientMoveAction,SIGNAL(triggered()),this,SLOT(moveClientGroup()));
    Action * clientMove = ActionManager::instance()->registAction(Constant::ACTION_ROUTE_CLIENT_MOVE,clientMoveAction);

    QAction * clientDeleteAction = new QAction(this);
    clientDeleteAction->setText(tr("Delete"));
    connect(clientDeleteAction,SIGNAL(triggered()),this,SLOT(deleteClientGroup()));
    Action * clientDelete = ActionManager::instance()->registAction(Constant::ACTION_ROUTE_CLIENT_DELETE,clientDeleteAction);

    clientMenu->addAction(clientNew,Constant::MENU_ROUTE_TABLE_CLIENT);
    clientMenu->addAction(clientMove,Constant::MENU_ROUTE_TABLE_CLIENT);
    clientMenu->addAction(clientDelete,Constant::MENU_ROUTE_TABLE_CLIENT);
}

void RouteTable::switchServerIndex(QModelIndex index)
{
    Q_D(RouteTable);
    ParameterSettings::NodeServer * server = static_cast<ParameterSettings::NodeServer *>(index.internalPointer());
    if(server){
        d->currServer = server;
        d->clientModel->setCurrServer(server->nodeId);
    }else{
        d->currServer = NULL;
    }
}

/*!
 * @brief  服务器端添加新的分组
 */
void RouteTable::addNewServerGroup()
{
    NewServerNode serverNode;
    connect(&serverNode,SIGNAL(newServerNodeInfo(ParameterSettings::NodeServer *)),this,SLOT(createNewServerNode(ParameterSettings::NodeServer *)));
    serverNode.exec();
}

/*!
 * @brief  服务器端添删除分组
 */
void RouteTable::deleteServerGroup()
{
    Q_D(RouteTable);

    if(d->currServer == NULL)
        return;

    std::function<void()> resetTable = [&](){
        d->serverView->clearSelection();
        d->currServer = NULL;
        d->clientModel->setCurrServer(-1);
        d->clientView->clearSelection();
    };

    QModelIndex index = d->serverView->currIndex();
    if(index.isValid() && index.row() < d->currRouteSettings->settings()->servers.size()){
        int result = QMessageBox::question(this,tr("question"),tr("Are you sure you want to delete this items?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
        if(result == QMessageBox::Yes){
            ushort serverId = d->currRouteSettings->settings()->servers.keys().at(index.row());
            ParameterSettings::NodeServer * server = d->currRouteSettings->settings()->servers.value(serverId);
            if(server){
                std::for_each(server->clients.begin(),server->clients.end(),[&](ParameterSettings::NodeClient * const client){
                    d->currRouteSettings->settings()->clients.removeOne(client);
                    delete client;
                });
                delete server;
                d->currRouteSettings->settings()->servers.remove(serverId);
                d->serverModel->refreshData();
                resetTable();
            }
        }
    }
    else{
        resetTable();
    }
}

/*!
 * @brief  客户端添加分组
 */
void RouteTable::addNewClientGroup()
{
    Q_D(RouteTable);

    if(d->currServer == NULL){
        QMessageBox::warning(this,tr("warning"),tr("Please choose a server node!"));
        return;
    }

    NewClientNode clientNode;
    connect(&clientNode,SIGNAL(newClientNodeInfo(ParameterSettings::NodeClient *)),this,SLOT(createNewClientNode(ParameterSettings::NodeClient *)));
    clientNode.exec();
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
    Q_D(RouteTable);

    if(d->currServer == NULL)
        return;

    std::function<void()> resetTable = [&](){
        d->clientView->clearSelection();
    };

    QModelIndex index = d->clientView->currIndex();
    if(index.isValid() && index.row() < d->currServer->clients.size()){
        int result = QMessageBox::question(this,tr("question"),tr("Are you sure you want to delete this items?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
        if(result == QMessageBox::Yes){
            d->currRouteSettings->settings()->clients.removeOne(d->currServer->clients.at(index.row()));
            delete d->currServer->clients.at(index.row());
            d->currServer->clients.removeAt(index.row());
            d->clientModel->refreshData();
            resetTable();
        }
    }
    else{
        resetTable();
    }
}

/*!
 * @brief 将当前信息保存更新路由表
 */
void RouteTable::saveFile()
{
    Q_D(RouteTable);
    bool flag = RSingleton<XMLParse>::instance()->saveRouteSettings(qApp->property(Constant::LOCAL_ROUTE_CONFIG_FILE).toString(),d->currRouteSettings->settings());
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

/*!
 * @brief 创建服务器节点
 * @param[in] client 新服务器节点信息
 */
void RouteTable::createNewServerNode(ParameterSettings::NodeServer *server)
{
    Q_D(RouteTable);
    QMap<ushort,ParameterSettings::NodeServer *>::iterator iter = d->currRouteSettings->settings()->servers.begin();
    while(iter != d->currRouteSettings->settings()->servers.end()){
        if(iter.value()->nodeId == server->nodeId){
            QMessageBox::warning(this,tr("warning"),tr("Server node has existed!"));
            return;
        }
        iter++;
    }

    d->currRouteSettings->settings()->servers.insert(server->nodeId,server);
    d->serverModel->refreshData();
}

/*!
 * @brief 创建客户端节点
 * @param[in] client 新客户端节点信息
 */
void RouteTable::createNewClientNode(ParameterSettings::NodeClient * client)
{
    Q_D(RouteTable);

    if(d->currServer == NULL){
        QMessageBox::warning(this,tr("warning"),tr("Please choose a server node!"));
        return;
    }

    d->currServer->clients.append(client);

    d->clientModel->refreshData();
}
