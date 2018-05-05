﻿#include "logindialog.h"

#include <QList>
#include <QToolButton>
#include <QResizeEvent>
#include <QTimer>
#include <QDebug>
#include <QListView>
#include <QMenu>
#include <QAction>
#include <QDesktopWidget>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QApplication>
#include <QDir>
#include <QListWidget>

#include "head.h"
#include "datastruct.h"
#include "toolbar.h"
#include "constants.h"
#include "onlinestate.h"
#include "rsingleton.h"
#include "user/userinfofile.h"
#include "Util/rutil.h"
#include "Util/rlog.h"
#include "Util/imagemanager.h"
#include "maindialog.h"
#include "systemtrayicon.h"
#include "Widgets/actionmanager/actionmanager.h"
#include "thread/taskmanager.h"
#include "thread/filerecvtask.h"
#include "Network/msgwrap.h"
#include "Network/netconnector.h"
#include "widget/rmessagebox.h"
#include "others/msgqueuemanager.h"
#include "registdialog.h"
#include "netsettings.h"
#include "global.h"
#include "widget/rlabel.h"
#include "messdiapatch.h"
#include "media/mediaplayer.h"
#include "notifywindow.h"
#include "systemnotifyview.h"
#include "user/userclient.h"
#include "user/userfriendcontainer.h"
#include "user/userchatcontainer.h"
#include "abstractchatwidget.h"
#include "sql/sqlprocess.h"
#include "sql/databasemanager.h"
#include "widget/rcombobox.h"
#include "file/filemanager.h"
#include "json/jsonresolver.h"
#include "file/filedesc.h"
#include "user/user.h"
using namespace TextUnit ;

class LoginDialogPrivate : public QObject,public GlobalData<LoginDialog>
{
    Q_DECLARE_PUBLIC(LoginDialog)
public:
    LoginDialogPrivate(LoginDialog * q):q_ptr(q)
    {
        initWidget();

        numberExp = QRegExp(Constant::AccountId_Reg);
        passExp = QRegExp(Constant::AccountPassword_Reg);

        notifyWindow = NULL;

        isSystemUserIcon = true;
        isNewUser = true;
        registView = false;
    }
    ~LoginDialogPrivate(){}

    void initWidget();

private:
    LoginDialog * q_ptr;

    int userIndex(QString text);

    QList<UserInfoDesc *> localUserInfo;

    QPoint mousePreseePoint;

    bool isSystemUserIcon;
    QString defaultUserIconPath;                /*!< 默认用户头像名称 */

    bool isNewUser;
    bool registView;                            /*!< 是否为注册模式 */

    QWidget * contentWidget;
    QWidget * iconWidget;

    RIconLabel *userIcon;
    RTextLabel *forgetPassword;
    QPushButton *login;
    RTextLabel *regist;
    QLineEdit *password;
    QCheckBox *rememberPassord;
    QCheckBox *autoLogin;
    RComboBox *userList;
    QListWidget * userListWidget;

    QRegExp numberExp;
    QRegExp passExp;

    ToolBar * toolBar;
    OnLineState * onlineState;
    QToolButton * systemSetting;
    SystemTrayIcon * trayIcon;
    MainDialog * mainDialog;

    NotifyWindow * notifyWindow;
};

void LoginDialogPrivate::initWidget()
{
    contentWidget = new QWidget(q_ptr);
    contentWidget->setObjectName("AbstractWidget_ContentWidget");

    QVBoxLayout * contentLayout = new QVBoxLayout();
    contentLayout->setContentsMargins(0,0,0,0);
    contentLayout->setSpacing(0);

    QWidget * imageWidget = new QWidget(contentWidget);
    imageWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageWidget->setStyleSheet("border-image:url(':/background/resource/background/login.png')");

    /***************头像显示区****************/
    QWidget * bottomWidget = new QWidget(contentWidget);

    iconWidget = new QWidget(bottomWidget);

    QVBoxLayout * iconLayout = new QVBoxLayout(iconWidget);
    iconLayout->setContentsMargins(5,0,5,0);

    userIcon = new RIconLabel(iconWidget);
    userIcon->setTransparency(true);
    userIcon->setMinimumSize(QSize(80, 80));
    userIcon->setMaximumSize(QSize(80, 80));
    iconLayout->addWidget(userIcon);

    QSpacerItem * iconSpacer = new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Expanding);

    iconLayout->addItem(iconSpacer);

    onlineState = new OnLineState(iconWidget);
    onlineState->setStyleSheet("background-color:rgba(0,0,0,0)");

    iconWidget->setLayout(iconLayout);

    /*****************登陆输入区******************/
    QWidget * inputWidget = new QWidget();
    QGridLayout * inputGridLayout = new QGridLayout;

    inputGridLayout->setHorizontalSpacing(10);
    inputGridLayout->setVerticalSpacing(2);
    inputGridLayout->setContentsMargins(0, 0, 0, 0);

    QRegExpValidator * numberValidator = new QRegExpValidator(QRegExp(Constant::AccountId_Reg));

    userListWidget = new QListWidget();

    userList = new RComboBox();
    userList->setObjectName("LoginComboBox");
    userList->setFixedSize(QSize(193, 28));
    userList->setModel(userListWidget->model());
    userList->setView(userListWidget);
    userList->setEditable(true);
    userList->lineEdit()->setValidator(numberValidator);
    userList->lineEdit()->setPlaceholderText(QObject::tr("Input number"));

    QRegExpValidator * passValidator = new QRegExpValidator(QRegExp(Constant::AccountPassword_Reg));
    password = new QLineEdit();
    password->setFixedSize(QSize(193, 28));
    password->setObjectName("password");
    password->setValidator(passValidator);
    password->setPlaceholderText(QObject::tr("Input password"));
    password->setEchoMode(QLineEdit::Password);

    login = new QPushButton();
    login->setMinimumSize(QSize(0, 28));
    login->setText(QObject::tr("Sin in"));
    login->setObjectName("login");

    forgetPassword = new RTextLabel();
    forgetPassword->setText(QObject::tr("Forget Password"));

    regist = new RTextLabel();
    regist->setText(QObject::tr("Sin up"));
    QObject::connect(regist,SIGNAL(mousePress()),q_ptr,SLOT(showRegistDialog()));

    QWidget * widget_4 = new QWidget(inputWidget);
    widget_4->setObjectName(QStringLiteral("widget_4"));
    widget_4->setMaximumSize(QSize(193, 28));

    QHBoxLayout * horizontalLayout = new QHBoxLayout(widget_4);
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    horizontalLayout->setContentsMargins(1, 1, 1, 1);

    rememberPassord = new QCheckBox();
    rememberPassord->setObjectName(QStringLiteral("rememberPassord"));
    rememberPassord->setText(QObject::tr("Remember password"));

    autoLogin = new QCheckBox();
    autoLogin->setText(QObject::tr("Auto login"));

    QSpacerItem * horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addWidget(rememberPassord);
    horizontalLayout->addItem(horizontalSpacer);
    horizontalLayout->addWidget(autoLogin);

    inputGridLayout->addWidget(userList, 0, 1, 1, 1);
    inputGridLayout->addWidget(password, 1, 1, 1, 1);
    inputGridLayout->addWidget(widget_4, 2, 1, 1, 1);

    inputGridLayout->addWidget(forgetPassword, 1, 2, 1, 1);
    inputGridLayout->addWidget(regist, 0, 2, 1, 1);

    inputGridLayout->addWidget(login, 3, 1, 1, 1);

    inputWidget->setLayout(inputGridLayout);

    QHBoxLayout * bottomLayout = new QHBoxLayout();
    bottomLayout->addSpacerItem(new QSpacerItem(50,50,QSizePolicy::Expanding,QSizePolicy::Expanding));
    bottomLayout->addWidget(iconWidget);
    bottomLayout->addWidget(inputWidget);
    bottomLayout->addSpacerItem(new QSpacerItem(50,50,QSizePolicy::Expanding,QSizePolicy::Expanding));

    bottomWidget->setLayout(bottomLayout);

    QObject::connect(userList,SIGNAL(currentTextChanged(QString)),q_ptr,SLOT(validateUserName(QString)));
    QObject::connect(login,SIGNAL(pressed()),q_ptr,SLOT(login()));
    QObject::connect(rememberPassord,SIGNAL(toggled(bool)),q_ptr,SLOT(setPassword(bool)));
    QObject::connect(autoLogin,SIGNAL(clicked(bool)),rememberPassord,SLOT(setChecked(bool)));
    QObject::connect(password,SIGNAL(textEdited(QString)),q_ptr,SLOT(validatePassword(QString)));

    mainDialog = NULL;

    RToolButton * systemSetting = ActionManager::instance()->createToolButton(Id(Constant::TOOL_SETTING),q_ptr,SLOT(showNetSettings()));
    systemSetting->setToolTip(QObject::tr("System Setting"));

    RToolButton * minSize = ActionManager::instance()->createToolButton(Id(Constant::TOOL_MIN),q_ptr,SLOT(minsize()));
    minSize->setToolTip(QObject::tr("Min"));

    RToolButton * closeButt = ActionManager::instance()->createToolButton(Id(Constant::TOOL_CLOSE),q_ptr,SLOT(closeWindow()));
    closeButt->setToolTip(QObject::tr("Close"));

    toolBar = new ToolBar(contentWidget);
    toolBar->addStretch(1);
    toolBar->appendToolButton(systemSetting);
    toolBar->appendToolButton(minSize);
    toolBar->appendToolButton(closeButt);

    contentLayout->addWidget(imageWidget);
    contentLayout->addWidget(bottomWidget);

    contentLayout->setStretchFactor(imageWidget,9);
    contentLayout->setStretchFactor(bottomWidget,7);

    contentWidget->setLayout(contentLayout);

    q_ptr->setContentWidget(contentWidget);
    userList->lineEdit()->installEventFilter(q_ptr);
    password->installEventFilter(q_ptr);
}

int LoginDialogPrivate::userIndex(QString text)
{
    for(int i = 0; i < localUserInfo.size(); i++)
    {
        if(localUserInfo.at(i)->accountId == text)
        {
            return i;
        }
    }

    return -1;
}

LoginDialog::LoginDialog(QWidget *parent) :
    Widget(parent),
    d_ptr(new LoginDialogPrivate(this))
{
    RSingleton<Subject>::instance()->attach(this);
    //在多屏状态下，默认选择第一个屏幕
    QSize size = qApp->desktop()->screen()->size();

    G_User = nullptr;

    setFixedSize(Constant::LOGIN_FIX_WIDTH,Constant::LOGIN_FIX_HEIGHT);
    setGeometry((size.width() - Constant::LOGIN_FIX_WIDTH)/2,(size.height() - Constant::LOGIN_FIX_HEIGHT)/2,Constant::LOGIN_FIX_WIDTH,Constant::LOGIN_FIX_HEIGHT);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);

    createTrayMenu();
    loadLocalSettings();

    connect(TextNetConnector::instance(),SIGNAL(connected(bool)),this,SLOT(respTextConnect(bool)));
    connect(TextNetConnector::instance(),SIGNAL(socketError()),this,SLOT(respTextSocketError()));
    connect(FileNetConnector::instance(),SIGNAL(connected(bool)),this,SLOT(respFileConnect(bool)));
    connect(FileNetConnector::instance(),SIGNAL(socketError()),this,SLOT(respFileSocketError()));
    connect(MessDiapatch::instance(),SIGNAL(recvLoginResponse(ResponseLogin,LoginResponse)),this,SLOT(recvLoginResponse(ResponseLogin,LoginResponse)));
    connect(MessDiapatch::instance(),SIGNAL(recvFriendRequest(OperateFriendResponse)),this,SLOT(recvFriendResponse(OperateFriendResponse)));
    connect(MessDiapatch::instance(),SIGNAL(recvText(TextRequest)),this,SLOT(procRecvText(TextRequest)));

    connect(MessDiapatch::instance(),SIGNAL(recvTextReply(TextReply)),this,SLOT(processTextReply(TextReply)));
    connect(MessDiapatch::instance(),SIGNAL(recvFileControl(SimpleFileItemRequest)),this,SLOT(procFileControl(SimpleFileItemRequest)));
    connect(MessDiapatch::instance(),SIGNAL(recvFileRequest(FileItemRequest)),this,SLOT(procFileRequest(FileItemRequest)));
    connect(MessDiapatch::instance(),SIGNAL(recvFileData(QString,QString)),this,SLOT(procFileData(QString,QString)));
    QTimer::singleShot(0, this, SLOT(readLocalUser()));
}

/*!
 * @brief 响应连接文字服务器状态
 * @details 1.程序登陆或注册时会先执行连接；
 *          2.程序掉线后，手动的切换在线状态来实现重连，也会执行连接；
 * @param[in] flag:bool 文字服务器连接状态
 */
void LoginDialog::respTextConnect(bool flag)
{
    MQ_D(LoginDialog);

    if(G_User){
        G_User->setTextOnline(flag);
    }

    if(!flag)
    {
        RLOG_ERROR("Connect to server %s:%d error!",G_TextServerIp.toLocal8Bit().data(),G_TextServerPort);
        RMessageBox::warning(this,QObject::tr("Warning"),QObject::tr("Connect to text server error!"),RMessageBox::Yes);
    }else{
        //若G_User为NULL，则说明还未登陆过
        if(!G_User){
#ifndef __NO_SERVER__
            LoginRequest * request = new LoginRequest();
            request->accountId = d->userList->currentText();
            request->password = RUtil::MD5( d->password->text());
            request->status = d->onlineState->state();
            G_OnlineStatus = d->onlineState->state();
            RSingleton<MsgWrap>::instance()->handleMsg(request);
#else
            recvLoginResponse(LOGIN_SUCCESS,LoginResponse());
#endif
        }
        RSingleton<Subject>::instance()->notify(MESS_TEXT_NET_OK);
    }

    enableInput(true);
}

void LoginDialog::respTextSocketError()
{
    G_User->setTextOnline(false);
    RSingleton<Subject>::instance()->notify(MESS_TEXT_NET_ERROR);
}

void LoginDialog::respFileConnect(bool flag)
{
    if(G_User){
        G_User->setFileOnline(flag);
    }
    if(flag)
    {
#ifndef __NO_SERVER__
        qDebug()<<__FILE__<<__LINE__<<__FUNCTION__<<"FileServer Connected!!!!";
#endif
        RSingleton<Subject>::instance()->notify(MESS_FILE_NET_OK);
    }else{
        RLOG_ERROR("Connect to server %s:%d error!",G_FileServerIp.toLocal8Bit().data(),G_FileServerPort);
        RSingleton<Subject>::instance()->notify(MESS_FILE_NET_ERROR);
        RMessageBox::warning(this,QObject::tr("Warning"),QObject::tr("Connect to file server error!"),RMessageBox::Yes);
    }
}

void LoginDialog::respFileSocketError()
{
    G_User->setFileOnline(false);
    RSingleton<Subject>::instance()->notify(MESS_FILE_NET_ERROR);
}

void LoginDialog::login()
{
    MQ_D(LoginDialog);
#ifndef __NO_SERVER__
    TextNetConnector::instance()->connect();
    enableInput(false);
#else
    LoginResponse response;
    recvLoginResponse(LOGIN_SUCCESS, response);
#endif
}

void LoginDialog::minsize()
{
    MQ_D(LoginDialog);
    if(d->trayIcon)
    {
        if(!d->trayIcon->isVisible())
        {
            d->trayIcon->setVisible(true);
        }
    }

    this->setVisible(false);
}

void LoginDialog::closeWindow()
{
    RSingleton<TaskManager>::instance()->removeAll();
    this->close();
}

void LoginDialog::setPassword(bool flag)
{
    MQ_D(LoginDialog);
    if(!flag && d->autoLogin->isChecked())
    {
        d->autoLogin->setChecked(false);
    }
}

void LoginDialog::createTrayMenu()
{
    MQ_D(LoginDialog);
    d->trayIcon = new SystemTrayIcon();
    d->trayIcon->setModel(SystemTrayIcon::System_Login);
    d->trayIcon->setVisible(RUtil::globalSettings()->value(Constant::SETTING_TRAYICON,true).toBool());

    connect(d->trayIcon,SIGNAL(quitApp()),this,SLOT(closeWindow()));
    connect(d->trayIcon,SIGNAL(showMainPanel()),this,SLOT(showNormal()));
}

void LoginDialog::loadLocalSettings()
{
    RUtil::globalSettings()->beginGroup(Constant::SYSTEM_NETWORK);
    G_TextServerIp = RUtil::globalSettings()->value(Constant::SYSTEM_NETWORK_TEXT_IP,Constant::DEFAULT_NETWORK_TEXT_IP).toString();
    G_TextServerPort = RUtil::globalSettings()->value(Constant::SYSTEM_NETWORK_TEXT_PORT,Constant::DEFAULT_NETWORK_TEXT_PORT).toUInt();

    G_FileServerIp = RUtil::globalSettings()->value(Constant::SYSTEM_NETWORK_FILE_IP,Constant::DEFAULT_NETWORK_FILE_IP).toString();
    G_FileServerPort = RUtil::globalSettings()->value(Constant::SYSTEM_NETWORK_FILE_PORT,Constant::DEFAULT_NETWORK_FILE_PORT).toUInt();

    RUtil::globalSettings()->endGroup();
}

int LoginDialog::isContainUser()
{
    MQ_D(LoginDialog);
    return d->userIndex(d->userList->currentText());
}

void LoginDialog::resetDefaultInput()
{
    MQ_D(LoginDialog);

    d->isSystemUserIcon = true;

    d->password->setText(tr(""));
    d->autoLogin->setChecked(false);
    d->rememberPassord->setChecked(false);

    d->onlineState->setState(ProtocolType::STATUS_ONLINE);
    d->login->setEnabled(false);
    repolish(d->login);

    resetDefaultPixmap();
}

void LoginDialog::resetDefaultPixmap()
{
    MQ_D(LoginDialog);
    QString fullPath = RSingleton<ImageManager>::instance()->getSystemUserIcon();
    QFileInfo  info(fullPath);
    d->defaultUserIconPath = info.fileName();
    d->userIcon->setPixmap(fullPath);
}

void LoginDialog::readLocalUser()
{
    MQ_D(LoginDialog);

    resetDefaultPixmap();
    RSingleton<UserInfoFile>::instance()->readUsers(d->localUserInfo);

    if(d->localUserInfo.size() > 0)
    {
        respItemChanged(d->localUserInfo.first()->accountId);
        foreach(UserInfoDesc * user,d->localUserInfo)
        {
            ComboxItem * item = new ComboxItem;
            connect(item,SIGNAL(deleteItem(QString)),this,SLOT(removeUserItem(QString)));
            connect(item,SIGNAL(itemClicked(QString)),this,SLOT(respItemChanged(QString)));
            item->setNickName(user->userName);
            item->setAccountId(user->accountId);
            if(user->isSystemIcon)
            {
                item->setPixmap(RSingleton<ImageManager>::instance()->getSystemUserIcon(user->iconId));
            }
            else
            {
                User tmpUser(user->accountId);
                item->setPixmap(tmpUser.getFilePath(user->iconId));
            }

            QListWidgetItem* widgetItem = new QListWidgetItem;
            //需要向Item设置当前基本信息，否则会出现空字符串
            widgetItem->setText(user->accountId);
            d->userListWidget->addItem(widgetItem);
            d->userListWidget->setItemWidget(widgetItem,item);
        }
    }
    else
    {
        resetDefaultInput();
    }
}

/*!
 * @brief 验证输入的用户信息
 * @details 若本地保存的登陆历史记录中不包含当前用户输入的名称，则将登陆框置为默认，否则显示该用户对应的信息
 */
void LoginDialog::validateInput()
{
    MQ_D(LoginDialog);

    if(d->userList->currentText().size() >0 && d->password->text().size() > 0)
    {
        if(d->numberExp.exactMatch(d->userList->currentText()) && d->passExp.exactMatch(d->password->text()))
        {
            d->login->setEnabled(true);
            repolish(d->login);
            return;
        }
    }

    d->login->setEnabled(false);

    repolish(d->login);
}

void LoginDialog::validateUserName(QString name)
{
    MQ_D(LoginDialog);

    if(d->userIndex(name) == -1){
        resetDefaultInput();
    }else{
        respItemChanged(name);
    }

    validateInput();
}

void LoginDialog::enableInput(bool flag)
{
    MQ_D(LoginDialog);
    d->login->setEnabled(flag);
    d->password->setEnabled(flag);
    d->userList->setEnabled(flag);
    d->rememberPassord->setEnabled(flag);
    d->autoLogin->setEnabled(flag);

    repolish(d->login);
    repolish(d->password);
    repolish(d->userList);
    repolish(d->rememberPassord);
    repolish(d->autoLogin);
}

void LoginDialog::validatePassword(QString)
{
    validateInput();
}

void LoginDialog::showNetSettings()
{
    NetSettings * settings = new NetSettings(this);
    settings->initLocalSettings();
    settings->show();
}

void LoginDialog::removeUserItem(QString accountId)
{
    MQ_D(LoginDialog);

    int index = -1;
    for(int i = 0; i < d->userListWidget->count();i++)
    {
        QListWidgetItem * item = d->userListWidget->item(i);
        ComboxItem * comboxItem = dynamic_cast<ComboxItem *>(d->userListWidget->itemWidget(item));
        if(comboxItem && comboxItem->getAccountId() == accountId)
        {
            index = i;
            break;
        }
    }

    if(index >= 0)
    {
       int result = RMessageBox::warning(this,"warning",QString("Delete count %1 from list?").arg(accountId),RMessageBox::Yes|RMessageBox::No,RMessageBox::No);
       if(result == RMessageBox::Yes)
       {
            d->userListWidget->takeItem(index);
            QList<UserInfoDesc *>::iterator iter = d->localUserInfo.begin();
            while(iter != d->localUserInfo.end())
            {
                if((*iter)->accountId == accountId)
                {
                    d->localUserInfo.erase(iter);
                    break;
                }
                iter++;
            }
            RSingleton<UserInfoFile>::instance()->saveUsers(d->localUserInfo);
       }
    }
}

void LoginDialog::respItemChanged(QString id)
{
    MQ_D(LoginDialog);

    bool existed = false;
    QList<UserInfoDesc *>::iterator iter = d->localUserInfo.begin();
    while(iter != d->localUserInfo.end())
    {
        if((*iter)->accountId == id)
        {
            UserInfoDesc * userInfo = *iter;
            d->userList->hidePopup();

            d->password->clear();

            d->userList->setEditText(id);
            if(userInfo->isRemberPassword)
                d->password->setText(userInfo->originPassWord);
            d->rememberPassord->setChecked(userInfo->isRemberPassword);
            d->autoLogin->setChecked(userInfo->isAutoLogin);
            if(userInfo->isSystemIcon)
            {
                QPixmap pixmap(RSingleton<ImageManager>::instance()->getSystemUserIcon(userInfo->iconId));
                if(pixmap.isNull())
                {
                    userInfo->iconId = d->defaultUserIconPath;
                    d->userIcon->setPixmap(RSingleton<ImageManager>::instance()->getSystemUserIcon(userInfo->iconId));
                }
                else
                {
                    d->userIcon->setPixmap(RSingleton<ImageManager>::instance()->getSystemUserIcon(userInfo->iconId));
                }
            }
            else
            {
                User currUser(id);
                QString pixmap = currUser.getC2CImagePath()+QDir::separator()+userInfo->iconId;
                if(QFile(pixmap).exists())
                {
                    d->userIcon->setPixmap(pixmap);
                }
                else
                {
                    d->userIcon->setPixmap(RSingleton<ImageManager>::instance()->getSystemUserIcon(userInfo->iconId));
                }
            }

            d->onlineState->setState((OnlineStatus)userInfo->loginState);
            d->isNewUser = false;

            existed = true;
            break;
        }
        iter++;
    }

    if(!existed)
    {
        d->isNewUser = true;
    }
}

void LoginDialog::showRegistDialog()
{
    MQ_D(LoginDialog);
    disconnect(TextNetConnector::instance(),SIGNAL(connected(bool)),this,SLOT(respTextConnect(bool)));

    RegistDialog * dialog = new RegistDialog(this);
    connect(dialog,SIGNAL(destroyed(QObject*)),this,SLOT(respRegistDialogDestory(QObject*)));
    dialog->show();
    d->registView = true;
}

void LoginDialog::respRegistDialogDestory(QObject *)
{
    MQ_D(LoginDialog);
    connect(TextNetConnector::instance(),SIGNAL(connected(bool)),this,SLOT(respTextConnect(bool)));
    d->registView = false;
}

/*!
 * @brief 接收登陆结果
 * @param[in] status 登陆状态
 * @param[in] response 登陆结果信息
 */
void LoginDialog::recvLoginResponse(ResponseLogin status, LoginResponse response)
{
    MQ_D(LoginDialog);

    if(status == LOGIN_SUCCESS)
    {
        int index = -1;
        UserBaseInfo baseInfo = response.baseInfo;
        if( (index = isContainUser()) >= 0)
        {
            if(index < d->localUserInfo.size())
            {
                UserInfoDesc * uinfo = d->localUserInfo.at(index);
                uinfo->userName = response.baseInfo.nickName;
                uinfo->accountId = response.baseInfo.accountId;
                uinfo->originPassWord = d->password->text();
                uinfo->password = RUtil::MD5(d->password->text());
                uinfo->isAutoLogin =  d->autoLogin->isChecked();
                uinfo->isRemberPassword = d->rememberPassord->isChecked();
                uinfo->loginState = (int)d->onlineState->state();

                uinfo->isSystemIcon = response.baseInfo.isSystemIcon;
                uinfo->iconId = response.baseInfo.iconId;
                if(response.baseInfo.iconId.size() == 0){
                    uinfo->iconId = d->defaultUserIconPath;
                    baseInfo.iconId = uinfo->iconId;
                }

                if(index != 0){
                    d->localUserInfo.removeAt(index);
                    d->localUserInfo.prepend(uinfo);
                }

                RSingleton<UserInfoFile>::instance()->saveUsers(d->localUserInfo);
            }
        }
        else
        {
            UserInfoDesc * desc = new UserInfoDesc;
            desc->userName = response.baseInfo.nickName;
            desc->accountId = response.baseInfo.accountId;
            desc->originPassWord = d->password->text();
            desc->password = RUtil::MD5(d->password->text());
            desc->isAutoLogin = d->autoLogin->isChecked();
            desc->isRemberPassword = d->rememberPassord->isChecked();
            desc->loginState = (int)d->onlineState->state();
            desc->isSystemIcon = response.baseInfo.isSystemIcon;

            if(response.baseInfo.iconId.size() == 0){
                desc->iconId = d->defaultUserIconPath;
                baseInfo.iconId = desc->iconId;
            }else{
                desc->iconId = response.baseInfo.iconId;
            }

            d->localUserInfo.prepend(desc);
            RSingleton<UserInfoFile>::instance()->saveUsers(d->localUserInfo);
        }

        //TODO 对文件服务器进行重连、状态显示等操作
        FileNetConnector::instance()->connect();

        G_User = new User(baseInfo);
        G_User->setTextOnline(true);

        if(!baseInfo.isSystemIcon && !QFile(G_User->getFilePath(baseInfo.iconId)).exists() && baseInfo.iconId.size() > 0)
         {
             SimpleFileItemRequest * request = new SimpleFileItemRequest;
             request->control = T_REQUEST;
             request->itemType = FILE_ITEM_USER_DOWN;
             request->fileId = QFileInfo(baseInfo.iconId).baseName();
             FileRecvTask::instance()->addRecvItem(request);
         }

//        QString apppath = qApp->applicationDirPath() + QString(Constant::PATH_UserPath);
//        G_Temp_Picture_Path = QString("%1/%2/%3").arg(apppath).arg(G_User->BaseInfo().accountId).arg(Constant::UserTempName);
//        RUtil::createDir(G_Temp_Picture_Path);

        setVisible(false);
        d->trayIcon->disconnect();
        d->trayIcon->setModel(SystemTrayIcon::System_Main);

        if(!d->mainDialog)
        {
            d->mainDialog = new MainDialog();
        }

        if(!d->notifyWindow)
        {
            d->notifyWindow = new NotifyWindow;
            connect(d->notifyWindow,SIGNAL(showSystemNotifyInfo(NotifyInfo,int)),this,SLOT(viewSystemNotify(NotifyInfo,int)));
        }

        d->mainDialog->show();
        d->mainDialog->setLogInState(d->onlineState->state());

        FriendListRequest * request = new FriendListRequest;
        request->accountId = baseInfo.accountId;
        RSingleton<MsgWrap>::instance()->handleMsg(request);

        ChatGroupListRequest * groupRequest = new ChatGroupListRequest;
        groupRequest->uuid = baseInfo.uuid;
        RSingleton<MsgWrap>::instance()->handleMsg(groupRequest);
    }
    else
    {
        QString errorInfo;
        switch(status)
        {
            case LOGIN_UNREGISTERED:
                 errorInfo = QObject::tr("User not registered");
                 break;
            case LOGIN_PASS_ERROR:
                 errorInfo = QObject::tr("Incorrect password");
                 break;
            case LOGIN_SERVER_REFUSED:
                 errorInfo = QObject::tr("Server Unreachable");
                 break;
            case LOGIN_USER_LOGINED:
                errorInfo = QObject::tr("Account has sign in");
                break;
            case LOGIN_SERVER_NOT_RESP:
                 errorInfo = QObject::tr("");
                 break;
            case LOGIN_FAILED:
            default:
                 errorInfo = QObject::tr("Login Failed");
                 break;
        }

        RMessageBox::warning(this,"Warning",errorInfo,RMessageBox::Yes);
    }
}

/*!
 * @brief 接收好友操作响应信息
 * @details 可操作好友请求、同意请求、拒绝请求
 * @param[in] toolButton 待插入的工具按钮
 * @return 是否插入成功
 */
void LoginDialog::recvFriendResponse(OperateFriendResponse resp)
{
    MQ_D(LoginDialog);

    if(G_User->systemSettings()->soundAvailable)
        RSingleton<MediaPlayer>::instance()->play(MediaPlayer::MediaSystem);

    NotifyInfo  info;
    info.identityId = RUtil::UUID();
    info.accountId = resp.requestInfo.accountId;
    info.nickName = resp.requestInfo.nickName;
    info.isSystemIcon = resp.requestInfo.isSystemIcon;
    info.type = NotifySystem;
    info.stype = resp.stype;
    info.iconId = RSingleton<ImageManager>::instance()->getIcon(ImageManager::ICON_SYSTEMNOTIFY,ImageManager::ICON_64);
    info.ofriendResult = resp.result;
    info.chatId = resp.chatId;
    info.chatName = resp.chatName;

    d->notifyWindow->addNotifyInfo(info);
    d->notifyWindow->showMe();

    d->trayIcon->notify(SystemTrayIcon::SystemNotify,info.identityId);
}

//TODO 考虑若未打开窗口，消息如何存储？？
/*!
 * @brief 接收发送的聊天消息
 * @details
 *          1.存储消息至数据库；
 *          2.将信息添加至历史会话列表;
 *          3.  若聊天对象的窗口未创建，则使用系统通知，进行提示；
 *              若聊天对象的窗口已创建，但不可见，则将信息保存并将窗口设置可见
 * @param[in] response 消息体内容
 * @return 是否插入成功
 */
void LoginDialog::procRecvText(TextRequest response)
{
    MQ_D(LoginDialog);
    UserClient * client = RSingleton<UserManager>::instance()->client(response.otherSideId);
    if(client)
    {
        //【1】存储消息至数据库
        SimpleUserInfo userInfo;
        userInfo.accountId = "0";
        ChatInfoUnit unit = RSingleton<JsonResolver>::instance()->ReadJSONFile(response.sendData.toLocal8Bit());
        RSingleton<SQLProcess>::instance()->insertTableUserChatInfo(G_User->database(),unit,userInfo);

        //TODO 20180423【2】将信息添加至历史会话列表
        HistoryChatRecord record;
        record.accountId = client->simpleUserInfo.accountId;
        record.nickName = client->simpleUserInfo.nickName;
        record.dtime = RUtil::currentMSecsSinceEpoch();
        record.lastRecord = RUtil::getTimeStamp();
        record.systemIon = client->simpleUserInfo.isSystemIcon;
        record.iconId = client->simpleUserInfo.iconId;
        record.type = CHAT_C2C;
        MessDiapatch::instance()->onAddHistoryItem(record);

        //【3】判断窗口是否创建或者是否可见
        if(client->chatWidget && client->chatWidget->isVisible()){
            if(response.msgCommand == MSG_TEXT_TEXT){
                client->chatWidget->showRecentlyChatMsg();
            }else if(response.msgCommand == MSG_TEXT_SHAKE){
                if(G_User->systemSettings()->windowShaking)
                    client->chatWidget->shakeWindow();
            }
        }else{
            if(response.msgCommand == MSG_TEXT_SHAKE){
                if(!client->chatWidget){
                    client->chatWidget = new AbstractChatWidget();
                    client->chatWidget->setUserInfo(client->simpleUserInfo);
                    client->chatWidget->initChatRecord();
                }
                client->chatWidget->show();
            }else if(response.msgCommand == MSG_TEXT_TEXT){
                //TODO 未将文本消息设置到提示框中，待对加密、压缩等信息处理
                NotifyInfo  info;
                info.identityId = RUtil::UUID();
                info.msgCommand = response.msgCommand;
                info.accountId = response.otherSideId;
                info.nickName = client->simpleUserInfo.nickName;
                info.type = NotifyUser;
                info.stype = response.type;
                info.isSystemIcon = client->simpleUserInfo.isSystemIcon;
                info.iconId = RSingleton<ImageManager>::instance()->getSystemUserIcon();

                QString inserInfoId = d->notifyWindow->addNotifyInfo(info);
                d->notifyWindow->showMe();
                if(inserInfoId == info.identityId)
                {
                    d->trayIcon->notify(SystemTrayIcon::UserNotify,info.identityId,RSingleton<ImageManager>::instance()->getSystemUserIcon());
                }
                else
                {
                    d->trayIcon->frontNotify(SystemTrayIcon::UserNotify,inserInfoId);
                }
            }
        }

        if(G_User->systemSettings()->soundAvailable){
            if(response.msgCommand == MSG_TEXT_TEXT)
                RSingleton<MediaPlayer>::instance()->play(MediaPlayer::MediaMsg);
            else if(response.msgCommand == MSG_TEXT_SHAKE)
                RSingleton<MediaPlayer>::instance()->play(MediaPlayer::MediaShake);
        }
    }
}

/*!
 * @brief 接收系统消息反馈
 * @param[in] reply 消息回复
 * @return 无
 * @note 接收系统推送的消息反馈后，根据反馈的类型不同，执行相应的操作: @n
 *      1.APPLY_SYSTEM:将聊天信息存入对应用户的db; @n
 *      2.APPLY_CONFIRM:显示接收提示(弹窗或其它方式显示); @n
 *      3.APPLY_RECEIPT:更新回执状态(已读/未读);
 */
void LoginDialog::processTextReply(TextReply reply)
{
    AbstractChatWidget * chatWidget = NULL;
    TextRequest * msgDesc = RSingleton<MsgQueueManager>::instance()->dequeue(reply.textId);
    if(msgDesc != NULL)
    {
         UserClient * client = RSingleton<UserManager>::instance()->client(msgDesc->otherSideId);
         if(client && client->chatWidget != NULL)
         {
             chatWidget = client->chatWidget;
         }
    }

    switch(reply.applyType)
    {
        case APPLY_SYSTEM:
             {

             }
             break;
        default:
            break;
    }
}

/*!
 * @brief 处理服务器端的传输控制
 * @details 在传输完成后，从上传文件的队列中更新对应的状态: @n
 *          1.在上传完头像后，应该将fileId更新至服务器数据记录中，将头像改成自定义; @n
 *          2.在传输完成文件后，将fileId通过聊天服务器发送至对方，由对方去下载; @n
 * @param[in] request 控制命令
 * @return 无
 */
void LoginDialog::procFileControl(SimpleFileItemRequest request)
{
    R_CHECK_ONLINE;
    MQ_D(LoginDialog);
    if(FileRecvTask::instance()->containsTask(request.fileId))
    {
        if(request.control == T_ABLE_SEND)
        {
            FileRecvTask::instance()->transfer(request.fileId);
        }
        else if(request.control == T_OVER)
        {
            FileDesc * fileDesc = RSingleton<FileManager>::instance()->getFile(request.fileId);
            if(fileDesc != nullptr)
            {
                switch(static_cast<FileItemType>(fileDesc->itemType))
                {
                    case FILE_ITEM_USER_UP:
                            //登陆用户自己ID图片,更新界面显示，更新远程数据库信息
                            if(fileDesc->otherId == G_User->BaseInfo().accountId)
                            {
                                QFileInfo fileInfo(fileDesc->filePath);
                                QString newFileName = fileDesc->fileId+"."+fileInfo.suffix();
                                QString newPath = G_User->getC2CImagePath()+QDir::separator()+newFileName;
                                if(QFile::copy(fileDesc->filePath,newPath))
                                {
                                    G_User->BaseInfo().isSystemIcon = false;
                                    G_User->BaseInfo().iconId = newFileName;
                                    RSingleton<Subject>::instance()->notify(MESS_ICON_CHANGE);

                                    UpdateBaseInfoRequest * request = new UpdateBaseInfoRequest;

                                    request->baseInfo.accountId = G_User->BaseInfo().accountId;
                                    request->baseInfo.nickName = G_User->BaseInfo().nickName;
                                    request->baseInfo.signName = G_User->BaseInfo().signName;
                                    request->baseInfo.sexual = G_User->BaseInfo().sexual;
                                    request->baseInfo.birthday = G_User->BaseInfo().birthday;
                                    request->baseInfo.address = G_User->BaseInfo().address;
                                    request->baseInfo.email = G_User->BaseInfo().email;
                                    request->baseInfo.phoneNumber = G_User->BaseInfo().phoneNumber;
                                    request->baseInfo.remark = G_User->BaseInfo().remark;
                                    request->baseInfo.isSystemIcon = false;
                                    request->baseInfo.iconId = newFileName;
                                    request->requestType = UPDATE_USER_DETAIL;

                                    RSingleton<MsgWrap>::instance()->handleMsg(request);

                                    QList<UserInfoDesc *>::iterator iter = d->localUserInfo.begin();
                                    while(iter != d->localUserInfo.end())
                                    {
                                        if((*iter)->accountId == G_User->BaseInfo().accountId)
                                        {
                                            (*iter)->isSystemIcon = false;
                                            (*iter)->iconId = newFileName;
                                            break;
                                        }
                                        iter++;
                                    }

                                    RSingleton<UserInfoFile>::instance()->saveUsers(d->localUserInfo);
                                }
                            }
                            //好友ID图片
                            else if(false)
                            {
                                //TODO 处理好友的头像信息
                            }
                        break;
                    default:
                        break;
                }
            }

            RMessageBox::information(NULL,"title","file transferover \n fileId:"+request.fileId,RMessageBox::Yes);
        }
        else if(request.control == T_SERVER_EXIST)
        {
            RMessageBox::information(this,"title","Server file exist file!",RMessageBox::Yes);
            FileRecvTask::instance()->nextFile();
        }
    }
}

/*!
 * @brief 处理服务器发送的文件传输请求信息
 * @param[in] request 传输请求信息
 * @return 无
 */
void LoginDialog::procFileRequest(FileItemRequest response)
{
    if(RSingleton<FileManager>::instance()->addFile(response,G_User->getC2CImagePath()))
    {
//        qDebug()<<response.accountId<<"_"<<response.otherId<<"_"<<response.fileName;
        SimpleFileItemRequest * request = new SimpleFileItemRequest;
        request->control = T_ABLE_SEND;
        request->itemType = FILE_ITEM_CHAT_DOWN;
        request->fileId = response.fileId;
        request->md5 = response.md5;

        FileRecvTask::instance()->sendControlItem(request);
    }
}

/*!
 * @brief 文件数据接收完成
 * @param[in] fileId 文件ID
 * @param[in] fileName 数据库中文件名
 * @return 是否插入成功
 */
void LoginDialog::procFileData(QString fileId, QString fileName)
{
    FileDesc * fileDesc = RSingleton<FileManager>::instance()->getFile(fileId);
    if(fileDesc != nullptr)
    {
        switch(static_cast<FileItemType>(fileDesc->itemType))
        {
            case FILE_ITEM_USER_DOWN:
                //登陆用户自己ID图片,更新界面显示，更新远程数据库信息
                if(fileDesc->otherId == G_User->BaseInfo().accountId)
                {
                    QString localFileName = G_User->getC2CImagePath()+QDir::separator()+fileName;
                    if(QFile(localFileName).exists())
                    {
                        QFileInfo fileInfo(localFileName);
                        QString newName = G_User->getC2CImagePath()+QDir::separator() + fileId + "." + fileInfo.suffix();

                        QFile::rename(localFileName,newName);

                        if(fileDesc->isRecvOver())
                        {
                            RSingleton<FileManager>::instance()->removeFile(fileId);
                        }

                        RSingleton<Subject>::instance()->notify(MESS_ICON_CHANGE);
                    }
                }
                else
                {
                    //TODO 处理好友的头像信息
                }
                break;
            default:
                break;
        }
    }
}

/*!
 * @brief 查看系统通知消息
 * @param[in] info 通知消息内容
 * @return 无
 */
void LoginDialog::viewSystemNotify(NotifyInfo info,int notifyCount)
{
    MQ_D(LoginDialog);
    Q_UNUSED(notifyCount);
    if(info.type == NotifySystem)
    {
        ResponseFriendApply reqType = (ResponseFriendApply)info.ofriendResult;

        SystemNotifyView * view = new SystemNotifyView();
        connect(view,SIGNAL(chatWidth(QString)),this,SLOT(openChatDialog(QString)));

        if(info.stype == OperatePerson){
            //解决:AB同时向对方发送请求，A先接收B的请求，此时已是好友，但B后查看通知消息，造成状态不一致。
            if(reqType == FRIEND_REQUEST && RSingleton<UserFriendContainer>::instance()->containUser(info.accountId)){
                reqType = FRIEND_AGREE;
            }
        }else if(info.stype == OperateGroup){

        }

        view->setNotifyInfo(info);
        view->setNotifyType(reqType);
        view->show();
    }
    else if(info.type == NotifyUser)
    {
        UserClient * client = RSingleton<UserManager>::instance()->client(info.accountId);
        if(client)
        {
            if(client->chatWidget == NULL)
            {
                client->chatWidget = new AbstractChatWidget();
                client->chatWidget->setUserInfo(client->simpleUserInfo);
                client->chatWidget->initChatRecord();
            }

//            client->chatWidget->showRecentlyChatMsg(notifyCount);
            client->chatWidget->show();
        }
    }

    d->trayIcon->removeNotify(info.identityId);
}

void LoginDialog::openChatDialog(QString accountId)
{
   UserClient * client = RSingleton<UserManager>::instance()->client(accountId);
   if(client->chatWidget == NULL)
   {
       client->chatWidget = new AbstractChatWidget();
       client->chatWidget->setUserInfo(client->simpleUserInfo);
       client->chatWidget->initChatRecord();
   }

   client->chatWidget->show();
}

LoginDialog::~LoginDialog()
{
    MQ_D(LoginDialog);

    RSingleton<Subject>::instance()->detach(this);

    if(d->toolBar)
    {
        delete d->toolBar;
        d->toolBar = NULL;
    }

    if(d->mainDialog)
    {
        delete d->mainDialog;
        d->mainDialog = NULL;
    }

    if(d->trayIcon)
    {
       delete d->trayIcon;
       d->trayIcon = NULL;
    }

    if(d->notifyWindow){
        delete d->notifyWindow;
        d->notifyWindow = NULL;
    }
}

void LoginDialog::onMessage(MessageType type)
{
    MQ_D(LoginDialog);
    switch(type)
    {
        case MESS_SETTINGS:
            {
                d->trayIcon->setVisible(RUtil::globalSettings()->value(Constant::SETTING_TRAYICON,true).toBool());
                break;
            }
        case MESS_NOTIFY_WINDOWS:
            {
                d->notifyWindow->showMe();
                break;
            }
        case MESS_BASEINFO_UPDATE:
            {
                QList<UserInfoDesc *>::iterator iter = d->localUserInfo.begin();
                while(iter != d->localUserInfo.end())
                {
                    if((*iter)->accountId == G_User->BaseInfo().accountId)
                    {
                        (*iter)->isSystemIcon = G_User->BaseInfo().isSystemIcon;
                        (*iter)->iconId = G_User->BaseInfo().iconId;
                        break;
                    }
                    iter++;
                }

                RSingleton<UserInfoFile>::instance()->saveUsers(d->localUserInfo);
            }
        default:
                break;
    }
}

void LoginDialog::resizeEvent(QResizeEvent *)
{
    MQ_D(LoginDialog);

    d->toolBar->setGeometry(WINDOW_MARGIN_SIZE,0,width() - WINDOW_MARGIN_SIZE * 3,d->toolBar->size().height());

    QRect ioconRect = d->userIcon->geometry();
    d->onlineState->setGeometry(ioconRect.right() - 20,ioconRect.bottom() - 22,30,30);
}

void LoginDialog::closeEvent(QCloseEvent *)
{
    qApp->exit();
}

bool LoginDialog::eventFilter(QObject *obj, QEvent *event)
{
    MQ_D(LoginDialog);

    if(event->type() == QEvent::KeyPress)
    {
        if(obj == d->password)
        {
            QKeyEvent * keyEvent = dynamic_cast<QKeyEvent *>(event);
            if(!d->isNewUser && keyEvent->key() == Qt::Key_Backspace)
            {
                d->password->setText("");
                validateInput();
                return true;
            }
        }
    }

    return Widget::eventFilter(obj,event);
}

void LoginDialog::keyPressEvent(QKeyEvent *event)
{
    MQ_D(LoginDialog);
    if(event->key() == Qt::Key_Return && !d->registView){
        if(d->login->isEnabled()){
            login();
        }
    }
}

ComboxItem::ComboxItem(QWidget *parent):QWidget(parent)
{
    QWidget * widget = new QWidget(this);

    iconLabel = new RIconLabel;
    iconLabel->setTransparency(true);
    iconLabel->setEnterCursorChanged(false);
    iconLabel->setEnterHighlight(false);
    iconLabel->setCorner(true);

    iconLabel->setPixmap(RSingleton<ImageManager>::instance()->getSystemUserIcon());

    nickNameLabel = new QLabel(widget);
    nickNameLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    nickNameLabel->setAlignment(Qt::AlignLeft);
    nickNameLabel->setMinimumWidth(125);
    nickNameLabel->setMinimumHeight(22);

    accountLabel = new QLabel(widget);
    accountLabel->setMinimumHeight(22);
    accountLabel->setAlignment(Qt::AlignLeft);

    closeButt = new QPushButton;
    closeButt->setText("X");
    closeButt->setFixedSize(20,20);
    closeButt->setToolTip(QObject::tr("Remove account"));
    connect(closeButt,SIGNAL(pressed()),this,SLOT(prepareDelete()));

    QGridLayout * gridlayout = new QGridLayout;
    gridlayout->setContentsMargins(2,2,2,2);
    gridlayout->addWidget(iconLabel,0,0,2,1);
    gridlayout->addWidget(nickNameLabel,0,1,1,6);
    gridlayout->addWidget(accountLabel,1,1,1,6);
    gridlayout->addWidget(closeButt,0,8,2,1);
    gridlayout->setHorizontalSpacing(1);
    gridlayout->setVerticalSpacing(1);

    widget->setLayout(gridlayout);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(widget);
    setLayout(layout);
}

ComboxItem::~ComboxItem()
{

}

void ComboxItem::setNickName(QString name)
{
    nickNameLabel->setText(name);
}

void ComboxItem::setPixmap(QString pixmap)
{
    iconLabel->setPixmap(pixmap);
}

void ComboxItem::setAccountId(QString id)
{
    accountLabel->setText(id);
}

QString ComboxItem::getAccountId()
{
    return accountLabel->text();
}

void ComboxItem::mouseReleaseEvent(QMouseEvent *)
{
    emit itemClicked(accountLabel->text());
}

void ComboxItem::prepareDelete()
{
    emit deleteItem(accountLabel->text());
}
