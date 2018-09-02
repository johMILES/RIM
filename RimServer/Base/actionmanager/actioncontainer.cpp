#include "actioncontainer.h"

#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QDebug>

#include "command.h"
#include "action.h"

const char DEFAULT_GROUP[]="Default.Group";

ActionContainer::ActionContainer(QObject *parent):
    QObject(parent)
{

}

ActionContainer::~ActionContainer()
{

}

ActionContainerProxy::ActionContainerProxy(Id id, QObject *parent):m_id(id),ActionContainer(parent)
{
}

ActionContainerProxy::~ActionContainerProxy()
{
    QList<Group>::iterator iter = m_groups.begin();
    while(iter != m_groups.end()){
        QList<QObject *>::iterator itemIter = iter->items.begin();
        while(itemIter != iter->items.end()){
            delete (*itemIter);
            itemIter++;
        }
        iter->items.clear();
        iter++;
    }
    m_groups.clear();
}

Id ActionContainerProxy::id() const
{
    return m_id;
}

QMenu *ActionContainerProxy::menu() const
{
    return 0;
}

QMenuBar *ActionContainerProxy::menuBar() const
{
    return 0;
}

QAction *ActionContainerProxy::insertLocation(Id groupId) const
{
    return 0;
}

void ActionContainerProxy::appendGroup(Id groupId)
{
     m_groups.append(Group(groupId));
}

void ActionContainerProxy::insertGroup(Id before, Id groupId)
{

}

void ActionContainerProxy::addAction(Command *command, Id groupId)
{
    const Id actualGroupId = groupId.isValid() ? groupId : Id(DEFAULT_GROUP);

    QList<Group>::const_iterator groupIt = findGroup(actualGroupId);
    if(groupIt == m_groups.constEnd())
        return;

    QAction *beforeAction = insertLocation(groupIt);
    m_groups[groupIt - m_groups.constBegin()].items.append(command);

    insertAction(0, command->action());
}

void ActionContainerProxy::addMenu(ActionContainer *menu, Id groupId)
{
    ActionContainer *container = static_cast<ActionContainer *>(menu);
    const Id actualGroupId = groupId.isValid() ? groupId : Id(DEFAULT_GROUP);
    QList<Group>::const_iterator groupIt = findGroup(actualGroupId);
    if(groupIt == m_groups.constEnd())
        return;
    QAction *beforeAction = insertLocation(groupIt);
    m_groups[groupIt - m_groups.constBegin()].items.append(menu);

    insertMenu(beforeAction, container->menu());
}

void ActionContainerProxy::addMenu(ActionContainer *before, ActionContainer *menu, Id group)
{

}

Command *ActionContainerProxy::addSeparator(Id groupId)
{
    Action * action = new Action(groupId);
    QAction * cat = new QAction();
    cat->setSeparator(true);
    action->setAction(cat);

    addAction(action,groupId);
    return action;
}

QList<Group>::const_iterator ActionContainerProxy::findGroup(Id groupId) const
{
    QList<Group>::const_iterator iter = m_groups.constBegin();
    while(iter != m_groups.constEnd()){
        if((*iter).id == groupId)
            return iter;
        iter++;
    }
    return iter;
}

QAction *ActionContainerProxy::insertLocation(QList<Group>::const_iterator group) const
{
    if (group == m_groups.constEnd())
        return 0;

    while (group != m_groups.constEnd()) {
        if (!group->items.isEmpty()) {
            QObject *item = group->items.last();
            if (Command *cmd = qobject_cast<Command *>(item)) {
                return cmd->action();
            } else if (ActionContainer *container = qobject_cast<ActionContainer *>(item)) {
                if (container->menu())
                    return container->menu()->menuAction();
            }
        }
        ++group;
    }
    return 0;
}

MenuActionContainer::MenuActionContainer(Id id, QObject *parent):m_menu(nullptr),ActionContainerProxy(id,parent)
{

}

void MenuActionContainer::setMenu(QMenu *menu)
{
    m_menu = menu;
}

QMenu *MenuActionContainer::menu() const
{
    return m_menu;
}

void MenuActionContainer::insertAction(QAction *before, QAction *action)
{
    m_menu->insertAction(before,action);
}

void MenuActionContainer::insertMenu(QAction *before, QMenu *menu)
{
    m_menu->insertMenu(before,menu);
}

void MenuActionContainer::removeAction(QAction *action)
{
    m_menu->removeAction(action);
}

void MenuActionContainer::removeMenu(QMenu *menu)
{
    m_menu->removeAction(menu->menuAction());
}

MenuBarActionContainer::MenuBarActionContainer(Id id):m_menuBar(nullptr),ActionContainerProxy(id)
{

}

void MenuBarActionContainer::setMenuBar(QMenuBar *menuBar)
{
    m_menuBar = menuBar;
}

QMenuBar *MenuBarActionContainer::menuBar() const
{
    return m_menuBar;
}

void MenuBarActionContainer::insertAction(QAction *before, QAction *action)
{
    m_menuBar->insertAction(before,action);
}

void MenuBarActionContainer::insertMenu(QAction *before, QMenu *menu)
{
    m_menuBar->insertMenu(before,menu);
}

void MenuBarActionContainer::removeAction(QAction *action)
{
    m_menuBar->removeAction(action);
}

void MenuBarActionContainer::removeMenu(QMenu *menu)
{
    m_menuBar->removeAction(menu->menuAction());
}
