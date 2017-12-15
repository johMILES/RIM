/*!
 *  @brief     历史聊天页面
 *  @details   显示登录用户历史与联系人交流的记录
 *  @file      panelhistorypage.h
 *  @author    wey
 *  @version   1.0
 *  @date      2017.12.15
 *  @warning
 *  @copyright GNU Public License.
 */
#ifndef PANELHISTORYPAGE_H
#define PANELHISTORYPAGE_H

#include <QWidget>

class PanelHistoryPagePrivate;

class PanelHistoryPage : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(PanelHistoryPage)
public:
    explicit PanelHistoryPage(QWidget * parent = 0);
    ~PanelHistoryPage();

private:
    void initWidget();

private:
    PanelHistoryPagePrivate * d_ptr;

    QWidget * contentWidget;
};

#endif // PANELHISTORYPAGE_H
