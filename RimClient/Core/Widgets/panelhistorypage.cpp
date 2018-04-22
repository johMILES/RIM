﻿#include "panelhistorypage.h"

#include <QHBoxLayout>
#include <QMenu>
#include <QFileInfo>
#include <QDir>
#include <QTimer>
#include <QDateTime>
#include <map>
#include <functional>
#include <ctime>

#include "head.h"
#include "constants.h"
#include "toolbox/listbox.h"
#include "sql/sqlprocess.h"
#include "rsingleton.h"
#include "global.h"
#include "Util/imagemanager.h"
#include "user/user.h"

#include "actionmanager/actionmanager.h"

class PanelHistoryPagePrivate : public GlobalData<PanelHistoryPage>
{
    Q_DECLARE_PUBLIC(PanelHistoryPage)

public:
    PanelHistoryPagePrivate(PanelHistoryPage * q):
        q_ptr(q)
    {
        initWidget();
    }
    void initWidget();

    PanelHistoryPage * q_ptr;
    ListBox * listBox;
    QWidget * contentWidget;

    std::map<ToolItem *,HistoryChatRecord> itemsMap;
    std::list<ToolItem *> topList;
};

void PanelHistoryPagePrivate::initWidget()
{
    contentWidget = new QWidget(q_ptr);
    contentWidget->setObjectName("Panel_Center_ContentWidget");

    QHBoxLayout * mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(contentWidget);
    q_ptr->setLayout(mainLayout);

    listBox = new ListBox(contentWidget);
    QHBoxLayout * contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(0,0,0,0);
    contentLayout->setSpacing(0);
    contentLayout->addWidget(listBox);
    contentWidget->setLayout(contentLayout);
}

PanelHistoryPage::PanelHistoryPage(QWidget *parent):
    d_ptr(new PanelHistoryPagePrivate(this)),
    QWidget(parent)
{
    createAction();

    d_ptr->listBox->setContextMenu(ActionManager::instance()->menu(Constant::MENU_PANEL_HISTORY_TOOLBOX));

    QTimer::singleShot(0,this,SLOT(loadHistoryList()));
}

PanelHistoryPage::~PanelHistoryPage()
{

}

void PanelHistoryPage::viewDetail()
{

}

void PanelHistoryPage::deleteContact()
{

}

/*!
 * @brief 设置item置顶显示
 * @details 将item从list列表中暂时移除，根据topList的尺寸，重新插入到listbox中对应的位置
 */
void PanelHistoryPage::setTopSession()
{
    MQ_D(PanelHistoryPage);
    ToolItem * selectedItem = d->listBox->selectedItem();
    if(selectedItem){
        HistoryChatRecord record = d->itemsMap.at(selectedItem);
        if(RSingleton<SQLProcess>::instance()->topHistoryRecord(G_User->database(),record.accountId,!selectedItem->isTop())){
            if(selectedItem->isTop()){

                //此时items中包含了selectedItems，因此长度需要减1
                QList<ToolItem *> items = d->listBox->sortedItems();

                int index = -1;
                for(int i = d->topList.size();i < items.size();i++){
                    if(d->itemsMap.at(items.at(i)).dtime < record.dtime){
                        index = i - 1;
                        break;
                    }
                }

                if(index == -1)
                    index = items.size() - 1;

                if(d->listBox->reInsert(selectedItem,index)){
                    selectedItem->setTop(false);
                    d->topList.remove(selectedItem);
                }
            }else{
                if(d->listBox->reInsert(selectedItem,d->topList.size())){
                    d->topList.push_back(selectedItem);
                    selectedItem->setTop(true);
                }
            }
        }
    }
}

/*!
 * @brief 清空聊天列表
 * @details 1.删除所有的item；2.清空itemsMap容器
 */
void PanelHistoryPage::clearList()
{
    MQ_D(PanelHistoryPage);

    if(RSingleton<SQLProcess>::instance()->removeAllHistoryRecord(G_User->database())){
        d->listBox->removeAllItem();
        d->itemsMap.clear();
        d->topList.clear();
    }
}

/*!
 * @brief 通过历史记录发送即时信息
 * @details 若发送信息后，即时调整此信息在列表中的位置。
 */
void PanelHistoryPage::sendInstantMessage()
{

}

/*!
 * @brief 将历史会话记录从会话列表中移除,同时从数据表HistoryChat表中移除对应记录
 */
void PanelHistoryPage::removeSessionFromList()
{
    MQ_D(PanelHistoryPage);

    HistoryChatRecord record =  d->itemsMap.at(d->listBox->selectedItem());

    if(RSingleton<SQLProcess>::instance()->removeOneHistoryRecord(G_User->database(),record.accountId)){
        auto size = d->itemsMap.erase(d->listBox->selectedItem());
        if(size == 1){
            d->topList.remove(d->listBox->selectedItem());
            d->listBox->removeItem(d->listBox->selectedItem());
        }
    }
}

/*!
 * @brief 加载历史会话列表
 */
void PanelHistoryPage::loadHistoryList()
{
    MQ_D(PanelHistoryPage);
    QList<HistoryChatRecord> recordList;
    if(RSingleton<SQLProcess>::instance()->loadChatHistoryChat(G_User->database(),recordList)){
        //TODO 20180422直接将谓语改成传递类成员函数
        std::for_each(recordList.begin(),recordList.end(),[&](HistoryChatRecord  record){
            createHistoryItem(record);
        });
//        std::for_each(recordList.begin(),recordList.end(),std::bind1st(std::mem_fun(&PanelHistoryPage::createHistoryItem), this));
    }
}

/*!
 * @brief 添加历史历史对话记录
 * @param[in] record 待添加记录的描述信息
 */
void PanelHistoryPage::addHistoryItem(HistoryChatRecord record)
{
    MQ_D(PanelHistoryPage);

    createHistoryItem(record);
}

void PanelHistoryPage::createHistoryItem(HistoryChatRecord &record)
{
    MQ_D(PanelHistoryPage);

    ToolItem * item = new ToolItem(NULL);
    item->setContentMenu(ActionManager::instance()->menu(Constant::MENU_PANEL_HISTORY_TOOLITEM));
    item->setName(record.nickName);
    item->setNickName(record.accountId);
    item->setDescInfo("");
    item->setTop(record.isTop);

    QDateTime ctime = QDateTime::currentDateTime();
    QDateTime dtime = QDateTime::fromMSecsSinceEpoch(record.dtime);
    QDateTime yesterdayStart = ctime.addDays(-1);
    yesterdayStart.setTime(QTime(0,0,0));

    QDateTime yesterdatEnd = ctime.addDays(-1);
    yesterdatEnd.setTime(QTime(23,59,59));

    //当天
    if(ctime.date().day() - dtime.date().day()== 0){
        item->setDate(dtime.toString("hh:mm"));
    }else if(dtime >= yesterdayStart && dtime <= yesterdatEnd){
        item->setDate(QStringLiteral("昨天"));
    }else{
        item->setDate(dtime.toString("MM-dd"));
    }

    if(record.type == CHAT_C2C){
        item->setIcon(G_User->getIcon(record.systemIon,record.iconId));
    }else{

    }

    d->itemsMap.insert({item,record});

    d->listBox->addItem(item);
    if(record.isTop){
        if(d->listBox->reInsert(item,d->topList.size()))
            d->topList.push_back(item);
    }
}

void PanelHistoryPage::createAction()
{
    //创建面板的右键菜单
    QMenu * menu = ActionManager::instance()->createMenu(Constant::MENU_PANEL_HISTORY_TOOLBOX);

    QAction * clearListAction = ActionManager::instance()->createAction(Constant::ACTION_PANEL_HISTORY_CLEARLIST,this,SLOT(clearList()));
    clearListAction->setText(tr("Clear list"));

    menu->addAction(clearListAction);

    //创建联系人的右键菜单
    QMenu * historyMenu = ActionManager::instance()->createMenu(Constant::MENU_PANEL_HISTORY_TOOLITEM);

    QAction * sendMessAction = ActionManager::instance()->createAction(Constant::ACTION_PANEL_HISTORY_SENDMESSAGE,this,SLOT(sendInstantMessage()));
    sendMessAction->setText(tr("Send Instant Message"));

    QAction * viewDetailAction = ActionManager::instance()->createAction(Constant::ACTION_PANEL_HISTORY_VIEWDETAIL,this,SLOT(viewDetail()));
    viewDetailAction->setText(tr("View Detail"));

    QAction * deleteAction = ActionManager::instance()->createAction(Constant::ACTION_PANEL_HISTORY_DELPERSON,this,SLOT(deleteContact()));
    deleteAction->setText(tr("Delete Friend"));

    QAction * stickSessionAction = ActionManager::instance()->createAction(Constant::ACTION_PANEL_HISTORY_STICKSESSION,this,SLOT(setTopSession()));
    stickSessionAction->setText(tr("Top session"));

    QAction * removeSessionAction = ActionManager::instance()->createAction(Constant::ACTION_PANEL_HISTORY_REMOVEFORMLIST,this,SLOT(removeSessionFromList()));
    removeSessionAction->setText(tr("Remove session"));

    historyMenu->addAction(sendMessAction);
    historyMenu->addSeparator();
    historyMenu->addAction(viewDetailAction);
    historyMenu->addAction(deleteAction);
    historyMenu->addSeparator();
    historyMenu->addSeparator();
    historyMenu->addAction(stickSessionAction);
    historyMenu->addAction(removeSessionAction);
}
