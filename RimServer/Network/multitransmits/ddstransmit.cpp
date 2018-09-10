#include "ddstransmit.h"

#include "3rdhead/DDS.h"
#include "Base/util/rlog.h"

namespace ServerNetwork{

DDSTransmit::DDSTransmit()
{

}

DDSTransmit::~DDSTransmit()
{

}

bool DDSTransmit::initialize()
{
    int result = DDSInit("Hello",2);
    if(result <=  DTS_ERROR){
        RLOG_ERROR("Initialize failed[%d]",result);
        return false;
    }

    result = DDSSubscribe(0x1234,0,0);
    if(result <=  DTS_ERROR){
        RLOG_ERROR("subscribe failed[%d]",result);
        return false;
    }

    result = DDSCreateTopic("Demo",0x2345);
    if(result <=  DTS_ERROR){
        RLOG_ERROR("create topic failed[%d]",result);
        return false;
    }
    netConnected = true;
    return true;
}

bool DDSTransmit::startTransmit(SendUnit &unit)
{
    if(!netConnected || unit.method != type())
        return false;

    int result = DDSSend(unit.dataUnit.data.data(),unit.dataUnit.data.size(),0);
    if(result < 1){
        RLOG_ERROR("send dds data failed!");
        return false;
    }

    return true;
}

bool DDSTransmit::startRecv(char *recvBuff, int recvBuffLen, ByteArrayHandler recvDataFunc)
{
    unsigned int topic = 1;
    int result = DDSFetch(recvBuff,recvBuffLen,topic);
    if(result == DTS_SUCCEED){
        //TODO 加入对数据类型 的处理
    }
    return true;
}

CommMethod DDSTransmit::type()
{
    return C_DDS;
}

QString DDSTransmit::name()
{
    return "DDS";
}

bool DDSTransmit::close()
{
    return true;
}

}
