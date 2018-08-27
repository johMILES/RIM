#include "action.h"

#include <QAction>

Action::Action(Id id, QObject *parent):
    CommandPrivate(id,parent),m_action(nullptr)
{

}

Action::~Action()
{

}

void Action::setAction(QAction *act)
{
    m_action = act;
}

QAction *Action::action() const
{
    return m_action;
}

QShortcut *Action::shortcut() const
{
    return 0;
}
