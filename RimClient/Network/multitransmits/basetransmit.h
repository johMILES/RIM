﻿/*!
 *  @brief     基本传输方式
 *  @details   软件支持多种信息传输信道，每种信道自己内部控制发送的逻辑。
 *  @author    wey
 *  @version   1.0
 *  @date      2018.06.11
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef BASETRANSMIT_H
#define BASETRANSMIT_H

#include "../network_global.h"
#include <functional>
#include <QByteArray>

namespace ClientNetwork{

class NETWORKSHARED_EXPORT BaseTransmit
{
public:
    explicit BaseTransmit(CommMethod method);
    virtual ~BaseTransmit();

    virtual bool startTransmit(const SendUnit & unit) = 0;
    virtual bool startRecv(char * recvBuff,int recvBuffLen,std::function<void(QByteArray &)> recvDataFunc) = 0;

    CommMethod type();
    bool connected();

    virtual bool connect(const char *remoteIp, const unsigned short remotePort, int timeouts = 3) = 0;
    virtual bool close() = 0;

protected:
    bool netConnected;

    CommMethod transMethod;
};

}

#endif // BASETRANSMIT_H
