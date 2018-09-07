/*!
 *  @brief     BD传输模式
 *  @details   使用BD方式封装了数据传输格式协议
 *  @author    尚超
 *  @version   1.0
 *  @date      2018.09.07
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef BDTRANSMIT_H
#define BDTRANSMIT_H

#include <queue>
#include <memory>
#include <condition_variable>

#include "basetransmit.h"
#include "../wraprule/bd_wraprule.h"

namespace ServerNetwork{

void __stdcall CallBack_RecvFun(int CmdType,char*data,int size);

class NETWORKSHARED_EXPORT BDTransmit : public BaseTransmit
{
public:
    BDTransmit();
    ~BDTransmit();

    CommMethod type();
    QString name();

    static std::shared_ptr<BDTransmit> instance();
    static int CreateCount();

    bool startTransmit(SendUnit &unit);
    bool startRecv(char *recvBuff, int recvBuffLen,ByteArrayHandler recvDataFunc);

    bool connect(const char *remoteIp, const unsigned short remotePort, int timeouts);
    bool close();

    void recvComData(int CmdType,char*data,int size);

private:
    std::shared_ptr<BD_WrapRule> dataPacketRule;
    std::queue<QByteArray> m_TextRecvBuffs;
    std::mutex m_TextRecvMutex;
    std::condition_variable  m_TextRecvCondition;
};

}

#endif // BDTRANSMIT_H
