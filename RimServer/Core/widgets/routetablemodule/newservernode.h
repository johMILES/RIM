/*!
 *  @brief     新建服务器节点页面
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.31
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef NEWSERVERNODE_H
#define NEWSERVERNODE_H

#include "Base/selfwidget/dialogproxy.h"

namespace ParameterSettings{
    struct NodeServer;
}

class NewServerNodePrivate;

class NewServerNode : public DialogProxy
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(NewServerNode)
public:
    explicit NewServerNode(QWidget * parent = 0);
    ~NewServerNode();

    QSize sizeHint()const;

signals:
    void newServerNodeInfo(ParameterSettings::NodeServer *);

protected:
    void respButtClicked(StandardButton butt);

private:
    bool validateInput();

private:
    NewServerNodePrivate * d_ptr;

};

#endif // NEWSERVERNODE_H
