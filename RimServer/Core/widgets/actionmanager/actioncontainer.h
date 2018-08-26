/*!
 *  @brief     Action容器
 *  @details   内部维护添加的Action
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.23
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef ACTIONCONTAINER_H
#define ACTIONCONTAINER_H

#include <QObject>

#include "id.h"

class QMenu;
class QMenuBar;
class QAction;
class Command;

struct Group
{
    Group(const Id &id) : id(id) {}
    Id id;
    QList<QObject *> items; // Command * or ActionContainer *
};

class ActionContainer : public QObject
{
    Q_OBJECT
public:
    ActionContainer(QObject * parent = 0);
    ~ActionContainer();

    virtual Id id() const = 0;

    virtual QMenu *menu() const = 0;
    virtual QMenuBar *menuBar() const = 0;
    virtual QAction *insertLocation(Id group) const = 0;
    virtual void appendGroup(Id group) = 0;
    virtual void insertGroup(Id before, Id group) = 0;
    virtual void addAction(Command *action, Id group = Id()) = 0;
    virtual void addMenu(ActionContainer *menu, Id group = Id()) = 0;
    virtual void addMenu(ActionContainer *before, ActionContainer *menu, Id group = Id()) = 0;

    virtual Command *addSeparator(Id groupId) = 0;
};

class ActionContainerProxy : public ActionContainer
{
    Q_OBJECT
public:
    explicit ActionContainerProxy(Id id,QObject * parent = 0);
    ~ActionContainerProxy();

    virtual Id id() const;
    virtual QMenu *menu() const;
    virtual QMenuBar *menuBar() const;
    virtual QAction *insertLocation(Id groupId) const;
    virtual void appendGroup(Id groupId);
    virtual void insertGroup(Id before, Id groupId);
    virtual void addAction(Command *command, Id groupId = Id());
    virtual void addMenu(ActionContainer *menu, Id groupId = Id());
    virtual void addMenu(ActionContainer *before, ActionContainer *menu, Id group = Id());

    virtual void insertAction(QAction *before, QAction *action) = 0;
    virtual void insertMenu(QAction *before, QMenu *menu) = 0;

    virtual void removeAction(QAction *action) = 0;
    virtual void removeMenu(QMenu *menu) = 0;

    Command * addSeparator(Id groupId);

protected:
    QList<Group> m_groups;

private:
    bool canAddAction(Command *action) const;
    QList<Group>::const_iterator findGroup(Id groupId) const;
    QAction *insertLocation(QList<Group>::const_iterator group) const;

private:
    Id m_id;
};

class MenuActionContainer : public ActionContainerProxy
{
public:
    explicit MenuActionContainer(Id id,QObject * parent = 0);

    void setMenu(QMenu * menu);
    QMenu *menu() const;

    void insertAction(QAction *before, QAction *action);
    void insertMenu(QAction *before, QMenu *menu);

    void removeAction(QAction *action);
    void removeMenu(QMenu *menu);

private:
    QMenu * m_menu;
};

class MenuBarActionContainer : public ActionContainerProxy
{
public:
    explicit MenuBarActionContainer(Id id);

    void setMenuBar(QMenuBar *menuBar);
    QMenuBar *menuBar() const;

    void insertAction(QAction *before, QAction *action);
    void insertMenu(QAction *before, QMenu *menu);

    void removeAction(QAction *action);
    void removeMenu(QMenu *menu);

private:
    QMenuBar *m_menuBar;
};


#endif // ACTIONCONTAINER_H
