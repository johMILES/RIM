﻿#include "paneltoparea.h"

#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

#include "datastruct.h"
#include "constants.h"
#include "global.h"
#include "head.h"
#include "Util/imagemanager.h"
#include "rsingleton.h"
#include "maindialog.h"
#include "widget/rlineedit.h"
#include "protocoldata.h"
#include "Network/msgwrap.h"
#include "messdiapatch.h"

#include "widget/rlabel.h"

#define PANEL_TOP_USER_ICON_SIZE 66
#define PANEL_TOP_SEARCH_ICON_SIZE 28

class PanelTopAreaPrivate : public GlobalData<PanelTopArea>
{
    Q_DECLARE_PUBLIC(PanelTopArea)

private:
    PanelTopAreaPrivate(PanelTopArea * q):q_ptr(q)
    {
        initWidget();
    }

    void initWidget();

    PanelTopArea * q_ptr;

    QWidget * contentWidget;

    RIconLabel * userIconLabel;
    QLabel * userNikcNameLabel;
    RLineEdit * userSignNameEdit;
    QWidget * extendToolWiget;
    OnLineState * onlineState;

    QLineEdit * searchLineEdit;
};

void PanelTopAreaPrivate::initWidget()
{
    contentWidget = new QWidget(q_ptr);
    contentWidget->setObjectName("Panel_Top_ContentWidget");

    QHBoxLayout * mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(1);
    mainLayout->addWidget(contentWidget);
    q_ptr->setLayout(mainLayout);

    QGridLayout * gridLayout = new QGridLayout(contentWidget);

    QWidget * iconWidget = new QWidget(contentWidget);
    iconWidget->setFixedSize(PANEL_TOP_USER_ICON_SIZE + 15,PANEL_TOP_USER_ICON_SIZE);
    QHBoxLayout * iconLayout = new QHBoxLayout;
    iconLayout->setContentsMargins(0,0,0,0);

    userIconLabel = new RIconLabel(contentWidget);
    userIconLabel->setTransparency(true);
    userIconLabel->setEnterHighlight(true);
    userIconLabel->setEnterHighlightColor(QColor(255,255,255,150));
    userIconLabel->setObjectName("Panel_Top_UserIconLabel");
    userIconLabel->setToolTip(QObject::tr("Edit personal information"));
    userIconLabel->setFixedSize(PANEL_TOP_USER_ICON_SIZE,PANEL_TOP_USER_ICON_SIZE);
    userIconLabel->setPixmap(RSingleton<ImageManager>::instance()->getSystemUserIcon());
    QObject::connect(userIconLabel,SIGNAL(mousePressed()),MainDialog::instance(),SLOT(showPersonalEditWindow()));
//    connect(userIconLabel,SIGNAL(mouseHover(bool)),MainDialog::instance(),SLOT(showUserInfo(bool)));

    iconLayout->addStretch(1);
    iconLayout->addWidget(userIconLabel);
    iconWidget->setLayout(iconLayout);

    QWidget * nameStatusWidget = new QWidget(contentWidget);
    nameStatusWidget->setFixedHeight(PANEL_TOP_USER_ICON_SIZE / 3);
    QHBoxLayout * nameStatusLayout = new QHBoxLayout;
    nameStatusLayout->setContentsMargins(0,0,0,0);
    nameStatusLayout->setSpacing(1);

    userNikcNameLabel = new QLabel(contentWidget);
    userNikcNameLabel->setObjectName("Panel_Top_UserNikcNameLabel");
    userNikcNameLabel->setFixedHeight(PANEL_TOP_USER_ICON_SIZE / 3);
    userNikcNameLabel->setText(G_UserBaseInfo.nickName);

    onlineState = new OnLineState(contentWidget);
    onlineState->setStyleSheet("background-color:rgba(0,0,0,0)");
    QObject::connect(onlineState,SIGNAL(stateChanged(OnlineStatus)),q_ptr,SLOT(stateChanged(OnlineStatus)));

    nameStatusLayout->addWidget(userNikcNameLabel);
    nameStatusLayout->addWidget(onlineState);
    nameStatusLayout->addStretch(1);
    nameStatusWidget->setLayout(nameStatusLayout);

    userSignNameEdit = new RLineEdit(contentWidget);
    userSignNameEdit->setObjectName("Panel_Top_UserSignNameEdit");
    userSignNameEdit->setFixedHeight(PANEL_TOP_USER_ICON_SIZE / 3);
    userSignNameEdit->setText(G_UserBaseInfo.signName);
    QObject::connect(userSignNameEdit,SIGNAL(contentChanged(QString)),q_ptr,SLOT(respSignChanged(QString)));

    extendToolWiget = new QWidget(contentWidget);
    extendToolWiget->setFixedHeight(PANEL_TOP_USER_ICON_SIZE / 3);

    QWidget * searchWidget = new QWidget(contentWidget);
    searchWidget->setObjectName("Panel_Top_SearchWidget");
    QHBoxLayout * searchLayout = new QHBoxLayout();
    searchLayout->setContentsMargins(5,0,0,0);
    searchLayout->setSpacing(1);

    QLabel * iconLabel = new QLabel(searchWidget);
    iconLabel->setPixmap(QPixmap(Constant::ICON_PANEL_SEARCH));
    iconLabel->setFixedSize(PANEL_TOP_SEARCH_ICON_SIZE,PANEL_TOP_SEARCH_ICON_SIZE);
    iconLabel->setToolTip(QObject::tr("Search person/group"));

    searchLineEdit = new QLineEdit(contentWidget);
    searchLineEdit->setFixedHeight(PANEL_TOP_SEARCH_ICON_SIZE);
    searchLineEdit->setObjectName("Panel_Top_SearchLineEdit");
    searchLineEdit->setPlaceholderText(QObject::tr("Search"));

    searchLayout->addWidget(iconLabel);
    searchLayout->addWidget(searchLineEdit);
    searchWidget->setLayout(searchLayout);

    gridLayout->setContentsMargins(0,0,0,0);
    gridLayout->setRowStretch(0,4);
    gridLayout->setVerticalSpacing(5);
    gridLayout->setHorizontalSpacing(5);

    gridLayout->addWidget(iconWidget,1,0,3,1);
    gridLayout->addWidget(nameStatusWidget,1,1,1,1);
    gridLayout->addWidget(userSignNameEdit,2,1,1,1);
    gridLayout->addWidget(extendToolWiget,3,1,1,1);
    gridLayout->setRowStretch(4,1);
    gridLayout->addWidget(searchWidget,5,0,1,2);

    contentWidget->setLayout(gridLayout);
}

PanelTopArea::PanelTopArea(QWidget *parent) :
    d_ptr(new PanelTopAreaPrivate(this)),
    QWidget(parent)
{
    RSingleton<Subject>::instance()->attach(this);

    connect(MessDiapatch::instance(),SIGNAL(recvUpdateBaseInfoResponse(ResponseUpdateUser,UpdateBaseInfoResponse)),
            this,SLOT(recvBaseInfoResponse(ResponseUpdateUser,UpdateBaseInfoResponse)));
}

PanelTopArea::~PanelTopArea()
{
    RSingleton<Subject>::instance()->detach(this);
    delete d_ptr;
}

void PanelTopArea::onMessage(MessageType type)
{
    switch(type)
    {
        case MESS_BASEINFO_UPDATE:
                             updateUserInfo();
                             break;
        default:
            break;
    }
}

/*!
     * @brief 设置面板中用户的状态显示
     * @param[in] state:OnlineStatus，用户状态枚举值
     * @return 无
     */
void PanelTopArea::setState(OnlineStatus state)
{
    MQ_D(PanelTopArea);
    d->onlineState->setState(state);
}

void PanelTopArea::respSignChanged(QString content)
{
    UpdateBaseInfoRequest * request = new UpdateBaseInfoRequest;

    request->baseInfo.accountId = G_UserBaseInfo.accountId;
    request->baseInfo.nickName = G_UserBaseInfo.nickName;
    request->baseInfo.signName = content;
    request->baseInfo.sexual = G_UserBaseInfo.sexual;
    request->baseInfo.birthday = G_UserBaseInfo.birthday;
    request->baseInfo.address = G_UserBaseInfo.address;
    request->baseInfo.email = G_UserBaseInfo.email;
    request->baseInfo.phoneNumber = G_UserBaseInfo.phoneNumber;
    request->baseInfo.remark = G_UserBaseInfo.remark;
    request->baseInfo.face = G_UserBaseInfo.face;
    request->baseInfo.customImgId = G_UserBaseInfo.customImgId;

    RSingleton<MsgWrap>::instance()->handleMsg(request);
}

void PanelTopArea::recvBaseInfoResponse(ResponseUpdateUser result, UpdateBaseInfoResponse response)
{
    if(result == UPDATE_USER_SUCCESS)
    {
        G_UserBaseInfo = response.baseInfo;

        RSingleton<Subject>::instance()->notify(MESS_BASEINFO_UPDATE);
    }
}

void PanelTopArea::updateUserInfo()
{
    MQ_D(PanelTopArea);
    d->userSignNameEdit->setText(G_UserBaseInfo.signName);
}

void PanelTopArea::stateChanged(OnlineStatus state)
{
    Q_UNUSED(state);
    //TODO LYS-通知服务器当前用户状态更新
    //另外对于不同状态做出相应的处理
}
