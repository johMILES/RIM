﻿/*!
 *  @brief     通知信息窗口
 *  @details   1.接收各种不同的消息，以列表形式展现；
 *             2.点击子条目，可以打开对应的信息窗口,包括：聊天窗口、系统通知窗口等
 *  @file      notifywindow.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.01.26
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note      20180127:wey:添加状态机显示，并支持双击查看通知信息；
 */
#ifndef NOTIFYWINDOW_H
#define NOTIFYWINDOW_H

#include "widget.h"
#include "datastruct.h"
#include "observer.h"

class NotifyWindowPrivate;
class ToolItem;

class NotifyWindow : public Widget ,public Observer
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(NotifyWindow)
public:
    explicit NotifyWindow(QWidget * parent = 0);
    ~NotifyWindow();

    void addNotifyInfo(NotifyInfo info);

    void showMe();
    void hideMe();

    void onMessage(MessageType type);

protected:
    void resizeEvent(QResizeEvent *);

signals:
    void showWindow();
    void hideWindow();
    void showSystemNotifyInfo(NotifyInfo info);

private slots:
    void viewAll();
    void ignoreAll();
    void viewNotify(ToolItem * item);

private:
    NotifyWindowPrivate * d_ptr;
};

#endif // NOTIFYWINDOW_H
