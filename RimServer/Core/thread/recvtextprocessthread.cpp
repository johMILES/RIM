﻿#include "recvtextprocessthread.h"

#include "Network/netglobal.h"
#include "Network/win32net/netutils.h"
#include "Network/msgparse/msgparsefactory.h"
#include "rsingleton.h"

#include <QDebug>

using namespace ServerNetwork;

RecvTextProcessThread::RecvTextProcessThread()
{

}

void RecvTextProcessThread::setDatabase(Database *db)
{
    database = db;
}

void RecvTextProcessThread::run()
{
    while(true)
    {
        while(G_RecvButts.size() == 0)
        {
            G_RecvCondition.wait(std::unique_lock<std::mutex>(G_RecvMutex));
        }

        SocketInData sockData;

        G_RecvMutex.lock();

        if(G_RecvButts.size() > 0)
        {
           sockData =  G_RecvButts.front();
           G_RecvButts.pop();
        }

        G_RecvMutex.unlock();

        RSingleton<MsgParseFactory>::instance()->getDataParse()->processData(database,sockData);
    }
}
