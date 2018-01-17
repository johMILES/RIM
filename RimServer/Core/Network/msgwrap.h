﻿/*!
 *  @brief     网络数据转换
 *  @details   接收应用层的数据，将其转换为网络数据发送结构
 *  @file      msgwrap.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.01.15
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef MSGWRAP_H
#define MSGWRAP_H

#include <QJsonObject>

#include "msgcontext.h"
#include "protocoldata.h"

using namespace ProtocolType;

class MsgWrap
{
public:
    MsgWrap();

    QByteArray handleMsg(MsgPacket * packet);
    QByteArray handleErrorSimpleMsg(MsgType type,MsgCommand command,int errorCode);

private:
    QByteArray handleRegistResponse(RegistResponse * packet);
    QByteArray handleLoginResponse(LoginResponse *packet);

    QByteArray wrappedPack(MsgPacket * packet, QJsonObject &data);

private:
    MsgResponseContext context;
};

#endif // MSGWRAP_H