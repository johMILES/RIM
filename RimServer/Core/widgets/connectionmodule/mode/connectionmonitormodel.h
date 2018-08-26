/*!
 *  @brief     连接信息数据模型
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.24
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef CONNECTIONMONITORMODEL_H
#define CONNECTIONMONITORMODEL_H

#include <QAbstractTableModel>

#include "Network/connection/tcpclient.h"

class ConnectionMonitorModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ConnectionMonitorModel(QObject *parent = nullptr);
    ~ConnectionMonitorModel();

    /*!
     *  @brief 客户端显示列
     */
    enum TableColumn{
        C_NODE,
        C_IP,
        C_PORT,
        C_INPUT,
        C_OUTPUT,
        C_PACKETS
    };

    void updateData(ServerNetwork::TcpClientManager::ClientInfoList & list);

protected:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    void refreshData();

private:
    QStringList headList;

    ServerNetwork::TcpClientManager::ClientInfoList dataList;
};

#endif // CONNECTIONMONITORMODEL_H
