﻿#include "binary716_msgparse.h"

#include "rsingleton.h"
#include "../msgprocess/data716process.h"
#include "Network/wraprule/tcp_wraprule.h"

#include <QDebug>

#ifdef __LOCAL_CONTACT__

Binary716_MsgParse::Binary716_MsgParse():
    DataParse()
{

}

/*!
 * @brief 处理网络层接收的数据信息
 * @note 此方法作为处理网络层数据的入口，具备对文本信息和文件信息分发处理的能力。 \n
 *       1.处理文本信息：分析发送的目的节点，根据离/在线情况、是否为同一节点进行不同处理 \n
 *       2.处理文件信息：将数据写入本地磁盘，并记录已经接收的状态，并周期性回复一定的状态信息 \n
 * @param[in] db 数据库连接
 * @param[in] data 待处理的是数据单元
 */
void Binary716_MsgParse::processData(Database *db, const RecvUnit &unit)
{
    ProtocolPackage packData;
    if(RSingleton<ServerNetwork::TCP_WrapRule>::instance()->unwrap(unit.data,packData)){
        packData.bPackType = unit.extendData.type495;
        packData.bPeserve = unit.extendData.bPeserve;
        //dwPackAllLen=分片数量*(sizeof(495)+sizeof(21)+sizeof(2051)+文件名长度)+data;注意不是文件的大小
        if(packData.usOrderNo == O_2051){
            packData.dwPackAllLen = unit.extendData.dwPackAllLen - sizeof(QDB21::QDB21_Head) - sizeof(QDB2051::QDB2051_Head);
        }else if(packData.usOrderNo == O_2048){
            packData.dwPackAllLen = unit.extendData.dwPackAllLen - sizeof(QDB21::QDB21_Head) - sizeof(QDB2048::QDB2048_Head);
        }

        //文本信息
        if(packData.cFileType == QDB2051::F_NO_SUFFIX){
            switch(unit.extendData.type495){
                case T_DATA_AFFIRM:
                case T_DATA_NOAFFIRM:
                    RSingleton<Data716Process>::instance()->processText(db,unit.extendData.sockId,packData);
                    break;
                case T_DATA_REG:
                    RSingleton<Data716Process>::instance()->processUserRegist(db,unit.extendData.sockId,packData);
                    break;

                default:break;
            }
        }
        //文件信息
        else if(packData.cFileType == QDB2051::F_TEXT || packData.cFileType == QDB2051::F_BINARY){
            RSingleton<Data716Process>::instance()->processFileData(db,unit.extendData.sockId,packData);
        }
    }
}

#endif
