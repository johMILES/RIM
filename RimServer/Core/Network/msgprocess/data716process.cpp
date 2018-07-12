﻿#include "data716process.h"

#include <QDebug>
#include <thread>

#include "../msgwrap/localmsgwrap.h"
#include "rsingleton.h"
#include "Network/connection/tcpclient.h"
#include "Network/connection/seriesconnection.h"
#include "../../thread/netconnector.h"
#include "../../thread/filesendqueuethread.h"
#include "../../protocol/datastruct.h"
#include "../../sql/sqlprocess.h"
#include "global.h"
#include "Util/rutil.h"
#include "Util/rlog.h"

using namespace ParameterSettings;
using namespace ServerNetwork;

#ifdef __LOCAL_CONTACT__
#include <QDebug>

extern OuterNetConfig QueryNodeDescInfo(QString nodeId,bool & result);
extern NodeServer QueryServerDescInfo(QString nodeId,bool & result);

Data716Process::Data716Process()
{

}

/*!
 * @brief 处理不同的文字信息发送
 * @note  【!!服务器在接收数据后，只根据目的节点号来判断!!】 \n
 *
 *        1.若目的节点号为服务器所在的指挥中心:
 *           1.1.若源节点号不是服务器所在节点号，查找所有与服务器节点一致的客户端，并广播消息; \n
 *           1.2.若源节点号是服务器所在的节点号，通过节点号找到所有的客户端，并通过IP地址和端口号过滤掉发送源的节点号，对剩下的节点进行广播; \n
 *        2.若目的节点为非服务器所在的指挥所节点： \n
 *           2.1.查找目的节点是否在当前通信列表中，若不在则退出. \n
 *           2.2.若目的节点在当前软件通信列表中，查找目的节点所属服务器的节点： \n
 *              2.2.1.若所属服务器节点和本服务器节点一致，若在线则直接推送；若不在线，则离线缓存。 \n
 *              2.2.2.若不是同一服务器节点： \n
 *                  2.2.2.1.若对方服务器已经建立连接，则直接推送；
 *                  2.2.2.2.若对方服务器未建立连接，则
 *       3.以上的传输方式不局限于TCP，还可能为其它方式。 \n
 * @param[in] db 数据库连接
 * @param[in] sockId 网络连接
 * @param[in] data 数据信息描述
 */
void Data716Process::processText(Database *db, int sockId, ProtocolPackage &data)
{
    if(data.wDestAddr == RGlobal::G_ParaSettings->baseInfo.nodeId.toUShort()){
        ClientList clist = TcpClientManager::instance()->getClients(QString::number(data.wDestAddr));
        TcpClient * sourceClient = TcpClientManager::instance()->getClient(sockId);

        if(data.wSourceAddr == RGlobal::G_ParaSettings->baseInfo.nodeId.toUShort()){
            std::for_each(clist.begin(),clist.end(),[&](TcpClient * destClient){
                if(sourceClient && destClient->ip() != sourceClient->ip() && destClient->port() != sourceClient->port()){
                    RSingleton<LocalMsgWrap>::instance()->hanldeMsgProtcol(destClient->socket(),data);
                }
            });
        }
        else{
            if(clist.size() > 0){
                std::for_each(clist.begin(),clist.end(),[&](TcpClient * destClient){
                    RSingleton<LocalMsgWrap>::instance()->hanldeMsgProtcol(destClient->socket(),data);
                });
            }else{
                RSingleton<SQLProcess>::instance()->saveChat716Cache(db,data);
            }
        }
    }else{
        bool findNode = false;
        QueryNodeDescInfo(QString::number(data.wDestAddr),findNode);

        if(findNode){
            bool findServer = false;
            NodeServer serverInfo = QueryServerDescInfo(QString::number(data.wDestAddr),findServer);
            if(findServer){
                if(serverInfo.nodeId == RGlobal::G_ParaSettings->baseInfo.nodeId){
                    TcpClient * destClient = TcpClientManager::instance()->getClient(QString::number(data.wDestAddr));
                    if(destClient && ((OnlineStatus)destClient->getOnLineState() == STATUS_ONLINE)){
                        RSingleton<LocalMsgWrap>::instance()->hanldeMsgProtcol(destClient->socket(),data);
                    }else{
                        RSingleton<SQLProcess>::instance()->saveChat716Cache(db,data);
                    }
                }else{
                    std::shared_ptr<SeriesConnection> conn = SeriesConnectionManager::instance()->getConnection(serverInfo.nodeId);

                    if(conn.get() != nullptr){
                        RSingleton<LocalMsgWrap>::instance()->hanldeMsgProtcol(conn->socket(),data,false);
                    }else{
                        cacheMsgProtocol(serverInfo,data);
                    }
                }
            }else{
                RLOG_ERROR("can't find destination node parent node!");
            }
        }else{
            RLOG_ERROR("destination node isn't in paraseting file!");
        }
    }
}

/*!
 * @brief 处理用户注册
 * @note 接收用户消息后，若当前列表中无此客户端信息，则认为是注册信息 \n
 *          [1]:用户登陆后，自动拉取当前用户的离线消息，推送至客户端。
 *
 *       若当前用户列表中已经存在此客户端信息，则认为是注销信息.
 * @param[in] db 数据库连接
 * @param[in] sockId 网络连接
 * @param[in] data 数据信息描述
 */
void Data716Process::processUserRegist(Database *db, int sockId, ProtocolPackage &data)
{
    TcpClient * tmpClient = TcpClientManager::instance()->getClient(sockId);
    if(tmpClient){
        if((OnlineStatus)tmpClient->getOnLineState() == STATUS_OFFLINE){
            tmpClient->setAccount(QString::number(data.wSourceAddr));
            tmpClient->setOnLineState(STATUS_ONLINE);

            QList<ProtocolPackage> historyMsg;
            if(RSingleton<SQLProcess>::instance()->loadChat716Cache(db,data.wSourceAddr,historyMsg)){
                std::for_each(historyMsg.begin(),historyMsg.end(),[&](ProtocolPackage & data){
                    RSingleton<LocalMsgWrap>::instance()->hanldeMsgProtcol(sockId,data);
                });
            }
        }else{
            tmpClient->setOnLineState(STATUS_OFFLINE);
        }
    }
}

/*!
 * @brief 处理文件数据接收
 * @note 文件数据发送目前采用端-端传输方式，若A-S-B，那么文件传输为：A-S、S-B \n
 *       [1]文件接收由专门的文件管理器负责，提供对文件的查询、创建、保存等功能； \n
 *       [2]文件接收完成后，检查接收端是否在线，不在线则缓存记录，在线则直接传输。 \n
 * @param[in] db 数据库连接
 * @param[in] sockId 网络连接
 * @param[in] data 数据信息描述
 */
void Data716Process::processFileData(Database *db, int sockId, ProtocolPackage &data)
{
    TcpClient * client = TcpClientManager::instance()->getClient(sockId);
    if(client)
    {
        FileRecvDesc * desc = client->getFile(QString::fromLocal8Bit(data.cFilename));
        if(desc == nullptr){
            desc = new FileRecvDesc();
            desc->md5 = RUtil::UUID();
            desc->itemKind = (int)data.cFileType;
            desc->bPackType = data.bPackType;
            desc->fileName = QString::fromLocal8Bit(data.cFilename);
            desc->accountId = QString::number(data.wSourceAddr);
            desc->otherId = QString::number(data.wDestAddr);
            desc->usOrderNo = data.usOrderNo;
            desc->usSerialNo = data.usSerialNo;
            desc->size = data.dwPackAllLen;
            desc->writeLen = 0;
            if(!client->addFile(QString::fromLocal8Bit(data.cFilename),desc))
                return;
        }

        std::unique_lock<std::mutex> ul(desc->RWMutex());
        if(desc->state() == FILE_CANCEL)
            return;

        if(desc->isNull() && !desc->create(RGlobal::G_FILE_UPLOAD_PATH))
            return;

        if(desc->state() == FILE_TRANING || desc->state() == FILE_PAUSE){
            //TODO 20180708在ProtocolPackage中加入2051报文头中有关偏移量和数据总长度的标识
            if(desc->seek(0) && desc->write(data.data) > 0)
            {
                if(desc->flush() && desc->isRecvOver())
                {
                    desc->close();
                }
            }
        }

        if(desc->isRecvOver())
        {
            int saveFileId = -1;
            if(RSingleton<SQLProcess>::instance()->add716File(db,desc,saveFileId))
            {
                if(data.wDestAddr == RGlobal::G_ParaSettings->baseInfo.nodeId.toUShort()){
                    ClientList clist = TcpClientManager::instance()->getClients(QString::number(data.wDestAddr));
                    TcpClient * sourceClient = TcpClientManager::instance()->getClient(sockId);

                    if(data.wSourceAddr == RGlobal::G_ParaSettings->baseInfo.nodeId.toUShort()){
                        std::for_each(clist.begin(),clist.end(),[&](TcpClient * destClient){
                            if(sourceClient && destClient->ip() != sourceClient->ip() && destClient->port() != sourceClient->port()){
                                ParameterSettings::Simple716FileInfo fileInfo(destClient->socket(),QString::number(data.wDestAddr),saveFileId);
                                RSingleton<FileSendManager>::instance()->addFile(fileInfo);
                            }
                        });
                    }
                    else{
                        if(clist.size() > 0){
                            std::for_each(clist.begin(),clist.end(),[&](TcpClient * destClient){
                                ParameterSettings::Simple716FileInfo fileInfo(destClient->socket(),QString::number(data.wDestAddr),saveFileId);
                                RSingleton<FileSendManager>::instance()->addFile(fileInfo);
                            });
                        }else{
                            RSingleton<SQLProcess>::instance()->add716FileCache(db,desc->otherId,saveFileId);
                        }
                    }
                }else{
                    bool findNode = false;
                    QueryNodeDescInfo(QString::number(data.wDestAddr),findNode);

                    if(findNode){
                        bool findServer = false;
                        NodeServer serverInfo = QueryServerDescInfo(QString::number(data.wDestAddr),findServer);
                        if(findServer){
                            if(serverInfo.nodeId == RGlobal::G_ParaSettings->baseInfo.nodeId){
                                TcpClient * destClient = TcpClientManager::instance()->getClient(QString::number(data.wDestAddr));
                                if(destClient && ((OnlineStatus)destClient->getOnLineState() == STATUS_ONLINE)){
                                    ParameterSettings::Simple716FileInfo fileInfo(destClient->socket(),QString::number(data.wDestAddr),saveFileId);
                                    RSingleton<FileSendManager>::instance()->addFile(fileInfo);
                                }else{
                                    RSingleton<SQLProcess>::instance()->add716FileCache(db,desc->otherId,saveFileId);
                                }
                            }else{
                                std::shared_ptr<SeriesConnection> conn = SeriesConnectionManager::instance()->getConnection(serverInfo.nodeId);

                                if(conn.get() != nullptr){
                                    RSingleton<LocalMsgWrap>::instance()->hanldeMsgProtcol(conn->socket(),data,false);
                                }else{
                                    ParameterSettings::Simple716FileInfo fileInfo = {-1,desc->otherId,saveFileId};
                                    cacheFileProtocol(db,serverInfo,fileInfo);
                                }
                            }
                        }else{
                            RLOG_ERROR("can't find destination node parent node!");
                        }
                    }else{
                        RLOG_ERROR("destination node isn't in paraseting file!");
                    }
                }
            }else{
                RLOG_ERROR("Save transfered failed!");
            }
            client->removeFile(QString::fromLocal8Bit(data.cFilename));
        }
    }
}

/*!
 * @brief 缓存数据信息请求，执行网络连接请求操作。
 * @details 若未与对方服务器建立连接，则先将发往对方的数据信息进行缓存，然后发起连接请求。 \n
 *          若建立连接，则将缓存的数据信息通过网络发送至对方; \n
 *          若未建立连接，则将缓存的数据移动至数据库。
 * @param[in] NodeServer 服务器信息
 * @param[in] ProtocolPackage & 请求数据信息
 */
void Data716Process::cacheMsgProtocol(NodeServer serverInfo, ProtocolPackage &package)
{
    std::unique_lock<std::mutex> ul(textCacheMutex);

    auto index = textServerCache.find(serverInfo.nodeId);
    if(index != textServerCache.end()){
        (*index).second.msgCache.push_back(package);
        //TODO 20180704 若第一次未连接成功，会造成之后数据就不会主动发起连接请求
    }else{
        TextServerCacheInfo cacheInfo;
        cacheInfo.serverInfo = serverInfo;
        cacheInfo.msgCache.push_back(package);

        cacheInfo.connStats = SCS_IN;
        textServerCache.insert(std::pair<QString,TextServerCacheInfo>(serverInfo.nodeId,cacheInfo));

        NetFunc func = std::bind(&Data716Process::respTextNetConnectResult,this,
                                 std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
        std::thread connector(RunNetConnetor,func,serverInfo);
        connector.detach();
    }
}

/*!
 * @brief 响应文本网络连接结果
 * @param[in] nodeId 远端服务器节点
 * @param[in] connected 是否建立连接
 * @param[in] socketId 建立连接的socket访问标识
 */
void Data716Process::respTextNetConnectResult(QString nodeId,bool connected,int socketId)
{
    std::unique_lock<std::mutex> ul(textCacheMutex);

    auto index = textServerCache.find(nodeId);
    if(index != textServerCache.end()){
        if(connected){
            (*index).second.connStats = SCS_OK;

            std::for_each((*index).second.msgCache.begin(),(*index).second.msgCache.end(),[&](ProtocolPackage & package){
                RSingleton<LocalMsgWrap>::instance()->hanldeMsgProtcol(socketId,package,false);
            });

            (*index).second.msgCache.clear();
            textServerCache.erase(index);
        }else{
            (*index).second.connStats = SCS_ERR;
        }
    }
}

/*!
 * @brief 缓存文件数据信息请求，执行网络连接请求操作。
 * @details 若未与对方服务器建立连接，则先将发往对方的数据信息进行缓存，然后发起连接请求。 \n
 *          若建立连接，则将文件发送至对方; \n
 *          若未建立连接，则缓存发文件信息。
 * @param[in] NodeServer 服务器信息
 * @param[in] ProtocolPackage & 请求数据信息
 */
void Data716Process::cacheFileProtocol(Database * db,NodeServer serverInfo, Simple716FileInfo &fileIndo)
{
    std::unique_lock<std::mutex> ul(fileCacheMutex);

    auto index = fileServerCache.find(serverInfo.nodeId);
    if(index != fileServerCache.end()){
        (*index).second.fileCache.push_back(fileIndo);
        RSingleton<SQLProcess>::instance()->add716FileCache(db,fileIndo.nodeId,fileIndo.fileId);
    }else{
        FileServerCacheInfo cacheInfo;
        cacheInfo.serverInfo = serverInfo;
        cacheInfo.fileCache.push_back(fileIndo);
        cacheInfo.connStats = SCS_IN;
        fileServerCache.insert(std::pair<QString,FileServerCacheInfo>(serverInfo.nodeId,cacheInfo));

        NetFunc func = std::bind(&Data716Process::respFileNetConnectResult,this
                                 ,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
        std::thread connector(RunNetConnetor,func,serverInfo);
        connector.detach();
    }
}

/*!
 * @brief 响应文件网络连接结果
 * @param[in] nodeId 远端服务器节点
 * @param[in] connected 是否建立连接
 * @param[in] socketId 建立连接的socket访问标识
 */
void Data716Process::respFileNetConnectResult(QString nodeId, bool connected, int socketId)
{
    std::unique_lock<std::mutex> ul(fileCacheMutex);

    auto index = fileServerCache.find(nodeId);
    if(index != fileServerCache.end()){
        if(connected){
            (*index).second.connStats = SCS_OK;

            std::for_each((*index).second.fileCache.begin(),(*index).second.fileCache.end(),[&](Simple716FileInfo & fileInfo){
                ParameterSettings::Simple716FileInfo finfo(socketId,fileInfo.nodeId,fileInfo.fileId);
                RSingleton<FileSendManager>::instance()->addFile(finfo);
            });

            (*index).second.fileCache.clear();
            fileServerCache.erase(index);
        }else{
            (*index).second.connStats = SCS_ERR;
            saveFile2Database((*index).second.fileCache);
        }
    }
}

/*!
 * @brief 在主动连接对方服务器失败后，将文件请求的记录保存至数据库
 * @note 此方法是由单独的线程进行回调，因此数据库的连接需要打开一个临时的。
 * @param[in] recvFileCache 连接失败返回时刻数据库中已经存在的文件数据信息
 */
void Data716Process::saveFile2Database(std::vector<ParameterSettings::Simple716FileInfo> & recvFileCache)
{
    std::for_each(recvFileCache.begin(),recvFileCache.end(),[&](ParameterSettings::Simple716FileInfo & perFileInfo){
        RSingleton<SQLProcess>::instance()->add716FileCache(perFileInfo.nodeId,perFileInfo.fileId);
    });
}

#endif
