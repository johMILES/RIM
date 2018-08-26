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

#include "database.h"
#include "protocol/datastruct.h"

class DatabaseManager
{
public:
    DatabaseManager();
    void setDatabaseType(Datastruct::DatabaseType type);
    void setConnectInfo(Datastruct::DatabaseConfigInfo configInfo);

    static QString getDatabaseName(Datastruct::DatabaseType type);
    static Datastruct::DatabaseType getDatabaseType(QString dbType);

    Database * newDatabase(QString connectionName = "");
    Database database(QString connectionName = "");

    bool hasFeature(QSqlDriver::DriverFeature feature);
    QStringList availableDrivers();

private:
    Datastruct::DatabaseConfigInfo dbConfigInfo;
};

#endif // DATABASEMANAGER_H
