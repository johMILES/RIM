/*!
 *  @brief     通用组件窗口类
 *  @details   定义了组件对组件管理者暴露接口
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.24
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RCOMPONENT_H
#define RCOMPONENT_H

#include <QDockWidget>

#include "actionmanager/id.h"
#include "pluginmanager/observer.h"
#include "base_global.h"

class BASESHARED_EXPORT RComponent : public QDockWidget,public Observer
{
    Q_OBJECT
public:
    RComponent(const char * id,QWidget * parent  = 0);
    virtual ~RComponent();

    Id id();
    QString name();

    virtual bool initialize() = 0;
    virtual void release() = 0;

    virtual void onMessage(MessageType::MessageType type) = 0;

protected:
    Id m_id;
    QString m_name;

};

#endif // RCOMPONENT_H
