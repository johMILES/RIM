/*!
 *  @brief     数据库管理
 *  @details   负责创建数据库连接
 *  @file      databasemanager.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.01.XX
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDriver>
#include <QList>

#include "database.h"
#include "../protocol/datastruct.h"
#include "../base_global.h"

class BASESHARED_EXPORT SupportedDB
{
public:
    SupportedDB(QString drName,QString dname,Datastruct::DatabaseType dtype,bool isValid = true):
        driverName(drName),databaseName(dname),databaseType(dtype),valid(isValid){

    }
    QString driverName;             /*!< 驱动名称 QMYSQL */
    QString databaseName;           /*!< 数据库名称 MySQL */
    Datastruct::DatabaseType databaseType;      /*!< 数据库类型 */
    bool valid;                     /*!< 是否有效 */
};

class BASESHARED_EXPORT DatabaseManager
{
public:
    DatabaseManager();
    void setDatabaseType(Datastruct::DatabaseType type);
    void setConnectInfo(Datastruct::DatabaseConfigInfo configInfo);

    typedef QList<SupportedDB> SupportedDBCollection;

    static QString getDatabaseName(Datastruct::DatabaseType type);
    static Datastruct::DatabaseType getDatabaseType(QString driverName);

    static QStringList getSupportedDatabase();

    static bool testConnection(Datastruct::DatabaseConfigInfo & info);

    Database * newDatabase(QString connectionName = "");
    Database database(QString connectionName = "");

    bool hasFeature(QSqlDriver::DriverFeature feature);
    QStringList availableDrivers();

private:
    Datastruct::DatabaseConfigInfo dbConfigInfo;
    static SupportedDBCollection dbList;
};

#endif // DATABASEMANAGER_H
