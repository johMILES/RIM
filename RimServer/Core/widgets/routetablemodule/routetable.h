/*!
 *  @brief     路由表查看与设置界面
 *  @details   通过此页面可编辑服务器、客户端基本信息，支持添加、删除、修改、保存，但目前保存后的数据，需要重新启动程序方可生效 \n
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.17
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef ROUTETABLE_H
#define ROUTETABLE_H

#include <QModelIndex>
#include <QSharedPointer>
#include "Base/selfwidget/dialogproxy.h"

namespace ParameterSettings{
struct NodeServer;
struct NodeClient;
}

class RouteTablePrivate;

class RouteTable : public DialogProxy
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(RouteTable)
public:
    explicit RouteTable(QWidget *parent = 0);
    ~RouteTable();

    QSize sizeHint()const;

protected:
    void respButtClicked(StandardButton butt);

private:
    void initContextMenu();

    void saveFile();
    void cancelSave();

private slots:
    void switchServerIndex(QModelIndex index);

    void addNewServerGroup();
    void deleteServerGroup();

    void addNewClientGroup();
    void moveClientGroup();
    void deleteClientGroup();

    void createNewServerNode(ParameterSettings::NodeServer * server);
    void createNewClientNode(ParameterSettings::NodeClient *client);

private:
    QSharedPointer<RouteTablePrivate> d_ptr;
};

#endif // ROUTETABLE_H
