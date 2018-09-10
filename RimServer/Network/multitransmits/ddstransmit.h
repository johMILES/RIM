/*!
 *  @brief     DDS传输控制
 *  @details   将DDS中的
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.10
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef DDSTRANSMIT_H
#define DDSTRANSMIT_H

#include "basetransmit.h"

namespace ServerNetwork{

class DDSTransmit : public BaseTransmit
{
public:
    explicit DDSTransmit();
    ~DDSTransmit();

    bool initialize();
    bool startTransmit(SendUnit & unit);
    bool startRecv(char * recvBuff,int recvBuffLen,ByteArrayHandler recvDataFunc);
    bool close();

    CommMethod type();
    QString name();

};

}

#endif // DDSTRANSMIT_H
