#include "databasemanager.h"

#include <QSqlError>

#include "Util/rlog.h"
#include "Util/rutil.h"

DatabaseManager::DatabaseManager()
{

}

void DatabaseManager::setDatabaseType(Datastruct::DatabaseType type)
{
    dbConfigInfo.dbType = type;
}

void DatabaseManager::setConnectInfo(Datastruct::DatabaseConfigInfo configInfo)
{
    this->dbConfigInfo = configInfo;
}

QString DatabaseManager::getDatabaseName(Datastruct::DatabaseType type)
{
    switch(type){
        case Datastruct::DB_MYSQL:
                return QString("QMYSQL");
            break;
        case Datastruct::DB_ORACLE:
                return QString("QORACLE");
            break;
        default:
            break;
    }
    return QString("");
}

Datastruct::DatabaseType DatabaseManager::getDatabaseType(QString dbType)
{
    dbType = dbType.toUpper();
    if(dbType == "QMYSQL")
        return Datastruct::DB_MYSQL;
    else if(dbType == "QORACLE")
        return Datastruct::DB_ORACLE;
    else
        return Datastruct::DB_NONE;
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
