﻿/*!
 *  @brief     495传输控制协议
 *  @details   采用716传输控制协议报文头，实现TCP传输，解决TCP传输粘包问题
 *  @author    wey
 *  @version   1.0
 *  @date      2018.06.26
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef TCP495DATAPACKETRULE_H
#define TCP495DATAPACKETRULE_H

#include "wraprule.h"

#include <QHash>

namespace ClientNetwork{

class TCP495DataPacketRule  : public WrapRule
{
public:
    explicit TCP495DataPacketRule();

    void wrap(ProtocolPackage & data);
    bool unwrap(const QByteArray & data,ProtocolPackage & result);

    bool wrap(const ProtocolPackage &dataUnit, std::function<int(const char *,const int)> sendDataFunc);
    bool unwrap(const char * data,const int length,DataHandler recvDataFunc);

private:
    void recvData(const char *recvData, int recvLen, RecvUnit &result);
    int countTotoalIndex(const int totalLength);

private:
    unsigned short SendPackId;
    char sendBuff[MAX_SEND_BUFF];

    QByteArray lastRecvBuff;              //断包接收缓冲区
    QHash<int,PacketBuff*> packetBuffs;   //多包缓冲区
};

}

#endif // TCP495DATAPACKETRULE_H
