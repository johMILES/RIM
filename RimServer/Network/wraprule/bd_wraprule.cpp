#include "bd_wraprule.h"

namespace ServerNetwork{

BD_WrapRule::BD_WrapRule()
{

}

void BD_WrapRule::registDataHandler(Handler *dataHandler)
{
    textHandler = dataHandler;
}

void BD_WrapRule::wrap(ProtocolPackage &data)
{

}

bool BD_WrapRule::unwrap(const QByteArray &data, ProtocolPackage &result)
{
    if(data.size() == 0)
    {
        return false;
    }

    return false;
}

bool BD_WrapRule::unwrap_TXA(const QByteArray &data, USET_TXA &result)
{
    if(data.size() < 4)
    {
        return false;
    }
    int CmdType = data.left(sizeof(int)).toInt();

    QByteArray recvData = data.right(data.size()-sizeof(int));
    switch(CmdType)
    {
    case TYPE_SEND_TXA:
        {
            USET_TXA txsq_data;
            memset(&txsq_data,0,sizeof(USET_TXA));
            memcpy(&txsq_data,recvData.data(),recvData.size());
            memcpy(&result,&txsq_data,sizeof(USET_TXA));
        }
        break;
    }
    return false;
}

}
