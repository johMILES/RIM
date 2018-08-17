﻿#include "filereceiveproctask.h"

#include "Network/netglobal.h"
#include "global.h"

#include "util/rsingleton.h"
#include "Network/msgparse/msgparsefactory.h"
#include "../Network/wraprule/tcp_wraprule.h"

#include <QDebug>

FileReceiveProcTask::FileReceiveProcTask(QObject *parent):
    RTask(parent)
{

}

FileReceiveProcTask::~FileReceiveProcTask()
{
    stopMe();
    wait();
}

void FileReceiveProcTask::startMe()
{
    RTask::startMe();

    runningFlag = true;

    if(!isRunning())
    {
        start();
    }
    else
    {
        Global::G_FileRecvCondition.notify_one();
    }
}

void FileReceiveProcTask::stopMe()
{
    RTask::stopMe();
    runningFlag = false;
    Global::G_FileRecvCondition.notify_one();
}

void FileReceiveProcTask::run()
{
    while(runningFlag)
    {
        while(runningFlag && Global::G_FileRecvBuffs.empty())
        {
            std::unique_lock<std::mutex> ul(Global::G_FileRecvMutex);
            Global::G_FileRecvCondition.wait(ul);
        }

        if(runningFlag && Global::G_FileRecvBuffs.size() > 0)
        {
            Global::G_FileRecvMutex.lock();
            RecvUnit array = Global::G_FileRecvBuffs.front();
            Global::G_FileRecvBuffs.pop();
            Global::G_FileRecvMutex.unlock();

            if(array.data.size() > 0)
            {
                validateRecvData(array);
            }
        }
    }
}

void FileReceiveProcTask::validateRecvData(const RecvUnit &data)
{
    ProtocolPackage package;
#ifdef __LOCAL_CONTACT__

    //可能是第一包数据，也可能就一包数据
    bool result = false;

    if(data.extendData.wOffset == 0){
        switch(data.extendData.method){
            case C_TCP:
                result = RSingleton<ClientNetwork::TCP_WrapRule>::instance()->unwrap(data.data,package);
                break;
            default:
                break;
        }
    }else{
        result = true;
        package.data = data.data;
    }

    if(result){
        package.bPackType = (unsigned char)data.extendData.type495;
        package.bPeserve = data.extendData.bPeserve;
        package.usSerialNo = data.extendData.usSerialNo;
        package.usOrderNo = data.extendData.usOrderNo;
        package.wOffset = data.extendData.wOffset;
        package.dwPackAllLen = data.extendData.dwPackAllLen;
        package.cFileType = data.extendData.fileType;
        package.wSourceAddr = data.extendData.wSourceAddr;
        package.wDestAddr = data.extendData.wDestAddr;

        RSingleton<MsgParseFactory>::instance()->getDataParse()->processData(package);
    }

#else
    package.data = data.data;
    RSingleton<MsgParseFactory>::instance()->getDataParse()->processData(package);
#endif

}
