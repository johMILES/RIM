﻿#include "msgreceiveproctask.h"

#include <QJsonDocument>

#include "network/netglobal.h"
#include "dataprocess.h"
#include "rsingleton.h"
#include "head.h"
#include "jsonkey.h"

MsgReceiveProcTask::MsgReceiveProcTask(QObject *parent):
    ClientNetwork::RTask(parent)
{

}

MsgReceiveProcTask::~MsgReceiveProcTask()
{
    wait();
}

void MsgReceiveProcTask::startMe()
{
    RTask::startMe();

    runningFlag = true;

    if(!isRunning())
    {
        start();
    }
    else
    {
        G_TextRecvCondition.wakeOne();
    }
}

void MsgReceiveProcTask::run()
{
    while(runningFlag)
    {
        while(G_TextRecvBuffs.isEmpty())
        {
            G_TextRecvMutex.lock();
            G_TextRecvCondition.wait(&G_TextRecvMutex);
            G_TextRecvMutex.unlock();
        }

        if(runningFlag && G_TextRecvBuffs.size() > 0)
        {
            G_TextRecvMutex.lock();
            QByteArray array = G_TextRecvBuffs.dequeue();
            G_TextRecvMutex.unlock();

            if(array.size() > 0)
            {
                validateRecvData(array);
            }
        }
    }
}

void MsgReceiveProcTask::validateRecvData(const QByteArray &data)
{
    QJsonDocument document = QJsonDocument::fromJson(data,&jsonParseError);
    if(jsonParseError.error == QJsonParseError::NoError)
    {
        QJsonObject root = document.object();
#ifndef __NO_SQL_PRINT__
        qDebug()<<document.toJson(QJsonDocument::Indented);
#endif
        switch(root.value(JsonKey::key(JsonKey::Type)).toInt())
        {
            case MSG_CONTROL:
                handleCommandMsg((MsgCommand)root.value(JsonKey::key(JsonKey::Command)).toInt(),root);
                break;
            case MSG_TEXT:
                handleTextMsg((MsgCommand)root.value(JsonKey::key(JsonKey::Command)).toInt(),root);
                break;
            default:
                break;
        }
    }
}

/*!
     * @brief 处理命令消息
     * @param[in] commandType 命令类型
     * @param[in] obj 数据主体
     * @return 无
     */
void MsgReceiveProcTask::handleCommandMsg(MsgCommand commandType, QJsonObject &obj)
{
    switch(commandType)
    {
        case MSG_USER_REGISTER:
                RSingleton<DataProcess>::instance()->proRegistResponse(obj);
                break;
        case MSG_USER_LOGIN:
                RSingleton<DataProcess>::instance()->proLoginResponse(obj);
                break;
        case MSG_USER_UPDATE_INFO:
                RSingleton<DataProcess>::instance()->proUpdateBaseInfoResponse(obj);
                break;
        case MSG_USER_STATE:
                RSingleton<DataProcess>::instance()->proUserStateChanged(obj);
                break;
        case MSG_RELATION_SEARCH:
                RSingleton<DataProcess>::instance()->proSearchFriendResponse(obj);
                break;
        case MSG_REALTION_ADD:
                RSingleton<DataProcess>::instance()->proAddFriendResponse(obj);
                break;
        case MSG_RELATION_OPERATE:
                RSingleton<DataProcess>::instance()->proOperateFriendResponse(obj);
                break;
        case MSG_RELATION_LIST:
                RSingleton<DataProcess>::instance()->proFriendListResponse(obj);
                break;
        case MSG_GROUPING_OPERATE:
                RSingleton<DataProcess>::instance()->proGroupingOperateResponse(obj);
                break;
        case MSG_RELATION_GROUPING_FRIEND:
                RSingleton<DataProcess>::instance()->proGroupingFriendResponse(obj);
                break;
        case MSG_GROUP_LIST:
                RSingleton<DataProcess>::instance()->proGroupListResponse(obj);
                break;
        case MSG_GROUP_CREATE:
                RSingleton<DataProcess>::instance()->proRegistGroupResponse(obj);
                break;
        case MSG_GROUP_SEARCH:
                RSingleton<DataProcess>::instance()->proSearchGroupResponse(obj);
                break;
        default:break;
    };
}

void MsgReceiveProcTask::handleTextMsg(MsgCommand commandType, QJsonObject &obj)
{
    switch(commandType)
    {
        case MSG_TEXT_TEXT:
        case MSG_TEXT_SHAKE:
            RSingleton<DataProcess>::instance()->proText(obj);
            break;

        case MSG_TEXT_APPLY:
            RSingleton<DataProcess>::instance()->proTextApply(obj);
            break;

        default:
            break;
    }
}

void MsgReceiveProcTask::stopMe()
{
    RTask::stopMe();
    runningFlag = false;
    G_TextRecvCondition.wakeOne();
}
