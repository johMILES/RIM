﻿/*!
 *  @brief     文件描述
 *  @details   用于描述的上传/下载文件信息
 *  @file      filedesc.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.03.20
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef FILEDESC_H
#define FILEDESC_H

#include <QString>
#include <QFile>

class FileDesc
{
public:
    explicit FileDesc();
     ~FileDesc();

    void setFilePath(const QString filePath);

    bool isOpen();
    bool create();
    bool seek(size_t pos);
    bool flush();
    bool isRecvOver();

    qint64 write(const QByteArray &data);
    qint64 getWriteSize(){return writeLen;}
    qint64 fileSize(){return size;}

    void close();
    void destory();

public:
    int itemType;                        /*!< 文件操作类型 @link FileItemType @endlink */
    int itemKind;                        /*!< 文件类型 @link FileItemKind @endlink */
    qint64 size;                         /*!< 文件大小 */
    qint64 writeLen;                     /*!< 文件已经写入的大小 */
    QString fileName;                    /*!< 文件名称 @attention 维护文件真实的信息 */
    QString md5;                         /*!< 文件MD5 */
    QString fileId;                      /*!< 文件唯一ID */
    QString accountId;                   /*!< 自己ID */
    QString otherId;                     /*!< 对方ID */
    QString filePath;                    /*!< 1.上传时保存上传文件的路径信息；2.下载时保存文件的路径，默认保存至用户文件夹的接收目录下，可修改 */
    QFile   file;                        /*!< 下载时作为本地文件句柄 */

#ifdef __LOCAL_CONTACT__
    int cdate;                           /*!< 日期 */
    int ctime;                           /*!< 时间 */
    unsigned short usSerialNo;           /*!< 流水号*/
    unsigned short usOrderNo;            /*!< 协议号*/
    unsigned char bPackType;             /*!< 报文类型 */
#endif

    friend class FileManager;
};

#endif // FILEDESC_H
