/*!
 *  @brief     连接信息监视窗口
 *  @details   监视当前连接数、显示每个连接的基本信息(ip、port、出入信息(数据包、字节数))、路由信息、以及错误信息
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.24
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef CONNECTIONDOCKWIDGET_H
#define CONNECTIONDOCKWIDGET_H

#include "../rcomponent.h"

class ConnectionPanel : public RComponent
{
    Q_OBJECT
public:
    ConnectionPanel(QWidget * parent = 0);
    ~ConnectionPanel();

    bool initialize();
    void release();

    void onMessage(Datastruct::MessageType type);

protected:
    void resizeEvent(QResizeEvent * event);

private:

};

#endif // CONNECTIONDOCKWIDGET_H
