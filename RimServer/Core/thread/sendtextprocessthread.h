﻿/*!
 *  @brief     发送信息线程
 *  @details   统一向客户端发回处理信息
 *  @file      sendtextprocessthread.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.01.16
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef SENDTEXTPROCESSTHREAD_H
#define SENDTEXTPROCESSTHREAD_H

#include <QThread>

#include <map>
#include <memory>

#include "Network/head.h"
#include "Network/multitransmits/transmits.h"

class SendTextProcessThread : public QThread
{
    Q_OBJECT
public:
    SendTextProcessThread();
    ~SendTextProcessThread();

protected:
    void run();

private:
    void initTransmits();
    bool handleDataSend(SendUnit &unit);

private:
    bool runningFlag;

    ServerNetwork::Transmits transmits;
};

#endif // SENDTEXTPROCESSTHREAD_H
