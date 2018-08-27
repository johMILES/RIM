/*!
 *  @brief     基本信息监视窗口
 *  @details   用于显示服务器基本的状态信息，包含当前的ip、监听端口号、数据库配置信息
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.24
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef BASEINFODOCKWINDOW_H
#define BASEINFODOCKWINDOW_H

#include "../rcomponent.h"

class BaseInfoWidget;

class BaseInfoDockPanel : public RComponent
{
    Q_OBJECT
public:
    explicit BaseInfoDockPanel(QWidget * parent = 0);
    ~BaseInfoDockPanel();

    bool initialize();
    void release();

    void onMessage(Datastruct::MessageType type);

    QSize sizeHint()const;

private:
    BaseInfoWidget * infoWidget;

};

#endif // BASEINFODOCKWINDOW_H
