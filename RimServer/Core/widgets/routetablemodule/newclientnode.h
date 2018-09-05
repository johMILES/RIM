/*!
 *  @brief     新建客户端节点
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.02
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef NEWCLIENTNODE_H
#define NEWCLIENTNODE_H

#include "Base/selfwidget/dialogproxy.h"

class NewClientNodePrivate;

namespace ParameterSettings{
    struct NodeClient;
}

class NewClientNode : public DialogProxy
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(NewClientNode)
public:
    explicit NewClientNode(QWidget * parent = 0);
    ~NewClientNode();

    QSize sizeHint()const;

signals:
    void newClientNodeInfo(ParameterSettings::NodeClient *);

private:
    bool validateInput();

protected:
    void respButtClicked(StandardButton butt);

private:
    NewClientNodePrivate * d_ptr;
};

#endif // NEWCLIENTNODE_H
