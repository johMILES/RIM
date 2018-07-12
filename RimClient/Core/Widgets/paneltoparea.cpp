﻿#include "paneltoparea.h"

#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDir>

#include "../protocol/datastruct.h"
#include "constants.h"
#include "global.h"
#include "head.h"
#include "Util/imagemanager.h"
#include "util/rsingleton.h"
#include "maindialog.h"
#include "widget/rlineedit.h"
#include "../protocol/protocoldata.h"
#include "Network/msgwrap/wrapfactory.h"
#include "messdiapatch.h"
#include "user/user.h"
#include "Network/netconnector.h"
#include "thread/taskmanager.h"
#include "Util/rutil.h"

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

    RIconLabel * userIconLabel;                     /*!< 用户头像 */
    QLabel * userNikcNameLabel;                     /*!< 用户昵称 */
    RLineEdit * userSignNameEdit;                   /*!< 用户签名 */
    QWidget * extendToolWiget;                      /*!< 扩展工具 */
    OnLineState * onlineState;                      /*!< 在线状态 */

    QLineEdit * searchLineEdit;

    QString originUserSignName;                     /*!< 原始用户签名 */
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
    userNikcNameLabel->setText(G_User->BaseInfo().nickName);

    onlineState = new OnLineState(contentWidget);
    onlineState->setState(G_OnlineStatus);
    onlineState->setStyleSheet("background-color:rgba(0,0,0,0)");
#ifndef __LOCAL_CONTACT__
    QObject::connect(onlineState,SIGNAL(stateChanged(OnlineStatus)),q_ptr,SLOT(stateChanged(OnlineStatus)));
#endif

    nameStatusLayout->addWidget(userNikcNameLabel);
    nameStatusLayout->addWidget(onlineState);
    nameStatusLayout->addStretch(1);
    nameStatusWidget->setLayout(nameStatusLayout);

    userSignNameEdit = new RLineEdit(contentWidget);
    userSignNameEdit->setObjectName("Panel_Top_UserSignNameEdit");
    userSignNameEdit->setFixedHeight(PANEL_TOP_USER_ICON_SIZE / 3);
    originUserSignName = G_User->BaseInfo().signName;
#ifndef __LOCAL_CONTACT__
    QObject::connect(userSignNameEdit,SIGNAL(contentChanged(QString)),q_ptr,SLOT(respSignChanged(QString)));
#endif

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
    userSignNameEdit->installEventFilter(q_ptr);
}

PanelTopArea::PanelTopArea(QWidget *parent) :
    d_ptr(new PanelTopAreaPrivate(this)),
    QWidget(parent)
{
    RSingleton<Subject>::instance()->attach(this);

    loadCustomUserImage();
#ifndef __LOCAL_CONTACT__
    connect(MessDiapatch::instance(),SIGNAL(recvUpdateBaseInfoResponse(ResponseUpdateUser,UpdateBaseInfoResponse)),
            this,SLOT(recvBaseInfoResponse(ResponseUpdateUser,UpdateBaseInfoResponse)));

    connect(MessDiapatch::instance(),SIGNAL(recvUserStateChangedResponse(MsgOperateResponse,UserStateResponse)),
            this,SLOT(recvUserStateChanged(MsgOperateResponse,UserStateResponse)));
#endif
}

PanelTopArea::~PanelTopArea()
{
    RSingleton<Subject>::instance()->detach(this);
    delete d_ptr;
}

void PanelTopArea::onMessage(MessageType type)
{
    MQ_D(PanelTopArea);
    switch(type)
    {
        case MESS_BASEINFO_UPDATE:
             updateUserInfo();
             break;
        case MESS_ICON_CHANGE:
             loadCustomUserImage();
             break;
        case MESS_TEXT_NET_ERROR:
             networkIsConnected(false);
             break;
        case MESS_TEXT_NET_OK:
             networkIsConnected(true);
             break;
        case MESS_USER_OFF_LINE:
             {
                d->userIconLabel->setGray(true);
                d->onlineState->setState(STATUS_OFFLINE);
             }
             break;
        default:
             break;
    }
}

void PanelTopArea::loadCustomUserImage()
{
    MQ_D(PanelTopArea);
    d->userIconLabel->setPixmap(G_User->getIcon());
}

void PanelTopArea::networkIsConnected(bool connected)
{
    MQ_D(PanelTopArea);
    d->userIconLabel->setGray(!connected);
    if(!connected){
        d->onlineState->setState(STATUS_OFFLINE);
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

#ifndef __LOCAL_CONTACT__
void PanelTopArea::respSignChanged(QString content)
{
    R_CHECK_ONLINE;
    R_CHECK_LOGIN;
    UpdateBaseInfoRequest * request = new UpdateBaseInfoRequest;

    request->baseInfo.accountId = G_User->BaseInfo().accountId;
    request->baseInfo.nickName = G_User->BaseInfo().nickName;
    request->baseInfo.signName = content;
    request->baseInfo.sexual = G_User->BaseInfo().sexual;
    request->baseInfo.birthday = G_User->BaseInfo().birthday;
    request->baseInfo.address = G_User->BaseInfo().address;
    request->baseInfo.email = G_User->BaseInfo().email;
    request->baseInfo.phoneNumber = G_User->BaseInfo().phoneNumber;
    request->baseInfo.remark = G_User->BaseInfo().remark;
    request->baseInfo.isSystemIcon = G_User->BaseInfo().isSystemIcon;
    request->baseInfo.iconId = G_User->BaseInfo().iconId;
    request->requestType = UPDATE_USER_DETAIL;

    RSingleton<WrapFactory>::instance()->getMsgWrap()->handleMsg(request);
}

void PanelTopArea::recvBaseInfoResponse(ResponseUpdateUser result, UpdateBaseInfoResponse response)
{
    if(result == UPDATE_USER_SUCCESS)
    {
        G_User->BaseInfo() = response.baseInfo;
        RSingleton<Subject>::instance()->notify(MESS_BASEINFO_UPDATE);
    }
}

void PanelTopArea::recvUserStateChanged(MsgOperateResponse result,UserStateResponse response)
{
    Q_UNUSED(result);
    Q_UNUSED(response);
}

/*!
 * @brief 改变用户状态
 * @details 用户状态改变只允许在登陆状态下改变；
 * @param[in] state 当前用户状态
 */
void PanelTopArea::stateChanged(OnlineStatus state)
{
    if(G_User->isTextOnLine()){
        if(!G_User->isLogin()){
            LoginRequest * request = new LoginRequest();
            request->accountId = G_User->getUserInfoDesc().accountId;
            request->password = G_User->getUserInfoDesc().password;
            request->status = state;
            G_OnlineStatus = state;
            request->loginType = RECONNECT_LOGIN;
            RSingleton<WrapFactory>::instance()->getMsgWrap()->handleMsg(request);
        }
        else
        {
            UserStateRequest * request = new UserStateRequest();
            request->accountId = G_User->BaseInfo().accountId;
            request->onStatus = state;
            RSingleton<WrapFactory>::instance()->getMsgWrap()->handleMsg(request);
        }
    }else{
        RSingleton<TaskManager>::instance()->initTask();
        if(TextNetConnector::instance())
            TextNetConnector::instance()->reconnect();
    }
}

#endif

void PanelTopArea::updateUserInfo()
{
    MQ_D(PanelTopArea);
    d->originUserSignName = G_User->BaseInfo().signName;
    d->userSignNameEdit->setText(RUtil::replaceLongTextWidthElide(d->userSignNameEdit->font(),d->originUserSignName,d->userSignNameEdit->width()));
    d->userNikcNameLabel->setText(G_User->BaseInfo().nickName);

    QString t_iconPath;
    if(G_User->BaseInfo().isSystemIcon){
        t_iconPath = RSingleton<ImageManager>::instance()->getSystemUserIcon(G_User->BaseInfo().iconId);
    }

    if(!t_iconPath.isEmpty()){
        QFileInfo t_iconFile(t_iconPath);
        if(!t_iconFile.exists())
            return;
    }

    d->userIconLabel->setPixmap(t_iconPath);
}

bool PanelTopArea::eventFilter(QObject *watched, QEvent *event)
{
    MQ_D(PanelTopArea);
    if(watched == d->userSignNameEdit){
        if(event->type() == QEvent::Resize){
            d->userSignNameEdit->setText(RUtil::replaceLongTextWidthElide(d->userSignNameEdit->font(),d->originUserSignName,d->userSignNameEdit->width()));
            return true;
        }
    }
    return QWidget::eventFilter(watched,event);
}
