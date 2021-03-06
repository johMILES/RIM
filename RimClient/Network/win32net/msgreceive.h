﻿/*!
 *  @brief     文本信息接收
 *  @details   接收文本信息，接收后直接保存由其它程序处理 \n
 *             为了满足不同通信信道接收数据问题，接收线程【绑定】一种接收方式。绑定后，线程工作后，由Transmit负责最终的接收(包括组包、重传等) \n
 *             接收到数据根据接收信道，压入接收队列，由应用层统一处理。
 *  @file      tcpmsgreceive.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.01.06
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note   20180123:wey:调整客户端支持分包接收、组包
 *          20180305:wey:调整接收结构，RecveiveTask完全封装数据组包接收，子类负责对数据进行处理
 */
#ifndef TCPMSGRECEIVE_H
#define TCPMSGRECEIVE_H

#include "../network_global.h"
#include "../rtask.h"
#include "../rsocket.h"
#include <memory>

namespace ClientNetwork{

class BaseTransmit;

class NETWORKSHARED_EXPORT DataRecveiveTask : public RTask
{
    Q_OBJECT
public:
    explicit DataRecveiveTask(QObject *parent = 0);
    virtual ~DataRecveiveTask();

    void bindTransmit(std::shared_ptr<BaseTransmit> trans);
    void registDataHandle(DataHandler handler);

    void startMe();
    void stopMe();

signals:
    void socketError(CommMethod method);

protected:
    void run();
    void processData(RecvUnit & data);

protected:
    QString errorString;
    std::shared_ptr<BaseTransmit> transmit;
    DataHandler  dataHandler;
};

} //ClientNetwork

#endif // TCPMSGRECEIVE_H
