/*!
 *  @brief     模态对话框代理
 *  @details   此类将一般设置对话框中的取消和确定抽象为单独窗口，由子类填充中心区域widget
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.31
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef DIALOGPROXY_H
#define DIALOGPROXY_H

#include <QDialog>

#include "../base_global.h"

class DialogProxyPrivate;

class BASESHARED_EXPORT DialogProxy : public QDialog
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DialogProxy)
public:
    explicit DialogProxy(QWidget * parent = 0);
    ~DialogProxy();

    enum StandardButton
    {
        NoButton           = 0x00000000,
        Ok                 = 0x00000400,
        Save               = 0x00000800,
        SaveAll            = 0x00001000,
        Open               = 0x00002000,
        Yes                = 0x00004000,
        YesToAll           = 0x00008000,
        No                 = 0x00010000,
        NoToAll            = 0x00020000,
        Abort              = 0x00040000,
        Retry              = 0x00080000,
        Ignore             = 0x00100000,
        Close              = 0x00200000,
        Cancel             = 0x00400000,
        Discard            = 0x00800000,
        Help               = 0x01000000,
        Apply              = 0x02000000,
        Reset              = 0x04000000,
        RestoreDefaults    = 0x08000000,

        FirstButton        = Ok,                // internal
        LastButton         = RestoreDefaults,   // internal

        YesAll             = YesToAll,          // obsolete
        NoAll              = NoToAll,           // obsolete
    };
    Q_DECLARE_FLAGS(StandardButtons, StandardButton)

    void setButton(int butts);
    void setContentWidget(QWidget * widget);
signals:
    void buttClicked(StandardButton butt);

protected:
    virtual void respButtClicked(StandardButton butt);

private slots:
    void respButtonClicked();

private:
    void addButton(DialogProxy::StandardButton buttType);
    QString standardButtText(DialogProxy::StandardButton butt);

private:
    DialogProxyPrivate * d_ptr;
};

#endif // DIALOGPROXY_H
