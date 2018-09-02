/*!
 *  @brief     命令类
 *  @details   实现对QAction的扩展
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.23
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef ACTION_H
#define ACTION_H

#include "command.h"
#include "../base_global.h"

class QAction;

class BASESHARED_EXPORT Action : public CommandPrivate
{
    Q_OBJECT
public:
    explicit Action(Id id,QObject * parent = 0);
    ~Action();

    void setAction(QAction * act);
    QAction *action() const ;
    QShortcut *shortcut() const;

private:
    QAction * m_action;

};

#endif // ACTION_H
