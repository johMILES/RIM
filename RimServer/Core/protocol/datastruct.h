﻿/*!
 *  @brief     类型定义
 *  @details   定义了通用的结构体、枚举等变量
 *  @file      datastruct.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.03.02
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QString>
#include <QVector>

namespace Datastruct {

/*!
 *  @brief 系统支持的服务类型
 *  @details 用户以命令行方式输入对应的服务.
 */
enum ServiceType
{
    SERVICE_TEXT,               /*!< 使用传输文本服务,默认类型 */
    SERVICE_FILE                /*!< 使用传输文件服务 */
};

/*!
 *  @brief 传输模式
 */
enum TransmissionMode
{
    TRANS_NET,              /*!< 网络传输方式 */
    TRANS_TEL               /*!< 电话传输方式 */
};

/*!
 *  @brief 数据库类型
 */
enum DatabaseType
{
    DB_MYSQL,               /*!< 使用MySql数据库*/
    DB_ORACLE               /*!< 使用Oracle数据库*/
};

/*!
 * @brief 解析结果
 */
enum ResultType
{
    PARSE_ERROR,            /*!< 发生错误*/
    VIEW_PROGRAM,           /*!< 查看程序信息*/
    EXEC_PROGRAM            /*!< 执行程序*/
};

struct CommandParameter
{
    CommandParameter()
    {
        serviceType = SERVICE_TEXT;
        transMode = TRANS_NET;
        dbType = DB_MYSQL;
        parseResult = PARSE_ERROR;
    }

    ServiceType serviceType;
    TransmissionMode  transMode;
    DatabaseType dbType;
    ResultType parseResult;
};

struct FileItemInfo
{
    QString fileName;               /*!< 文件名称 @attention 维护文件真实的信息 */
    size_t size;                    /*!< 文件大小 */
    QString fileId;                 /*!< 文件数据库中唯一标识 */
    QString md5;                    /*!< 文件MD5 @attention 服务器以此文件名作为在磁盘中保存的索引 */
    QString filePath;               /*!< 文件保存的路径 */
    QString accountId;              /*!< 发送方的ID */
    QString otherId;                /*!< 接收方ID */
};

} //namespace Datastruct

#ifdef __LOCAL_CONTACT__
/*************************参数配置文件*********************************/
/*!
 *  @brief 参数配置.txt
 */
namespace ParameterSettings {

/*!
 *  @brief  通信方式
 */
enum CommucationMethod{
    C_None = 0x00,
    C_NetWork  = 0x01,      /*!< 网络 */
    C_905_DataChain,        /*!< 905数据链 */
    C_Jids,                 /*!< JIDS */
    C_TongKong              /*!< 通控器 */
};

/*!
 *  @brief  报文格式
 */
enum MessageFormat{
    M_NONE = 0x00,
    M_6670 = 0x01,
    M_205,
    M_QDB12,
    M_495,
    M_905,
    M_JIDS
};

/*!
 *  @brief  基本信息
 */
struct BaseInfo{
    QString nodeId;             /*!< 本节点号 */
    QString localIp;            /*!< 本机IP地址 */
    QString lon;                /*!< 节点经度 */
    QString lat;                /*!< 节点纬度 */
};

/*!
 *  @brief  网络参数
 */
struct NetParamSetting{
    unsigned short port;        /*!< 收发端口 */
    unsigned short backPort;    /*!< 收发辅助端口 */
    unsigned long maxSegment;   /*!< 发送报文最大长度 */
    unsigned short delayTime;   /*!< 网络发送延时 */
};

/*!
 *  @brief 信源
 */
struct MessSource{
    QString nodeName;           /*!< 节点名 */
    QString nodeId;             /*!< 节点号 */
    QString ip;                 /*!< ip地址 */
    unsigned short encryption;  /*!< 加密标识 */
    unsigned short priority;    /*!< 发送优先级 */
    unsigned short connectTimeOut;  /*!< 建链初始时延 */
    QString protocol;           /*!< 传输协议 */
    unsigned short port;        /*!< 端口号 */
};

/*!
 *  @brief  网络收发配置
 */
struct NetSiteSettings{
    NetParamSetting netParamSetting;    /*!< 网络参数 */
    QVector<MessSource> sites;          /*!< 网络信源 */
};

/*!
 *  @brief  传输配置-条目
 */
struct TrasmitSetting{
    QString nodeId;                           /*!< 节点号 */
    QString protocol;                         /*!< 传输协议 暂不用*/
    MessageFormat messageFormat;              /*!< 报文格式 */
};

/*!
 *  @brief  通信控制器
 */
struct CommucationControl{
    QString ip;                         /*!< 通控器IP */
    unsigned short port;                /*!< 通控器端口 */
    QString emualteIp;                  /*!< 模拟器IP */
    unsigned short emulatePort;         /*!< 模拟器端口 */
    QVector<TrasmitSetting> transmitSettings;       /*!< 传输配置 */
};

/*!
 *  @brief  外发信息配置条目
 */
struct OuterNetConfig{
    QString nodeId;                           /*!< 节点号 */
    QString channel;                          /*!< 通道 */
    CommucationMethod communicationMethod;    /*!< 通信方式 */
    MessageFormat messageFormat;              /*!< 报文格式 */
    QString distributeMessageType;            /*!< 下发报文类别 */
};

/*!
 *  @brief  信息收发参数配置
 */
struct ParaSettings{
    BaseInfo baseInfo;                          /*!< 基本信息 */
    NetSiteSettings netSites;                   /*!< 网络收发配置 */
    CommucationControl commControl;             /*!< 通信控制器 */
    QVector<OuterNetConfig> outerNetConfig;     /*!< 外发信息配置 */
};

}

#endif  //namespace Datastruct

#endif // DATASTRUCT_H