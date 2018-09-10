﻿#include "sendtextprocessthread.h"

#include <qmath.h>

#include "Network/netglobal.h"
#include "Network/win32net/netutils.h"
#include "Network/connection/tcpclient.h"
#include "Network/win32net/iocpcontext.h"
#include "Network/win32net/tcpserver.h"
#include "../Network/connection/seriesconnection.h"
#include "Base/util/rlog.h"

#include "Network/multitransmits/tcptransmit.h"
#include "Network/multitransmits/bdtransmit.h"

using namespace ServerNetwork;

SendTextProcessThread::SendTextProcessThread()
{
    runningFlag = false;

    initTransmits();
}

SendTextProcessThread::~SendTextProcessThread()
{
    wait();
}

/*!
 * @brief 初始化数据传输链路
 */
void SendTextProcessThread::initTransmits()
{
    std::shared_ptr<TcpTransmit> tcpTrans = std::make_shared<TcpTransmit>();
    if(tcpTrans->initialize())
        transmits.insert(tcpTrans);

    std::shared_ptr<BDTransmit> bdTrans = std::make_shared<BDTransmit>();
    if(bdTrans->initialize())
        transmits.insert(bdTrans);
}

void SendTextProcessThread::run()
{
    runningFlag = true;

    while(runningFlag)
    {
        while(G_SendButts.size() == 0){
            G_SendCondition.wait(std::unique_lock<std::mutex>(G_SendMutex));
        }

        if(runningFlag)
        {
            SendUnit sockData;

            G_SendMutex.lock();
            if(G_SendButts.size() > 0)
            {
               sockData =  G_SendButts.front();
               G_SendButts.pop();
            }
            G_SendMutex.unlock();

            if(!handleDataSend(sockData)){
                if(sockData.localServer){

                }else{
                    //20180702 对错误处理,若为服务器连接，则需要从SeriesConnectionManager移除对应的连接
                    SeriesConnectionManager::instance()->removeFd(sockData.sockId);
                }
            }
        }
    }
}

bool SendTextProcessThread::handleDataSend(SendUnit &unit)
{
    auto selectedTrans = transmits.transmits.find(unit.method);
    if( selectedTrans == transmits.transmits.end())
        return false;

    return (*selectedTrans).second->startTransmit(unit);
}
