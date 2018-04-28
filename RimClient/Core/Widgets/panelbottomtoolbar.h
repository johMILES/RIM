﻿/*!
 *  @brief     主面板底部菜单栏
 *  @details
 *  @file      panelbottomtoolbar.h
 *  @author    wey
 *  @version   1.0
 *  @date      2017.12.14
 *  @warning
 *  @copyright NanJing RenGu.
 */

#ifndef PANELBOTTOMTOOLBAR_H
#define PANELBOTTOMTOOLBAR_H

#include <QWidget>

#include "observer.h"

class QToolButton;
class PanelBottomToolBarPrivate;

class PanelBottomToolBar : public  QWidget , public Observer
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(PanelBottomToolBar)
public:
    explicit PanelBottomToolBar(QWidget * parent = 0);
    ~PanelBottomToolBar();

    void onMessage(MessageType mtype);

signals:
    void showSearchPerson();

private slots:
    void showAddFriendPanel();
    void showSystemSetting();
    void updateFrinedInstance(QObject *);
    void updateSettingInstnce(QObject *);
    void showNotifyWindow();

private:
    PanelBottomToolBarPrivate * d_ptr;
};

#endif // PANELBOTTOMTOOLBAR_H
