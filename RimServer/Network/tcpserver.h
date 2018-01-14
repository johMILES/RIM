﻿/*!
 *  @brief     服务器端
 *  @details   用于接受客户端的请求，负责创建socket、IOCP、处理线程
 *  @file      tcpserver.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.01.09
 *  @warning
 *  @copyright NanJing RenGu.
 */

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QList>

#include "head.h"
#include "netglobal.h"
#include "tcpclient.h"
#include "iocpcontext.h"

namespace ServerNetwork {

class WorkThread;
class TcpClient;
class SharedIocpData;
class TcpClientManager;

class NETWORKSHARED_EXPORT TcpServer
{
public:
    explicit TcpServer();

    static TcpServer * instance();

    TcpClientManager * clientManager();

    bool startMe(const char * ip,unsigned short port);

    void handleIo(IocpContext * ioData,unsigned long recvLength,TcpClient * recvClient);

    WorkThread * getThread()
    {
        return workThreads.at(0);
    }

#if defined(Q_OS_WIN)
    bool createListenSocket(const char * ip,unsigned short port);
    bool createIocpPort();
    bool createWorkThread();
    bool startAccept();
    friend UINT  __stdcall  workerThreadProc(LPVOID v);
#endif

private:
    void handleRecv(IocpContext *ioData,unsigned long recvLen,TcpClient* tcpClient);
    void handleAccept(IocpContext *ioData);

private:
    static TcpServer * serverInstance;

    bool runningFlag;
    int dwNumberOfProcessors;

    SharedIocpData * m_sharedIocpData;
    QList<WorkThread *> workThreads;
};

}   //namespace ServerNetwork

#endif // TCPSERVER_H