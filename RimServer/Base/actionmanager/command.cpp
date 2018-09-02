#include "command.h"

Command::Command(QObject *parent):QObject(parent)
{

}

Command::~Command()
{

}

CommandPrivate::CommandPrivate(Id id, QObject *parent):m_id(id),Command(parent)
{

}

CommandPrivate::~CommandPrivate()
{

}

Id CommandPrivate::id() const
{
    return m_id;
}

QAction *CommandPrivate::action() const
{
    return 0;
}

QShortcut *CommandPrivate::shortcut() const
{
    return 0;
}
