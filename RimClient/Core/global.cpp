﻿#include "global.h"
#include <QApplication>
#include <QAction>

QString G_TextServerIp;                         //文本服务器IP
unsigned short G_TextServerPort;                //文本服务器监听端口

QString G_FileServerIp;                         //文件服务器IP
unsigned short G_FileServerPort;                //文件服务器监听端口

//TODO
QString G_ScreenShotKeySequence;

OnlineStatus G_OnlineStatus;                   //在线状态
UserBaseInfo G_UserBaseInfo;                   //登陆成功后接收用户基本信息
QList<RGroupData *> G_FriendList;              //保存登陆用户的好友列表信息

QString G_Temp_Picture_Path;
QAction *G_pScreenShotAction = NULL;
ScreenShot *G_pScreenShot = NULL;
int G_mIsEnter;

