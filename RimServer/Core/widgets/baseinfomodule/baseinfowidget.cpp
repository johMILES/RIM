#include "baseinfowidget.h"
#include "ui_baseinfowidget.h"

#include <QAction>
#include <QDir>

#include "Base/protocol/datastruct.h"
#include "Base/file/globalconfigfile.h"
#include "Base/global.h"
#include "Base/sql/databasemanager.h"

#include "Base/actionmanager/action.h"
#include "Base/actionmanager/actioncontainer.h"
#include "Base/actionmanager/actionmanager.h"
#include "Base/constants.h"

#include "baseinfoedit.h"
#include "databaseedit.h"
#include "baseinfoconstant.h"

BaseInfoWidget::BaseInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseInfoWidget)
{
    ui->setupUi(this);

    initView();
    updateBaseInfo();
    updateDatabaseInfo();
}

BaseInfoWidget::~BaseInfoWidget()
{
    delete ui;
}

void BaseInfoWidget::initView()
{
    ActionContainer * settingMenu = ActionManager::instance()->actionContainer(Constant::G_SETTING);

    QAction * baseInfoAction = new QAction(QIcon(),tr("Baseinfo setting"));
    connect(baseInfoAction,SIGNAL(triggered()),this,SLOT(respBaseInfoEdit()));

    Action * baseInfo = ActionManager::instance()->registAction(BaseInfoConstant::BASEINFO_EDIT,baseInfoAction);
    settingMenu->addAction(baseInfo,Constant::G_SETTING);

    QAction * databaseAction = new QAction(QIcon(),tr("Database info setting"));
    connect(databaseAction,SIGNAL(triggered()),this,SLOT(respDatabaseEdit()));

    Action * database = ActionManager::instance()->registAction(BaseInfoConstant::DATABASE_EDIT,databaseAction);
    settingMenu->addAction(database,Constant::G_SETTING);
}

void BaseInfoWidget::updateBaseInfo()
{
    ui->listenIp->setText(RGlobal::G_GlobalConfigFile->netSettingConfig.textIp);
    ui->listenPort->setText(QString::number(RGlobal::G_GlobalConfigFile->netSettingConfig.textListenPort));
    ui->fileRecvSavePath->setText(RGlobal::G_GlobalConfigFile->fileServerSetting.fileRecvPath);
}

void BaseInfoWidget::updateDatabaseInfo()
{
    ui->databaseType->setText(DatabaseManager::getDatabaseName(RGlobal::G_GlobalConfigFile->databaseConfig.dbType));
    ui->hostName->setText(RGlobal::G_GlobalConfigFile->databaseConfig.hostName);
    ui->databaseName->setText(RGlobal::G_GlobalConfigFile->databaseConfig.dbName);
    ui->userName->setText(RGlobal::G_GlobalConfigFile->databaseConfig.dbUser);
    ui->password->setText("******");
    ui->port->setText(QString::number(RGlobal::G_GlobalConfigFile->databaseConfig.port));
}

void BaseInfoWidget::respBaseInfoEdit()
{
    BaseInfoEdit edit(this);
    edit.exec();
}

void BaseInfoWidget::respDatabaseEdit()
{
    DatabaseEdit edit(this);
    edit.exec();
}
