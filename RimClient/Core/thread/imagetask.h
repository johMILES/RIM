﻿/*!
 *  @brief     图片发送任务
 *  @details   用户上传、下载不同图片。
 *  @file      imagetask.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.02.28
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef IMAGETASK_H
#define IMAGETASK_H

#include <QQueue>
#include <QWaitCondition>
#include <QMutex>

#include "Network/rtask.h"

#include "protocoldata.h"
using namespace ProtocolType;

class ImageTask : public ClientNetwork::RTask
{
    Q_OBJECT
public:
    ImageTask();

    static ImageTask * instance();

    bool addItem(FileItemDesc * item);

    void startMe();
    void stopMe();

protected:
    void run();

private:
    void handleItem(FileItemDesc * request);

private:
    static ImageTask * imageTask;

    QQueue<FileItemDesc *> processItems;
    QWaitCondition waitCondition;
    QMutex mutex;
};

#endif // IMAGETASK_H
