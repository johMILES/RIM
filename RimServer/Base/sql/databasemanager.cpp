#include "databasemanager.h"

#include <QSqlError>

#include <functional>

#include "Util/rlog.h"
#include "Util/rutil.h"

DatabaseManager::DatabaseManager()
{
    dbList.push_back(SupportedDB("QMYSQL","MySQL",Datastruct::DB_MYSQL));
    dbList.push_back(SupportedDB("QORACLE","Oracle",Datastruct::DB_ORACLE));
    dbList.push_back(SupportedDB("None","None",Datastruct::DB_NONE,false));
}

DatabaseManager::SupportedDBCollection DatabaseManager::dbList;

void DatabaseManager::setDatabaseType(Datastruct::DatabaseType type)
{
    dbConfigInfo.dbType = type;
}

void DatabaseManager::setConnectInfo(Datastruct::DatabaseConfigInfo configInfo)
{
    this->dbConfigInfo = configInfo;
}

/*!
 * @brief 获取对应数据库类型的数据库名
 * @param[in] type 数据库类型
 * @return 对应数据库名，不存在的返回空值
 */
QString DatabaseManager::getDatabaseName(Datastruct::DatabaseType type)
{
    auto index = std::find_if(dbList.begin(),dbList.end(),[&type](SupportedDB & db){
        return type == db.databaseType;
    });

    if(index != dbList.end())
        return index->driverName;

    return QString("");
}

Datastruct::DatabaseType DatabaseManager::getDatabaseType(QString driverName)
{
    auto index = std::find_if(dbList.begin(),dbList.end(),[&driverName](SupportedDB & db){
        return driverName == db.databaseName;
    });

    if(index != dbList.end())
        return index->databaseType;

    return Datastruct::DB_NONE;
}

/*!
 * @brief 获取当前工具支持的数据库名称集合
 * @return 所支持的数据库名称集合
 */
QStringList DatabaseManager::getSupportedDatabase()
{
    QStringList list;
    std::for_each(dbList.begin(),dbList.end(),[&list](SupportedDB & db){
        if(db.valid)
            list<<db.databaseName;
    });
    return list;
}

/*!
 * @brief 测试当前设置是否正常
 * @param[in] info 新的数据库连接信息
 * @return true表示测试通过，false表示测试失败
 */
bool DatabaseManager::testConnection(Datastruct::DatabaseConfigInfo &info)
{
    bool opened = false;
    Database db(info.dbType,RUtil::UUID());
    if(db.init())
    {
        db.setHostName(info.hostName);
        db.setDatabaseName(info.dbName);
        db.setUserName(info.dbUser);
        db.setPassword(info.dbPass);
        if(info.port > 0)
            db.setPort(info.port);

        opened = db.open();

        db.close();
    }
    return opened;
}

Database *DatabaseManager::newDatabase(QString connectionName)
{
    Database * db = new Database(dbConfigInfo.dbType,connectionName);
    if(db->init())
    {
        db->setHostName(dbConfigInfo.hostName);
        db->setDatabaseName(dbConfigInfo.dbName);
        db->setUserName(dbConfigInfo.dbUser);
        db->setPassword(dbConfigInfo.dbPass);
        if(dbConfigInfo.port > 0)
            db->setPort(dbConfigInfo.port);

        db->open();
    }
    return db;
}

Database DatabaseManager::database(QString connectionName)
{
    Database db(dbConfigInfo.dbType,connectionName);
    if(db.init())
    {
        db.setHostName(dbConfigInfo.hostName);
        db.setDatabaseName(dbConfigInfo.dbName);
        db.setUserName(dbConfigInfo.dbUser);
        db.setPassword(dbConfigInfo.dbPass);
        if(dbConfigInfo.port > 0)
            db.setPort(dbConfigInfo.port);

        db.open();
    }
    return db;
}

/*!
 * @brief 查询当前数据库是否支持指定的功能
 * @param[in] feature 待查询的功能
 * @return 是否支持指定的功能
 */
bool DatabaseManager::hasFeature(QSqlDriver::DriverFeature feature)
{
    static Database db(dbConfigInfo.dbType);
    if(db.init()){
        return db.sqlDatabase().driver()->hasFeature(feature);
    }
    return false;
}

QStringList DatabaseManager::availableDrivers()
{
    return QSqlDatabase::drivers();
}
