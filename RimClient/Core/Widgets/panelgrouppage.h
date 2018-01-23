﻿/*!
 *  @brief     群信息界面
 *  @details   显示登录用户群信息
 *  @file      panelgrouppage.h
 *  @author    wey
 *  @version   1.0
 *  @date      2017.12.15
 *  @warning
 *  @copyright NanJing RenGu.
 *  @change
 *      date:20180122 content:添加群分组重命名 name:LYS
 */
#ifndef PANELGROUPPAGE_H
#define PANELGROUPPAGE_H

#include <QWidget>

class PanelGroupPagePrivate;

class PanelGroupPage : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(PanelGroupPage)
public:
    explicit PanelGroupPage(QWidget *parent = 0);
    ~PanelGroupPage();

private slots:
    void searchGroup();
    void newGroup();
    void addGroups();

    void renameGroup();
    void deleteGroup();

    void sendMessage();
    void viewDetail();
    void modifyGroupInfo();
    void exitGroup();

public slots:
    void renameEditFinished();

private:
    void createAction();

private:
    PanelGroupPagePrivate * d_ptr;

};

#endif // PANELGROUPPAGE_H
