#include "connectionmonitormodel.h"

#include <QDebug>
#include "Util/scaleswitcher.h"

ConnectionMonitorModel::ConnectionMonitorModel(QObject *parent):
    QAbstractTableModel(parent)
{
    headList<<tr("Node id")<<tr("Remote ip")<<tr("Remote port")<<tr("Input")<<tr("Output")
           <<tr("Packets");
}

ConnectionMonitorModel::~ConnectionMonitorModel()
{

}

void ConnectionMonitorModel::updateData(ServerNetwork::TcpClientManager::ClientInfoList &list)
{
    dataList = std::move(list);
    refreshData();
}

int ConnectionMonitorModel::rowCount(const QModelIndex &parent) const
{
    return dataList.size();
}

int ConnectionMonitorModel::columnCount(const QModelIndex &parent) const
{
    return headList.size();
}

QVariant ConnectionMonitorModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(index.row() >= dataList.size())
        return QVariant();

    int row = index.row();
    int column = index.column();

    switch(role){
        case Qt::TextAlignmentRole:
                return Qt::AlignCenter;
            break;
        case Qt::DisplayRole:
                switch(static_cast<TableColumn>(column)){
                    case C_NODE:
                        return  ScaleSwitcher::fromDecToHex(dataList.at(row).accountId);
                    case C_IP:
                          return dataList.at(row).ip;
                    case C_PORT:
                          return dataList.at(row).cPort;
                    case C_INPUT:
                          return dataList.at(row).recvBytes;
                    case C_OUTPUT:
                          return dataList.at(row).sendBytes;
                    case C_PACKETS:
                          return dataList.at(row).recvPacks;
                    default:
                        break;
                }
            break;
        default:
            break;
    }

    return QVariant();
}

Qt::ItemFlags ConnectionMonitorModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags  flag = Qt::ItemIsSelectable;
    flag |=  Qt::ItemIsEnabled;
    return flag;
}

QVariant ConnectionMonitorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && section < headList.size()){
        switch(role){
            case Qt::TextAlignmentRole:
                    return Qt::AlignCenter;
                break;
            case Qt::DisplayRole:
                    return headList.at(section);
                break;
            default:
                break;
        }
    }
    return QVariant();
}

void ConnectionMonitorModel::refreshData()
{
    beginResetModel();
    endResetModel();
}
