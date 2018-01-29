﻿/*!
 *  @brief     数据处理中转
 *  @details   处理接收数据，并将结果分发至对应的页面
 *  @file      dataprocess.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.01.17
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note   20180124:wey:增加更新个人基本信息
 */
#ifndef DATAPROCESS_H
#define DATAPROCESS_H

#include <QJsonObject>

class DataProcess
{
public:
    DataProcess();

    void proRegistResponse(QJsonObject &data);
    void proLoginResponse(QJsonObject &data);
    void proUpdateBaseInfoResponse(QJsonObject &data);
    void proSearchFriendResponse(QJsonObject &data);
    void proAddFriendResponse(QJsonObject &data);
    void proOperateFriendResponse(QJsonObject &data);
    void proFriendListResponse(QJsonObject &data);

private:

};

#endif // DATAPROCESS_H
