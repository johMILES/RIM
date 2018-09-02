#include "actionmanager.h"

#include <QDebug>

#include "util/rsingleton.h"
#include "action.h"
#include "actioncontainer.h"

template<class T,class P>
bool contains(QMap<T,P*> & map,const T &id)
{
    QMapIterator<T,P *> iterator(map);
    while(iterator.hasNext())
    {
        iterator.next();
        if(iterator.key() == id)
        {
            return true;
        }
    }
    return false;
}

ActionManager * ActionManager::actionManager = NULL;

ActionManager::ActionManager(QObject *parent):
    QObject(parent)
{
    actionManager = this;

}

ActionManager *ActionManager::instance()
{
    if(actionManager == NULL)
    {
        actionManager = new ActionManager();
    }
    return actionManager;
}

ActionManager::~ActionManager()
{

}

Action *ActionManager::registAction(Id id, QAction *newAction)
{
    if(contains<Id,Action>(actions,id))
        return actions.value(id);

    Action * action = new Action(id);
    action->setAction(newAction);
    action->setObjectName(id.data());
    actions.insert(id,action);

    connect(action, SIGNAL(destroyed()), this, SLOT(itemDestroyed()));

    return action;
}

Action *ActionManager::action(Id id)
{
    if(contains<Id,Action>(actions,id))
        return actions.value(id);

    return NULL;
}

ActionContainer *ActionManager::createMenu(Id id)
{
    if(contains<Id,ActionContainer>(menus,id))
        return menus.value(id);

    QMenu * menu = new QMenu();
    menu->setObjectName(id.data());

    MenuActionContainer * container = new MenuActionContainer(id);
    container->setMenu(menu);

    menus.insert(id,container);

    return container;
}

ActionContainer *ActionManager::actionContainer(Id id)
{
    if(contains<Id,ActionContainer>(menus,id))
        return menus.value(id);

    return NULL;
}

void ActionManager::releaseMenu(Id id)
{
    if(contains<Id,ActionContainer>(menus,id)){
        ActionContainerProxy * menuProxy = dynamic_cast<ActionContainerProxy *>(menus.value(id));
        if(menuProxy){
             delete menuProxy;
        }
        menus.remove(id);
    }
}

void ActionManager::itemDestroyed()
{
    if(sender()){
        actions.remove(sender()->objectName().toLocal8Bit().data());
    }
}

ActionContainer *ActionManager::createMenuBar(Id id)
{
    if(contains<Id,ActionContainer>(menus,id))
        return menus.value(id);

    QMenuBar *mb = new QMenuBar;
    mb->setObjectName(id.toString());

    MenuBarActionContainer *mbc = new MenuBarActionContainer(id);
    mbc->setMenuBar(mb);

    menus.insert(id,mbc);

    return mbc;
}
