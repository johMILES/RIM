﻿/*!
 *  @brief     数据执行处理
 *  @details   用于对数据库进行操作
 *  @file      sqlprocess.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.01.16
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 *      20180201:wey:添加对用户分组好友的操作(创建、移动、修改)
 *      20180301:wey:修复查询默认分组时未加入默认分组条件bug
 *      20180418:wey:修复删除分组时，为将分组下联系人移动至默认分组;
 *      20180419:wey:添加分组排序
 *      20180424:wey:添加群分组相关操作
 */
#ifndef SQLPROCESS_H
#define SQLPROCESS_H

#include "protocol/protocoldata.h"
using namespace ProtocolType;

#include "Network/connection/tcpclient.h"

namespace Datastruct
{
struct FileItemInfo;
}

class Database;

class SQLProcess
{
public:
    SQLProcess();

    ResponseRegister processUserRegist(Database *db, const RegistRequest * request, QString & accountId, QString &userId);
    ResponseLogin processUserLogin(Database *db, const LoginRequest * request);
    ResponseUpdateUser processUpdateUserInfo(Database *db, const UpdateBaseInfoRequest * request);
    ResponseAddFriend processSearchFriend(Database *db,SearchFriendRequest *request,SearchFriendResponse * response);
    ResponseAddFriend processSearchGroup(Database *db,SearchFriendRequest *request,SearchGroupResponse * response);
    ResponseAddFriend processAddFriendRequest(Database *db,QString accountId,QString operateId,int type);

    bool createGroupAndGroupDesc(Database *db, OperateType type, QString userId, QString accountId, QString groupId, QString groupName, bool isDefault = false);
    bool createGroup(Database *db, QString userId, QString groupName, QString groupId, bool isDefault = false);
    bool createChatGroup(Database *db, QString userId, QString groupName, QString groupId, bool isDefault = false);

    bool renameGroup(Database *db,GroupingRequest* request);
    bool deleteGroup(Database *db,GroupingRequest* request);

    bool createGroupDesc(Database *db,OperateType type, QString uuid,QString accountId,QString groupId);
    bool addGroupToGroupDesc(Database *db, OperateType gtype, const QString &userId, QString groupId);
    bool delGroupInGroupDesc(Database *db,GroupingRequest *request);
    bool sortGroupInGroupDesc(Database *db,GroupingRequest *request);

    bool testEstablishRelation(Database *db,OperateFriendRequest *request);
    bool establishRelation(Database *db,OperateFriendRequest *request);
    bool getFriendList(Database *db,QString accountId,FriendListResponse * response);
    void getFriendAccountList(Database *db, const QString accountId, QList<QString> &friendList);
    bool getUserInfo(Database *db, const QString accountId, UserBaseInfo &userInfo);
    bool getUserByChatroomId(Database *db, const QString chatroomId,QString & chatUserId);

    bool updateGroupFriendInfo(Database *db,GroupingFriendRequest * request);
    bool updateMoveGroupFriend(Database *db,GroupingFriendRequest * request);
    bool deleteFriend(Database *db, GroupingFriendRequest * request, QString &accountId, QString &otherUserGroupId);

    bool getGroupList(Database * db, const QString &userId, ChatGroupListResponse * response);
    ResponseRegister registGroup(Database * db, RegistGroupRequest * request,RegistGroupResponse * response);
    bool addChatGroupToGroup(Database * db,RegistGroupRequest *request, RegistGroupResponse *response);
    bool getSingleChatGroupInfo(Database * db,RegistGroupResponse * response);
    bool getChatroomInfo(Database * db,const QString chatId,ChatBaseInfo & baseInfo);
    bool getSimpleChatInfoByChatroomId(Database * db, QString groupId, SimpleChatInfo & chatInfo);

    bool exitGroupChat(Database * db, GroupingCommandRequest * request);

    bool loadSystemCache(Database * db,QString accountId,QList<AddFriendRequest> & requests);
    bool loadChatCache(Database * db, QString accountId, QList<TextRequest> &textResponse);

    bool saveUserChat2Cache(Database * db, QSharedPointer<TextRequest> request);

    bool addQuoteFile(Database * db,const FileItemRequest * request);
    bool queryFile(Database * db,const QString & fileMd5);
    bool addFile(Database * db, ServerNetwork::FileRecvDesc * desc);
    bool getFileInfo(Database * db, SimpleFileItemRequest *request, FileItemRequest * response);
    bool getDereferenceFileInfo(Database * db, SimpleFileItemRequest *request, Datastruct::FileItemInfo *itemInfo);

    QString getDefaultGroupByUserId(Database * db, OperateType type, const QString userId);
    QString getDefaultGroupByUserAccountId(Database * db,OperateType type,const QString id);
    QStringList getGroupListByUserId(Database * db,const QString id);
    QStringList getGroupListByUserAccountId(Database * db,const QString id);

#ifdef __LOCAL_CONTACT__
    bool saveChat716Cache(Database * db,ProtocolPackage & packageData);
    bool loadChat716Cache(Database * db,unsigned short nodeId,QList<ProtocolPackage> & historyResult);
#endif

private:
    QStringList getGroupsById(Database * db,const QString id);
};

#endif // SQLPROCESS_H
