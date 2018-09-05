/*!
 *  @brief     软件启动配置文件
 *  @details   启动时解析/config/config.ini文件
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.17
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef GLOBALCONFIGFILE_H
#define GLOBALCONFIGFILE_H

#include <QSettings>

#include "Util/rlog.h"
#include "../protocol/datastruct.h"

class GlobalConfigFile
{
public:
    GlobalConfigFile();

    void setSettings(QSettings * settings){this->settings = settings;}

    bool saveSettingConfig(Datastruct::SettingConfig & config);
    bool saveFileServerConfig(Datastruct::FileServerSetting & config);
    bool saveDatabaseConfig(Datastruct::DatabaseConfigInfo & config);

    bool parseFile();

public:
    RLog::LogConfig logConfig;
    Datastruct::SettingConfig netSettingConfig;
    Datastruct::DatabaseConfigInfo databaseConfig;
    Datastruct::FileServerSetting fileServerSetting;

private:
    QVariant getGlobalValue(const QString & group,const QString &key, const QVariant & defaultValue);

private:
    QSettings * settings;

};

#endif // GLOBALCONFIGFILE_H
