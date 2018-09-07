#include "bdtransmit.h"
#include "../../BD/bdcommanager.h"
#include "../../BD/bduser.h"
#include "Util/rlog.h"
#include <QFileInfo>

namespace ServerNetwork{

BDTransmit *g_BDTransmit = NULL;

BDTransmit::BDTransmit() :
    BaseTransmit()
{
}

BDTransmit::~BDTransmit()
{

}

CommMethod BDTransmit::type()
{
    return C_COM;
}

QString BDTransmit::name()
{
    return "BDCom";
}

bool BDTransmit::startTransmit(SendUnit &unit)
{
    QByteArray bdMSG = unit.dataUnit.data;
    USET_TXA t_txa;
    memset(&t_txa,0,sizeof(USET_TXA));
    t_txa.Type = USER_TXSQ_BWTX;
    t_txa.DestID = unit.dataUnit.pack495.destAddr;
    t_txa.Fast = 0;
    t_txa.Flag = 0;
    t_txa.Key = 0;
    t_txa.Len = bdMSG.size();
    memcpy(t_txa.Msg,bdMSG.data(),bdMSG.size());

    char *buff = new char[10+bdMSG.size()];
    memcpy(buff,&t_txa,10+bdMSG.size());

    int iRet = BDCom_SendCmd(TYPE_SEND_TXA,buff,10+bdMSG.size());
    if (iRet < 1){
        RLOG_ERROR("DDS send topic failed!");
    }
    delete [] buff;
    return iRet;
}

bool BDTransmit::startRecv(char *recvBuff, int recvBuffLen, ByteArrayHandler recvDataFunc)
{
    std::unique_lock<std::mutex> ul(m_TextRecvMutex);
    m_TextRecvCondition.wait(ul);

    m_TextRecvMutex.lock();
    RecvUnit inData;
    inData.extendData.method = C_COM;
    inData.extendData.type = SOCKET_TEXT;
    inData.data = m_TextRecvBuffs.front();
    recvDataFunc(inData);

    m_TextRecvMutex.unlock();

    return true;
}

bool BDTransmit::connect(const char *remoteIp, const unsigned short remotePort, int timeouts)
{
    QString path = QString::fromLocal8Bit("北斗协议.ini");
    QFileInfo file(path);

    if(QFileInfo::exists(path))
    {
        int ret = BDCom_Init(file.absoluteFilePath().toLocal8Bit().data());
        BDCom_BindRecv(CallBack_RecvFun);
        return ret;
    }
    return true;
}

bool BDTransmit::close()
{
    return true;
}

void __stdcall CallBack_RecvFun(int CmdType, char *data, int size)
{
    if(g_BDTransmit)
    {
        g_BDTransmit->recvComData(CmdType,data,size);
    }
}

void BDTransmit::recvComData(int CmdType, char *data, int size)
{
    m_TextRecvMutex.lock();
    QByteArray array(data,size);
    array.prepend(CmdType);
    m_TextRecvBuffs.push(array);
    m_TextRecvMutex.unlock();

    m_TextRecvCondition.notify_one();
}

}
