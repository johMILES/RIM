/*!
 *  @brief     日志监视面板
 *  @details   记录实时产生的日志信息
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.24
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef OPERETEPANEL_H
#define OPERETEPANEL_H

#include "Base/rcomponent.h"
#include "Base/messagetype.h"

class OperetePanel : public RComponent
{
    Q_OBJECT
public:
    explicit OperetePanel(QWidget * parent = 0);
    ~OperetePanel();

    bool initialize();
    void release();

    void onMessage(MessageType::MessageType type);

    QSize sizeHint()const;

};

#endif // OPERETEPANEL_H
