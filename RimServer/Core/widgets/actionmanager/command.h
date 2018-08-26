/*!
 *  @brief     命令
 *  @details   定义了通用命令的接口
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.23
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>
#include "id.h"

class QAction;
class QShortcut;

class Command : public QObject
{
    Q_OBJECT
public:
    explicit Command(QObject * parent = 0);
    virtual ~Command();

    virtual Id id() const = 0;

    virtual QAction *action() const = 0;
    virtual QShortcut *shortcut() const = 0;

};

class CommandPrivate : public Command
{
    Q_OBJECT
public:
    explicit CommandPrivate(Id id,QObject * parent = 0);
    ~CommandPrivate();

    virtual Id id()const;

    virtual QAction *action() const ;
    virtual QShortcut *shortcut() const;

private:
    Id m_id;

};

#endif // COMMAND_H
