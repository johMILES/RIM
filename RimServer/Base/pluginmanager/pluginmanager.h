/*!
 *  @brief     插件管理器
 *  @details   用于维护加载、初始化、卸载插件
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.24
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QMap>
#include <mutex>

#include "../actionmanager/id.h"

class  RComponent;

class BASESHARED_EXPORT PluginManager : public QObject
{
public:
    explicit PluginManager(QObject * parent = 0);

    typedef QMap<Id,RComponent *> ComponentMap;

    void load();

    void addPlugin(RComponent * component);
    ComponentMap plugins();

private:
    ComponentMap componentMap;
    std::mutex  m_mutex;
};

#endif // PLUGINMANAGER_H
