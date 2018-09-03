﻿#include "routetableview.h"

#include <QHeaderView>
#include <QContextMenuEvent>
#include <QCursor>
#include <QDebug>

#include "Base/constants.h"
#include "Base/actionmanager/actioncontainer.h"
#include "Base/actionmanager/actionmanager.h"

RouteTableView::RouteTableView(ViewType type, QWidget *parent):
    QTableView(parent),vtype(type)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    horizontalHeader()->setStretchLastSection(true);

    setStyleSheet(QString(QLatin1String("QTreeView{ border:none; background:white; } \
                               QTreeView::item:selected{ color:white; background:%1; }")).arg(Constant::SELECTED_LINE_COLOR));

}

RouteTableView::~RouteTableView()
{

}

QModelIndex RouteTableView::currIndex()
{
    return indexAt(contextEventPos);
}

void RouteTableView::contextMenuEvent(QContextMenuEvent *event)
{
    contextEventPos = event->pos();
    if(vtype == V_SERVER){
        ActionManager::instance()->actionContainer(Constant::MENU_ROUTE_TABLE_SERVER)->menu()->exec(QCursor::pos());
    }else if(vtype == V_CLIENT){
        ActionManager::instance()->actionContainer(Constant::MENU_ROUTE_TABLE_CLIENT)->menu()->exec(QCursor::pos());
    }
}
