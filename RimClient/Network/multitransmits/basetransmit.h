/*!
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
#include <memory>
#include <QByteArray>

typedef std::function<void(FileDataSendProgress)> SendCallbackFunc;

namespace ClientNetwork{

class NETWORKSHARED_EXPORT BaseTransmit
{
public:
    explicit BaseTransmit();
    virtual ~BaseTransmit();

    virtual bool initialize() = 0;
    virtual bool startTransmit(SendUnit & unit,SendCallbackFunc func = nullptr) = 0;
    virtual bool startRecv(char * recvBuff,int recvBuffLen,DataHandler recvDataFunc) = 0;
    virtual bool close() = 0;

    virtual CommMethod type() = 0;
    virtual QString name() = 0;

    bool connected();

protected:
    bool netConnected;
};

}

typedef std::shared_ptr<ClientNetwork::BaseTransmit> BaseTransmitPtr;

#endif // BASETRANSMIT_H
