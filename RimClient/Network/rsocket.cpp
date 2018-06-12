﻿#include "rsocket.h"

#include <QtGlobal>
#include <QDebug>

#ifdef WIN32
#include <WinSock2.h>
#include <windows.h>
typedef  int socklen_t;
#elif defined(LINUX)
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define closesocket close
#endif

#include "Util/rlog.h"

#pragma  comment(lib,"ws2_32.lib")

namespace ClientNetwork {

RSocket::RSocket()
{
    tcpSocket = 0;
    socketPort = 0;
    errorCode = 0;
    socketValid = false;
    blockAble = false;
    memset(socketIp,0,sizeof(socketIp));
}

RSocket::~RSocket()
{
    qDebug()<<"delete socket";
}

bool RSocket::createSocket(int type)
{
#ifdef WIN32
    static bool isInit = false;
    if(!isInit)
    {
        isInit = true;
        WSADATA ws;
        if(WSAStartup(MAKEWORD(2,2),&ws) != 0)
        {
            RLOG_ERROR("Init windows socket failed!");
            return false;
        }
    }
#endif

    tcpSocket = socket(AF_INET,type,0);
    if(tcpSocket == INVALID_SOCKET)
    {
        errorCode = WSAGetLastError();
        RLOG_ERROR("Create socket failed! [ErrorCode:%d]",errorCode);
        return false;
    }

    RLOG_INFO("Create socket success!");

    socketValid = true;
    return true;
}

bool RSocket::bind(const char *ip, unsigned short port)
{
    if(!isValid())
        return false;

    sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = inet_addr(ip);

    int ret = ::bind(tcpSocket,(sockaddr*)&saddr,sizeof(saddr));
    if(ret == SOCKET_ERROR)
    {
        closeSocket();
        errorCode = WSAGetLastError();
        RLOG_ERROR("Bind socket error [%s:%d] [ErrorCode:%d]",ip,port,errorCode);
        return false;
    }

    RLOG_INFO("Bind socket success!");

    strcpy(socketIp,ip);
    socketPort = port;

    return true;
}

bool RSocket::listen()
{
    if(!isValid())
        return false;

    int ret = ::listen(tcpSocket,10);
    if(ret == SOCKET_ERROR)
    {
        closeSocket();
        errorCode = WSAGetLastError();
        RLOG_ERROR("Listen socket error! [ErrorCode:%d]",errorCode);
        return false;
    }

    RLOG_INFO("Listen socket success!");

    return true;
}

bool RSocket::closeSocket()
{
    if(isValid())
    {
        if(closesocket(tcpSocket) == 0)
        {
            socketValid = false;
            tcpSocket = 0;
            return true;
        }
#ifdef Q_OS_WIN
        RLOG_ERROR("Close socket error [ErrorCode:%d]!",WSAGetLastError());
#endif
    }
    return false;
}

/*!
     * @brief 作为server端接收客户端连接
     * @param[in] 无
     * @return 返回接收的socket描述信息
     */
RSocket RSocket::accept()
{
    RSocket tmpSocket;
    if(!isValid())
        return tmpSocket;

    sockaddr_in clientAddr;
    int len = sizeof(clientAddr);

    int clientSocket = ::accept(tcpSocket,(sockaddr*)&clientAddr,(socklen_t*)&len);
    if(clientSocket == INVALID_SOCKET)
    {
        RLOG_ERROR("Accept failed [ErrorCode:%d]!",GetLastError());
        return tmpSocket;
    }

    strcpy(tmpSocket.socketIp,inet_ntoa(clientAddr.sin_addr));
    tmpSocket.socketValid = true;
    tmpSocket.socketPort= ntohs(clientAddr.sin_port);
    tmpSocket.tcpSocket = clientSocket;

    RLOG_INFO("Recv socket [%s:%d]",tmpSocket.socketIp,tmpSocket.socketPort);

    return tmpSocket;
}

/*!
     * @brief 接收数据，并将结果保存至缓冲区
     * @param[out] buff 保存并返回接收的数据
     * @param[in] length 缓冲区数据长度
     * @return 实际接收数据的长度
     */
int RSocket::recv(char *buff, int length)
{
    if(!isValid() || buff == NULL){
        return -1;
    }

    size_t buffLen = strlen(buff);
    memset(buff,0,buffLen);

    int ret = ::recv(tcpSocket,buff,length,0);

    return ret;
}

/*!
     * @brief 发送一定长度多少护具
     * @param[in] buff 待发送数据的缓冲区
     * @param[in] length 待发送的长度
     * @return 是否发送成功
     */
int RSocket::send(const char *buff, const int length)
{
    if(!isValid() || buff == NULL)
        return -1;

    int sendLen = 0;
    while(sendLen != length)
    {
        int ret = ::send(tcpSocket,buff+sendLen,length-sendLen,0);
        if (ret <= 0)
        {
            qDebug()<<GetLastError();
            sendLen = -1;
            break;
        }
        sendLen += ret;
    }
    return sendLen;
}

/*!
     * @brief 连接socket
     * @param[in] remoteIp 远程IP
     * @param[in] remotePort 远程端口
     * @param[in] teimeouts 超时时间
     * @return 连接是否成功
     */
bool RSocket::connect(const char *remoteIp, const unsigned short remotePort, int timeouts)
{
    if(!isValid())
        return false;

    sockaddr_in remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(remotePort);
    remoteAddr.sin_addr.s_addr = inet_addr(remoteIp);

    setBlock(false);

    if(::connect(tcpSocket,(sockaddr*)&remoteAddr,sizeof(remoteAddr)) != 0)
    {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(tcpSocket,&set);
        timeval tm;
        tm.tv_sec = timeouts;
        tm.tv_usec = 0;
        if(select(tcpSocket+1,0,&set,0,&tm) <= 0)
        {
            RLOG_ERROR("Connect timeout or error [%s:%d] %s ",remoteIp,remotePort,strerror(errno));
            return false;
        }
    }

    setBlock(true);
    RLOG_INFO("connect %s:%d success!\n",remoteIp,remotePort);
    return true;
}

/*!
     * @brief 设置socket是否为阻塞模式
     * @param[in] flag 状态
     * @return 返回设置的是否成功设置
     */
bool RSocket::setBlock(bool flag)
{
    if(!isValid())
        return false;

#if defined(Q_OS_WIN)
    unsigned long ul = 0;
    if(!flag)
    {
        ul = 1;
    }
    ioctlsocket(tcpSocket,FIONBIO,&ul);
#else defined()
    int flags = fcntl(tcpSocket,F_GETFL,0);
    if(flags<0)
    {
        return false;
    }

    if(flag)
    {
        flags = flags&~O_NONBLOCK;
    }
    else
    {
        flags = flags|O_NONBLOCK;
    }
    if(fcntl(tcpSocket,F_SETFL,flags)!=0)
    {
        return false;
    }
#endif
    return true;
}

/*!
 * @brief 设置socket的属性
 * @param[in] level sock级别：SOL_SOCKET
 * @param[in] optname 属性名
 * @param[in] optval 属性值
 * @param[in] optlen 属性长度
 * @return 执行结果，无错误为0，其它为对应的错误，参考https://msdn.microsoft.com/en-us/library/windows/desktop/ms740476(v=vs.85).aspx
 */
int RSocket::setSockopt(int optname, const char *optval, int optlen)
{
    if(!isValid())
        return false;

    return setsockopt(tcpSocket,SOL_SOCKET,optname,optval,optlen);
}

int RSocket::getLastError()
{
    return WSAGetLastError();
}

}//namespace ServerNetwork
