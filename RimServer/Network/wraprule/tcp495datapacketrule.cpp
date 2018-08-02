﻿#include "tcp495datapacketrule.h"

#ifdef __LOCAL_CONTACT__

#include <qmath.h>

#include "head.h"
#include "../win32net/iocpcontext.h"
#include "../dataprocess/handler.h"
#include "Util/rsingleton.h"
#include "Util/scaleswitcher.h"
#include "tcp_wraprule.h"

namespace ServerNetwork{

using namespace QDB495;
using namespace QDB21;
using namespace QDB2051;
using namespace QDB2048;

TCP495DataPacketRule::TCP495DataPacketRule():
    WrapRule(),textHandler(nullptr)
{
    SendPackId = qrand()%1024;
}

void TCP495DataPacketRule::registTextHandler(Handler *dataHandler)
{
    textHandler = dataHandler;
}

/*!
 * @brief 对接收数据进行接受前处理，检查之前是否有断包缓存，若有则先将之前数据拼接在当前数据之前。
 * @param[in] context 接收数据入口
 * @param[in] recvLen 数据接收长度
 */
void TCP495DataPacketRule::bindContext(IocpContext * context, unsigned long recvLen)
{
    ioContext = context;

    ioContext->getPakcet()[recvLen] = 0;
    int lastRecvBuffSize = ioContext->getClient()->getHalfPacketBuff().size();
    if(lastRecvBuffSize > 0)
    {
        int tmpBuffLen = lastRecvBuffSize + recvLen + 1;
        char * dataBuff = new char[tmpBuffLen];
        memset(dataBuff,0,tmpBuffLen * sizeof(char));

        memcpy(dataBuff,ioContext->getClient()->getHalfPacketBuff().data(),lastRecvBuffSize);
        memcpy(dataBuff + lastRecvBuffSize,ioContext->getPakcet(),recvLen);

        ioContext->getClient()->getHalfPacketBuff().clear();

        recvData(dataBuff,lastRecvBuffSize + recvLen);

        delete[] dataBuff;
    }
    else
    {
        recvData(ioContext->getPakcet(),recvLen);
    }
}

/*!
 * @brief 使用iocp方式对原始数据添加协议，并且分片发送
 * @param[in] dunit 待发送给的数据单元
 * @param[in] sendFunc 发送回调函数
 * @return 是否发送成功
 */
bool TCP495DataPacketRule::wrap(SendUnit &dunit, IocpContextSender sendFunc)
{
    TcpClient * client = TcpClientManager::instance()->getClient(dunit.sockId);
    if(client != NULL)
    {
        QDB495_SendPackage packet;
        memset((char *)&packet,0,sizeof(QDB495_SendPackage));
        packet.bVersion = 1;
        packet.bPackType = dunit.dataUnit.bPackType;
        packet.bPriority = 0;
        packet.bPeserve = dunit.dataUnit.bPeserve;
        packet.wSerialNo = ScaleSwitcher::htons(dunit.dataUnit.usSerialNo);
        packet.wCheckout = ScaleSwitcher::htons(0);
        packet.wDestAddr = ScaleSwitcher::htons(dunit.dataUnit.wDestAddr);
        packet.wSourceAddr = ScaleSwitcher::htons(dunit.dataUnit.wSourceAddr);

        int sendDataLen = 0;

        //文件数据，每一片文件数据均小于最大传输限制
        if(dunit.dataUnit.cFileType == QDB2051::F_BINARY){
            int protocolDataLen = 0;
            if(dunit.dataUnit.wOffset == 0)
            {
                if(dunit.dataUnit.usOrderNo == O_2051){
                    protocolDataLen = QDB21_Head_Length + QDB2051_Head_Length + dunit.dataUnit.cFilename.length();
                }
                RSingleton<TCP_WrapRule>::instance()->wrap(dunit.dataUnit);
            }
            packet.wPackLen = ScaleSwitcher::htons(dunit.dataUnit.data.length());

            packet.dwPackAllLen = ScaleSwitcher::htonl(dunit.dataUnit.dwPackAllLen + protocolDataLen);
            packet.wOffset = ScaleSwitcher::htons(dunit.dataUnit.wOffset);

            unsigned short currPackTotalLen = dunit.dataUnit.data.length() + sizeof(QDB495_SendPackage);

            IocpContext * context = IocpContext::create(IocpType::IOCP_SEND,client);
            memcpy(context->getPakcet(),(char *)&packet,sizeof(QDB495_SendPackage));
            memcpy(context->getPakcet()+ sizeof(QDB495_SendPackage),dunit.dataUnit.data.data(),dunit.dataUnit.data.length());

            context->getWSABUF().len = currPackTotalLen;

            DWORD realSendLen = 0;
            if(!sendFunc(dunit.sockId,context,realSendLen))
                return false;

            if(realSendLen == currPackTotalLen){
                return true;
            }
        }else if(dunit.dataUnit.cFileType == QDB2051::F_NO_SUFFIX || dunit.dataUnit.cFileType == QDB2051::F_TEXT){
            //多个协议头长度
            int protocolDataLen = 0;

            switch(dunit.dataUnit.usOrderNo){
                case O_2051:
                    protocolDataLen = QDB21_Head_Length + QDB2051_Head_Length;
                    break;
                case O_2048:
                    protocolDataLen = QDB21_Head_Length + QDB2048_Head_Length;
                    break;
                default:
                    break;
            }

            unsigned long packAllLen = dunit.dataUnit.data.length() + protocolDataLen;
            packet.dwPackAllLen = ScaleSwitcher::htonl(packAllLen);
            int totalIndex = countTotoalIndex(packAllLen);

            //添加21、2051协议头
            RSingleton<TCP_WrapRule>::instance()->wrap(dunit.dataUnit);

            QByteArray originalData = dunit.dataUnit.data;

            for(unsigned int i = 0; i < totalIndex; i++)
            {
                packet.wOffset = ScaleSwitcher::htons(i);

                int leftDataLen = packAllLen - sendDataLen;
                int sliceLen = leftDataLen > MAX_PACKET ? MAX_PACKET: leftDataLen;

                dunit.dataUnit.data.clear();
                dunit.dataUnit.data.append(originalData.mid(sendDataLen,sliceLen));

                packet.wPackLen = ScaleSwitcher::htons(sliceLen);
                unsigned short currPackTotalLen = sliceLen + sizeof(QDB495_SendPackage);

                IocpContext * context = IocpContext::create(IocpType::IOCP_SEND,client);
                memcpy(context->getPakcet(),(char *)&packet,sizeof(QDB495_SendPackage));
                memcpy(context->getPakcet()+ sizeof(QDB495_SendPackage),dunit.dataUnit.data.data(),dunit.dataUnit.data.length());

                context->getWSABUF().len = currPackTotalLen;

                DWORD realSendLen = 0;
                if(!sendFunc(dunit.sockId,context,realSendLen))
                    return false;

                if(realSendLen == currPackTotalLen){
                    sendDataLen += (sliceLen);
                }else{
                    break;
                }
            }

            if(sendDataLen == originalData.length()){
                return true;
            }
        }
    }

    return false;
}

bool TCP495DataPacketRule::wrap(SendUnit &dunit, ByteSender sendFunc)
{
    QDB495_SendPackage packet;
    memset((char *)&packet,0,sizeof(QDB495_SendPackage));
    packet.bVersion = 1;
    packet.bPackType = dunit.dataUnit.bPackType;
    packet.bPriority = 0;
    packet.bPeserve = dunit.dataUnit.bPeserve;
    packet.wSerialNo = ScaleSwitcher::htons(dunit.dataUnit.usSerialNo);
    packet.wCheckout = ScaleSwitcher::htons(0);
    packet.wDestAddr = ScaleSwitcher::htons(dunit.dataUnit.wDestAddr);
    packet.wSourceAddr = ScaleSwitcher::htons(dunit.dataUnit.wSourceAddr);

    int sendDataLen = 0;

    //文件数据，每一片文件数据均小于最大传输限制
    if(dunit.dataUnit.cFileType == QDB2051::F_BINARY){
        int protocolDataLen = 0;
        if(dunit.dataUnit.wOffset == 0){
            if(dunit.dataUnit.usOrderNo == O_2051){
                protocolDataLen = QDB21_Head_Length + QDB2051_Head_Length + dunit.dataUnit.cFilename.length();
            }
            RSingleton<TCP_WrapRule>::instance()->wrap(dunit.dataUnit);
        }
        packet.wPackLen = ScaleSwitcher::htons(dunit.dataUnit.data.length());
        packet.dwPackAllLen = ScaleSwitcher::htonl(dunit.dataUnit.dwPackAllLen + protocolDataLen);
        packet.wOffset = ScaleSwitcher::htons(dunit.dataUnit.wOffset);

        memset(sendBuff,0,TCP_SEND_BUFF);
        memcpy(sendBuff,(char *)&packet,sizeof(QDB495_SendPackage));
        memcpy(sendBuff + sizeof(QDB495_SendPackage),dunit.dataUnit.data.data(),dunit.dataUnit.data.length());

        unsigned short currPackTotalLen = dunit.dataUnit.data.length() + sizeof(QDB495_SendPackage);

        int realSendLen = sendFunc(dunit.sockId,sendBuff,currPackTotalLen);

        if(realSendLen == currPackTotalLen){
            return true;
        }
    }else if(dunit.dataUnit.cFileType == QDB2051::F_NO_SUFFIX || dunit.dataUnit.cFileType == QDB2051::F_TEXT){
        //多个协议头长度
        int protocolDataLen = 0;

        switch(dunit.dataUnit.usOrderNo){
            case O_2051:
                protocolDataLen = QDB21_Head_Length + QDB2051_Head_Length;
                break;
            case O_2048:
                protocolDataLen = QDB21_Head_Length + QDB2048_Head_Length;
                break;
            default:
                break;
        }

        unsigned long packAllLen = dunit.dataUnit.data.length() + protocolDataLen;
        packet.dwPackAllLen = ScaleSwitcher::htonl(packAllLen);
        int totalIndex = countTotoalIndex(packAllLen);

        //添加21、2051协议头
        RSingleton<TCP_WrapRule>::instance()->wrap(dunit.dataUnit);

        QByteArray originalData = dunit.dataUnit.data;

        for(unsigned int i = 0; i < totalIndex; i++)
        {
            packet.wOffset = ScaleSwitcher::htons(i);

            int leftDataLen = packAllLen - sendDataLen;
            int sliceLen = leftDataLen > MAX_PACKET ? MAX_PACKET: leftDataLen;

            dunit.dataUnit.data.clear();
            dunit.dataUnit.data.append(originalData.mid(sendDataLen,sliceLen));

            packet.wPackLen = ScaleSwitcher::htons(sliceLen);
            unsigned short currPackTotalLen = packet.wPackLen + sizeof(QDB495_SendPackage);

            memset(sendBuff,0,TCP_SEND_BUFF);
            memcpy(sendBuff,(char *)&packet,sizeof(QDB495_SendPackage));
            memcpy(sendBuff + sizeof(QDB495_SendPackage),dunit.dataUnit.data.data(),dunit.dataUnit.data.length());

            int realSendLen = sendFunc(dunit.sockId,sendBuff,currPackTotalLen);

            if(realSendLen == currPackTotalLen){
                sendDataLen += sliceLen;
            }else{
                break;
            }
        }

        if(sendDataLen == originalData.length()){
            return true;
        }
    }

    return false;
}

/*!
 * @brief 计算数据总分包数量
 * @note 2048协议头没有传输内容，因此要手动的置为1
 * @param[in] totalLength 数据总长度
 * @return 总分包大小
 */
int TCP495DataPacketRule::countTotoalIndex(const int totalLength)
{
    if(totalLength == 0)
        return 1;
    return qCeil(((float) totalLength/ (MAX_PACKET)));
}

void TCP495DataPacketRule::wrap(ProtocolPackage &data)
{
    Q_UNUSED(data)
}

bool TCP495DataPacketRule::unwrap(const QByteArray &data, ProtocolPackage &result)
{
    Q_UNUSED(data)
    Q_UNUSED(result)
    return false;
}

/*!
 * @brief 解析并处理数据，去除495传输控制协议(21、2051、2048等协议待应用层进行解析)
 * @note 根据495传输控制协议，将网络接收到的数据按照协议解析，并根据数据的类型分发处理; \n
 *       若2051协议中的fileType为0时，表示文本数据，则需要进行断包接收处理，并设立接收缓冲区; \n
 *       若2051协议中的fileType为1或2时，表示文件数据，则接收一包后直接交由应用层写入磁盘操作. \n
 * @param[in] recvData 接收到的网络数据
 * @param[in] recvLen 数据长度
 */
void TCP495DataPacketRule::recvData(const char *recvData, int recvLen)
{
    QDB495_SendPackage packet;
    memset((char *)&packet,0,sizeof(QDB495_SendPackage));
    QDB495_SendPackage packetOrigin;
    memset((char *)&packetOrigin,0,sizeof(QDB495_SendPackage));

    if(recvLen >= sizeof(QDB495_SendPackage))
    {
        int processLen = 0;
        do
        {
            memcpy((char *)&packetOrigin,recvData+processLen,sizeof(QDB495_SendPackage));
            memcpy((char *)&packet,recvData+processLen,sizeof(QDB495_SendPackage));
            processLen += sizeof(QDB495_SendPackage);
            //[1]数据头部分正常
            if(true)
            {
                packet.wPackLen = ScaleSwitcher::ntohs(packet.wPackLen);
                packet.wSerialNo = ScaleSwitcher::ntohs(packet.wSerialNo);
                packet.wCheckout =ScaleSwitcher:: ntohs(packet.wCheckout);
                packet.wOffset = ScaleSwitcher::ntohs(packet.wOffset);
                packet.dwPackAllLen = ScaleSwitcher::ntohl(packet.dwPackAllLen);
                packet.wDestAddr = ScaleSwitcher::ntohs(packet.wDestAddr);
                packet.wSourceAddr = ScaleSwitcher::ntohs(packet.wSourceAddr);

                RecvUnit socketData;
                socketData.extendData.method = C_TCP;
                socketData.extendData.sockId = ioContext->getClient()->socket();
                socketData.extendData.type495 = static_cast<PacketType_495>(packet.bPackType);
                socketData.extendData.bPeserve = packet.bPeserve;
                socketData.extendData.wOffset = packet.wOffset;
                socketData.extendData.dwPackAllLen = packet.dwPackAllLen;
                socketData.extendData.usSerialNo = packet.wSerialNo;
                socketData.extendData.wDestAddr = packet.wDestAddr;
                socketData.extendData.wSourceAddr = packet.wSourceAddr;

                //[1.1]至少存在多余一个完整数据包
                int currentDataPackLen = packet.wPackLen;
                if(currentDataPackLen <= recvLen - processLen)
                {
                    //若协议为2051需要对文件的类型进行判断，若为2048则直接默认以text形式发送
                    FileType ptype = QDB2051::F_NO_SUFFIX;

                    //第一包数据，包含21、2051，可判断是文本还是文件。若allpackLen>packlen，则保存此包的序列号。待下次接收数据后再做统一判断
                    if(packet.wOffset == 0)
                    {
                        if(socketData.extendData.type495 != T_DATA_REG)
                        {
                            //对数据包类型进行预判断处理
                            QDB21::QDB21_Head head21;
                            memset(&head21,0,sizeof(QDB21::QDB21_Head));
                            memcpy((char *)&head21,recvData + processLen,sizeof(QDB21::QDB21_Head));

                            if(head21.usOrderNo == O_2051){
                                QDB2051::QDB2051_Head head2051;
                                memset(&head2051,0,sizeof(QDB2051::QDB2051_Head));
                                memcpy((char *)&head2051,recvData + processLen + sizeof(QDB21::QDB21_Head),sizeof(QDB2051::QDB2051_Head));
                                ptype = static_cast<FileType>((head2051.cFileType));
                            }else if(head21.usOrderNo == O_2048){
                                //2048中不包含相关协议字段，暂不解析
                            }
                        }
                    }
                    else
                    {
                        if(!ioContext->getClient()->queryFiletype(RecvFileTypeId(packet.wSourceAddr,packet.wDestAddr,packet.wSerialNo),ptype))
                        {
                            //错误处理
                        }
                    }

                    if(ptype == QDB2051::F_TEXT || ptype == QDB2051::F_BINARY)
                    {
                        if(packet.wOffset == 0){
                           ioContext->getClient()->addFileId(RecvFileTypeId(RecvFileTypeId(packet.wSourceAddr,packet.wDestAddr,packet.wSerialNo,ptype)));
                        }
                        socketData.extendData.type = SOCKET_FILE;
                        socketData.data.resize(currentDataPackLen);
                        memcpy(socketData.data.data(),recvData + processLen,currentDataPackLen);
                        if(textHandler)
                            textHandler->handle(socketData);
                    }
                    else if(ptype == QDB2051::F_NO_SUFFIX)
                    {
                        //[1.1.1]一包数据
                        if(packet.dwPackAllLen == packet.wPackLen)
                        {
                            socketData.extendData.sliceNum = packet.wOffset + 1;
                            socketData.extendData.type = SOCKET_TEXT;
                            socketData.data.resize(currentDataPackLen);
                            memcpy(socketData.data.data(),recvData + processLen,currentDataPackLen);

                            if(textHandler)
                                textHandler->handle(socketData);
                        }
                        //[1.1.2]多包数据(保存除495头以外的部分)
                        else
                        {
                            QByteArray data;
                            data.resize(currentDataPackLen);

                            //currentDataPackLen的第一包长度=sizeof(21)+sizeof(2051)+真实数据长度，第二包开始currentDataPackLen=真实数据长度
                            memcpy(data.data(),ioContext->getPakcet() + processLen,currentDataPackLen);

                            std::unique_lock<std::mutex> ul(ioContext->getClient()->BuffMutex());
                            if(ioContext->getClient()->getPacketBuffs().value(packet.wSerialNo,NULL) == NULL)
                            {
                                PacketBuff * buff = new PacketBuff;
                                buff->totalPackLen = packet.dwPackAllLen;
                                buff->recvSize += packet.wPackLen;
                                buff->buff.append(data);

                                ioContext->getClient()->getPacketBuffs().insert(packet.wSerialNo,buff);
                                ioContext->getClient()->addFileId(RecvFileTypeId(RecvFileTypeId(packet.wSourceAddr,packet.wDestAddr,packet.wSerialNo,ptype)));
                            }
                            else
                            {
                                PacketBuff * buff = ioContext->getClient()->getPacketBuffs().value(packet.wSerialNo,NULL);
                                if(buff)
                                {
                                    buff->buff.append(data);
                                    buff->recvSize += packet.wPackLen;
                                    if(buff->recvSize == buff->totalPackLen)
                                    {
                                        buff->isCompleted = true;
                                        socketData.extendData.type = SOCKET_TEXT;
                                        socketData.extendData.sliceNum = packet.wOffset + 1;

                                        buff->packDataWidthPrtocol(socketData.data);

                                        ioContext->getClient()->getPacketBuffs().remove(packet.wSerialNo);
                                        delete buff;

                                        ioContext->getClient()->removeFileId(RecvFileTypeId(RecvFileTypeId(packet.wSourceAddr,packet.wDestAddr,packet.wSerialNo,ptype)));

                                        if(textHandler)
                                            textHandler->handle(socketData);
                                    }
                                }
                            }
                        }
                    }

                    processLen += currentDataPackLen;

                    //[1.1.3]检验是否满足下次处理需求
                    int leftLen = recvLen - processLen;
                    if(leftLen <= 0)
                    {
                        break;
                    }

                    if(leftLen >= sizeof(QDB495_SendPackage))
                    {
                        continue;
                    }
                    else
                    {
                        //[1.1.3.1]【信息被截断】
                        std::unique_lock<std::mutex> ul(ioContext->getClient()->BuffMutex());
                        ioContext->getClient()->getHalfPacketBuff().clear();
                        ioContext->getClient()->getHalfPacketBuff().append(recvData + processLen,leftLen);
                        processLen += leftLen;
                        break;
                    }
                }
                //[1.2]【信息被截断】
                else
                {
                    int leftLen = recvLen - processLen;

                    std::unique_lock<std::mutex> ul(ioContext->getClient()->BuffMutex());
                    ioContext->getClient()->getHalfPacketBuff().clear();
                    ioContext->getClient()->getHalfPacketBuff().append((char *)&packetOrigin,sizeof(QDB495_SendPackage));
                    ioContext->getClient()->getHalfPacketBuff().append(recvData+processLen,leftLen);
                    processLen += leftLen;
                    break;
                }
            }
            else
            {
                //TODO 对错误处理
            }
        }while(processLen <= recvLen);
    }
    else
    {
        std::unique_lock<std::mutex> ul(ioContext->getClient()->BuffMutex());
        ioContext->getClient()->getHalfPacketBuff().clear();
        ioContext->getClient()->getHalfPacketBuff().append((char *)recvData,recvLen);
    }
}

}

#endif
