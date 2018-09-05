#include "pluginmanager.h"

#include "../rcomponent.h"

PluginManager::PluginManager(QObject *parent):
    QObject(parent)
{

}

//TODO 加载插件
void PluginManager::load()
{

}

void PluginManager::addPlugin(RComponent *component)
{
    if(component == NULL)
        return;

    std::lock_guard<std::mutex> lg(m_mutex);
    if(componentMap.contains(component->id()))
        return;

    componentMap.insert(component->id(),component);
}

PluginManager::ComponentMap PluginManager::plugins()
{
    std::lock_guard<std::mutex> lg(m_mutex);
    return componentMap;
}
