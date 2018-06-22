﻿/*!
 *  @brief     DDS传输模式
 *  @details   使用DDS方式封装了数据传输格式协议
 *  @author    SC
 *  @version   1.0
 *  @date      2018.06.12
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef DDSTRANSMIT_H
#define DDSTRANSMIT_H

#include <memory>

#include "basetransmit.h"
#include "../wraprule/datapacketrule.h"

namespace ClientNetwork{

class NETWORKSHARED_EXPORT DDSTransmit : public BaseTransmit
{
public:
    DDSTransmit();
    ~DDSTransmit();

    bool startTransmit(const SendUnit &unit);
    bool startRecv(char *recvBuff, int recvBuffLen,ByteArrayHandler recvDataFunc);

    bool connect(const char *remoteIp, const unsigned short remotePort, int timeouts);
    bool close();


private:
    std::shared_ptr<DataPacketRule> dataPacketRule;
};

}

#endif // DDSTRANSMIT_H
