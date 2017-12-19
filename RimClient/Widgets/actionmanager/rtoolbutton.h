/*!
 *  @brief     工具按钮
 *  @details   继承QToolButton，添加了样式操作
 *  @file
 *  @author    wey
 *  @version   1.0
 *  @date      2017.12.XX
 *  @warning
 *  @copyright GNU Public License.
 *  @note      20171216:wey:默认在设置objName时，自动根据objName设置Icon
 *             20171218:wey:重载对toolTip事件处理
 */
#ifndef RTOOLBUTTON_H
#define RTOOLBUTTON_H

#include <QToolButton>

class RToolButtonPrivate;

class RToolButton : public QToolButton
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(RToolButton)
public:
    explicit RToolButton(QWidget *parent = NULL);
    ~RToolButton();

    void setToolTip(const QString & toolTipInfo);
    void setObjectName(QString objName);

protected:
    bool event(QEvent *e);

private:
    RToolButtonPrivate * d_ptr;

};

#endif // RTOOLBUTTON_H
