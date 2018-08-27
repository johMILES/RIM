#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QDebug>

#include "actionmanager/actioncontainer.h"
#include "actionmanager/actionmanager.h"
#include "actionmanager/action.h"
#include "constants.h"
#include "../config/routetable.h"
#include "Util/rsingleton.h"
#include "pluginmanager/pluginmanager.h"

#include "baseinfomodule/baseinfodockpanel.h"
#include "connectionmodule/connectiondockpanel.h"
#include "operatemodule/operetepanel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    baseInfoPanel(nullptr),
    connectionPanel(nullptr),
    operatePanel(nullptr)
{
    ui->setupUi(this);

    setWindowTitle(tr("RimServer"));

    initMenu();
    initComponent();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMenu()
{
    ActionContainer *menubar = ActionManager::instance()->createMenuBar(Constant::MENU_BAR);

    setMenuBar(menubar->menuBar());
    menubar->appendGroup(Constant::G_PROGRAM);
    menubar->appendGroup(Constant::G_SETTING);
    menubar->appendGroup(Constant::G_HELP);

    //程序菜单
    ActionContainer * serverMenu = ActionManager::instance()->createMenu(Constant::G_PROGRAM);
    serverMenu->appendGroup(Constant::G_PROGRAM);
    menubar->addMenu(serverMenu, Constant::G_PROGRAM);
    serverMenu->menu()->setTitle(tr("&Program"));

    QAction * deleteAction = new QAction(QIcon(), tr("&Exit"), this);
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(programExit()));
    Action * serverDelete = ActionManager::instance()->registAction(Constant::PROGRAM_EXIT,deleteAction);

    serverMenu->addAction(serverDelete,Constant::G_PROGRAM);

    //设置菜单
    ActionContainer * settingsMenu = ActionManager::instance()->createMenu(Constant::G_SETTING);
    settingsMenu->appendGroup(Constant::G_SETTING);
    menubar->addMenu(settingsMenu, Constant::G_SETTING);
    settingsMenu->menu()->setTitle(tr("&Settings"));

    QAction * topHintAction = new QAction(QIcon(),tr("Top hint"),this);
    topHintAction->setCheckable(true);
    connect(topHintAction,SIGNAL(triggered(bool)),this,SLOT(windowTopHint(bool)));
    Action * topHint = ActionManager::instance()->registAction(Constant::TOP_HINT,topHintAction);
    settingsMenu->addAction(topHint,Constant::G_SETTING);

    QAction * fullScreenAction = new QAction(QIcon(),tr("Full screen"),this);
    fullScreenAction->setCheckable(true);
    connect(fullScreenAction,SIGNAL(triggered(bool)),this,SLOT(windowFullScreen(bool)));
    Action * fullScreen = ActionManager::instance()->registAction(Constant::FULL_SCREEN,fullScreenAction);
    settingsMenu->addAction(fullScreen,Constant::G_SETTING);

    settingsMenu->addSeparator(Constant::G_SETTING);

    QAction * routeTableAction = new QAction(QIcon(), tr("&Route table settings"), this);
    Action * serverNew = ActionManager::instance()->registAction(Constant::ROUTE_TABLE_SETTING,routeTableAction);
    connect(routeTableAction,SIGNAL(triggered()),this,SLOT(routeTableEdit()));
    settingsMenu->addAction(serverNew,Constant::G_SETTING);

    QAction * seriPortAction = new QAction(QIcon(),tr("&Seriport settings"),this);
    Action * seriPort = ActionManager::instance()->registAction(Constant::SERIPORT_SETTING,seriPortAction);
    connect(seriPortAction,SIGNAL(triggered()),this,SLOT(seriportEditWindow()));
    settingsMenu->addAction(seriPort,Constant::G_SETTING);

    settingsMenu->addSeparator(Constant::G_SETTING);

    //帮助菜单
    ActionContainer * helpMenu = ActionManager::instance()->createMenu(Constant::G_HELP);
    helpMenu->appendGroup(Constant::G_HELP);
    menubar->addMenu(helpMenu, Constant::G_HELP);
    helpMenu->menu()->setTitle(tr("&Help"));

    QAction * supportAction = new QAction(QIcon(),tr("Technical support"),this);
    connect(supportAction,SIGNAL(triggered()),this,SLOT(technicalSupport()));
    Action * support = ActionManager::instance()->registAction(Constant::TEC_SUPPORT,supportAction);
    helpMenu->addAction(support,Constant::G_HELP);

    helpMenu->addSeparator(Constant::G_HELP);

    QAction * aboutPorgramAction = new QAction(QIcon(),tr("About program"),this);
    connect(aboutPorgramAction,SIGNAL(triggered()),this,SLOT(aboutProgram()));
    Action * aboutProgram = ActionManager::instance()->registAction(Constant::ABOUT_PROGRAM,aboutPorgramAction);
    helpMenu->addAction(aboutProgram,Constant::G_HELP);

}

/*!
 * @brief   路由表编辑
 */
void MainWindow::routeTableEdit()
{
    RouteTable table(this);
    table.exec();
}

/*!
 * @brief   程序退出
 */
void MainWindow::programExit()
{
    exit(0);
}

/*!
 * @brief   程序置顶
 */
void MainWindow::windowTopHint(bool flag)
{
    Qt::WindowFlags flags = windowFlags();
    if(flag)
        flags |= Qt::WindowStaysOnTopHint;
    else
        flags = flags & ~Qt::WindowStaysOnTopHint;

    setWindowFlags(flags);
    show();
}

void MainWindow::windowFullScreen(bool)
{
    Qt::WindowStates state = windowState();
    if(!isFullScreen())
        state |= Qt::WindowFullScreen;
    else
        state = state & ~Qt::WindowFullScreen;

    setWindowState(state);
}

void MainWindow::seriportEditWindow()
{

}

void MainWindow::technicalSupport()
{

}

void MainWindow::aboutProgram()
{

}

/*!
 * @brief   初始化各个组件
 */
void MainWindow::initComponent()
{
    setDockNestingEnabled(true);
    QWidget * centeral = takeCentralWidget();
    if (centeral)
        delete centeral;

    //TODO 20180824 待从各个dll中读取
    baseInfoPanel = new BaseInfoDockPanel;
    connectionPanel = new ConnectionPanel;
    operatePanel = new OperetePanel;

    //TODO 20180824 考虑布局
    addDockWidget(Qt::LeftDockWidgetArea,baseInfoPanel);
    splitDockWidget(baseInfoPanel,operatePanel,Qt::Vertical);
    addDockWidget(Qt::RightDockWidgetArea,connectionPanel);

    RSingleton<PluginManager>::instance()->addPlugin(baseInfoPanel);
    RSingleton<PluginManager>::instance()->addPlugin(connectionPanel);
    RSingleton<PluginManager>::instance()->addPlugin(operatePanel);

    RSingleton<PluginManager>::instance()->load();
    PluginManager::ComponentMap maps = RSingleton<PluginManager>::instance()->plugins();

    PluginManager::ComponentMap::iterator iter = maps.begin();
    while(iter != maps.end()){
        RComponent * comp = iter.value();
        comp->setFeatures(QDockWidget::DockWidgetMovable);
        comp->setWindowTitle(comp->name());
        comp->initialize();

        iter++;
    }
}
