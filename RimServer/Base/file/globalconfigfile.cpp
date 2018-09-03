#include "globalconfigfile.h"

#include "Util/rutil.h"
#include "Util/rsingleton.h"
#include "constants.h"
#include "others/broadcastnode.h"
#include "sql/databasemanager.h"

#include <QDebug>

GlobalConfigFile::GlobalConfigFile():settings(nullptr)
{
    databaseConfig.dbType = Datastruct::DB_MYSQL;
    databaseConfig.hostName = "127.0.0.1";
    databaseConfig.dbName = "rimserver";
    databaseConfig.dbUser = "root";
    databaseConfig.dbPass = "rengu123456";
    databaseConfig.port = 3306;
}

/*!
 * @brief 保存网络配置信息
 * @param[in]  config 待保存网络配置信息
 */
bool GlobalConfigFile::saveSettingConfig(Datastruct::SettingConfig &config)
{
    if(settings == nullptr)
        return false;

    settings->beginGroup(Constant::GroupNetwork);
    settings->setValue(Constant::TEXT_PORT,config.textListenPort);
    settings->setValue(Constant::TEXT_IP,config.textIp);
    settings->endGroup();

    return true;
}

/*!
 * @brief 保存文件服务器配置信息
 * @param[in]  config 待保存文件服务器配置信息
 */
bool GlobalConfigFile::saveFileServerConfig(Datastruct::FileServerSetting &config)
{
    if(settings == nullptr)
        return false;

    settings->beginGroup(Constant::FileServerSetting);
    settings->setValue(Constant::UPLOAD_FILE_PATH,config.fileRecvPath);
    settings->endGroup();

    return true;
}

bool GlobalConfigFile::saveDatabaseConfig(Datastruct::DatabaseConfigInfo &config)
{
    if(settings == nullptr)
        return false;

    settings->beginGroup(Constant::DB_SETTING);
    settings->setValue(Constant::DB_TYPE,config.dbType);
    settings->setValue(Constant::DB_HOST,config.hostName);
    settings->setValue(Constant::DB_DATABASE_NAME,config.dbName);
    settings->setValue(Constant::DB_USERNAME,config.dbUser);
    settings->setValue(Constant::DB_PASSWORD,config.dbPass);
    settings->setValue(Constant::DB_PORT,config.port);
    settings->endGroup();

    return true;
}

bool GlobalConfigFile::parseFile()
{
    if(settings == nullptr)
        return false;

    //解析日志模块
    logConfig.isRecord2File = RUtil::getGlobalValue(Constant::LOG_LOG,Constant::LOG_LOG_RECORDFILE,true).toBool();
    logConfig.level = RUtil::getGlobalValue(Constant::LOG_LOG,Constant::LOG_LOG_LEVEL,RLog::RINFO).toInt();

    //网络配置
    netSettingConfig.textRecvProcCount = RUtil::getGlobalValue(Constant::GroupNetwork,Constant::DB_THREAD,1).toInt();
    netSettingConfig.textSendProcCount = RUtil::getGlobalValue(Constant::GroupNetwork,Constant::MSG_THREAD,1).toInt();
    netSettingConfig.textListenPort = RUtil::getGlobalValue(Constant::GroupNetwork,Constant::TEXT_PORT,8023).toInt();
    netSettingConfig.fileListenPort = RUtil::getGlobalValue(Constant::GroupNetwork,Constant::FILE_PORT,8024).toInt();
    netSettingConfig.textIp = RUtil::getGlobalValue(Constant::GroupNetwork,Constant::TEXT_IP,"127.0.0.1").toString();
    netSettingConfig.fileIp = RUtil::getGlobalValue(Constant::GroupNetwork,Constant::FILE_IP,"127.0.0.1").toString();

    //文件服务器配置
    fileServerSetting.fileRecvPath = RUtil::getGlobalValue(Constant::FileServerSetting,Constant::UPLOAD_FILE_PATH,"./").toString();

    //服务器发送
    QString defaultBraodCastNode = RSingleton<BroadcastNode>::instance()->getDefaultNode();
    RSingleton<BroadcastNode>::instance()->parseData(RUtil::getGlobalValue(Constant::TRANS_SETTING,Constant::BROADCAST_DEST_NODE,defaultBraodCastNode).toString());

    //数据库配置
    QString defaultDb = DatabaseManager::getDatabaseName(databaseConfig.dbType);

    QString db = RUtil::getGlobalValue(Constant::DB_SETTING,Constant::DB_TYPE,defaultDb).toString();
    databaseConfig.dbType = DatabaseManager::getDatabaseType(db);
    databaseConfig.hostName = RUtil::getGlobalValue(Constant::DB_SETTING,Constant::DB_HOST,databaseConfig.hostName).toString();
    databaseConfig.dbName = RUtil::getGlobalValue(Constant::DB_SETTING,Constant::DB_DATABASE_NAME,databaseConfig.dbName).toString();
    databaseConfig.dbUser = RUtil::getGlobalValue(Constant::DB_SETTING,Constant::DB_USERNAME,databaseConfig.dbUser).toString();
    databaseConfig.dbPass = RUtil::getGlobalValue(Constant::DB_SETTING,Constant::DB_PASSWORD,databaseConfig.dbPass).toString();
    databaseConfig.port = RUtil::getGlobalValue(Constant::DB_SETTING,Constant::DB_PORT,databaseConfig.port).toInt();

    return true;
}
