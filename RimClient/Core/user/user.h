﻿/*!
 *  @brief     登陆用户信息
 *  @details   保存当前登陆用户的所有信息，可通过此获取登陆用户文件目录系统
 *  @file      user.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.03.22
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 *       20180322:wey:整个个人信息访问接口
 */
#ifndef USER_H_2018_03_22
#define USER_H_2018_03_22

#include <QString>
#include <QVariant>
class QSettings;
class Database;

#include "protocoldata.h"

using namespace ProtocolType;

class User
{
public:
    explicit User(const UserBaseInfo & baseInfo);
    User(const QString userId);
    ~User();

    QString getUserHome()const;

    QString getUserDatabasePath();
    void setUserDatabasePath(const QString & path);

    QString getFileRecvPath();
    void setFileRecvPath(const QString & path);

    QSettings * getSettings();

    QVariant getSettingValue(const QString & group,const QString &key,QVariant defaultValue);
    void setSettingValue(const QString & group,const QString &key,QVariant value);

    QString getFilePath(QString fileId);
    QString getIcon();

    QString getChatImgPath();
    void setChatImgPath(const QString &path);

    void setDatabase(Database * database);
    Database * database();

    UserBaseInfo & BaseInfo(){return userBaseInfo;}

private:
    bool createUserHome(const QString id);

private:
    static User * puser;

    UserBaseInfo userBaseInfo;       /*!< 用户基本数据信息 */

    QString userHome;
    QString userDBPath;
    QString userFilePath;
    QString chatImgPath;    //聊天对话中图片存储路径

    QSettings * userSettings;

    Database * chatDatabase;            /*!< 数据库连接 */
};

#endif // USER_H
