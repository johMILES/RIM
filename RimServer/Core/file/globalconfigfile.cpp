#include "globalconfigfile.h"

#include "Util/rutil.h"
#include "Util/rsingleton.h"
#include "constants.h"
#include "../broadcastnode.h"
#include "../sql/databasemanager.h"

GlobalConfigFile::GlobalConfigFile():settings(nullptr)
{
    databaseConfig.dbType = Datastruct::DB_MYSQL;
    databaseConfig.hostName = "127.0.0.1";
    databaseConfig.dbName = "rimserver";
    databaseConfig.dbUser = "root";
    databaseConfig.dbPass = "rengu123456";
    databaseConfig.port = 3306;
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
    netSettingConfig.uploadFilePath = RUtil::getGlobalValue(Constant::GroupNetwork,Constant::FILE_IP,"./").toString();

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
