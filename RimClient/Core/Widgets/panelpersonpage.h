﻿/*!
 *  @brief     用户界面
 *  @details   显示用户的联系人信息
 *  @file      panelpersonpage.h
 *  @author    wey
 *  @version   1.0
 *  @date      2017.12.15
 *  @warning
 *  @copyright NanJing RenGu.
 *  @change
 *      date:20180118 content:添加联系人分组重命名 name:LYS
 *      date:20180122 content:添加移动联系人至其他分组功能 name:LYS
 *
 */
#ifndef PANELPERSONPAGE_H
#define PANELPERSONPAGE_H

#include <QWidget>

class PanelPersonPagePrivate;
class ToolItem;
class ToolPage;

class PanelPersonPage : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(PanelPersonPage)
public:
    explicit PanelPersonPage(QWidget * parent =0);
    ~PanelPersonPage();

private slots:
    void refreshList();
    void addGroup();
    void renameGroup();
    void delGroup();

    void createChatWindow(ToolItem * item);

    void sendInstantMessage();
    void showUserDetail();
    void modifyUserInfo();
    void deleteUser();

public slots:
    void renameEditFinished();
    void updateGroupActions(ToolPage *);
    void movePersonTo();


private:
    void createAction();

private:
    PanelPersonPagePrivate * d_ptr;
};

#endif // PANELPERSONPAGE_H
