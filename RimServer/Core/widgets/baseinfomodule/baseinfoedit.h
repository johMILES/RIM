#ifndef BASEINFOEDIT_H
#define BASEINFOEDIT_H

#include <QSharedPointer>
#include "Base/selfwidget/dialogproxy.h"

class BaseInfoEditPrivate;

class BaseInfoEdit : public DialogProxy
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(BaseInfoEdit)
public:
    explicit BaseInfoEdit(QWidget *parent = 0);
    ~BaseInfoEdit();

    QSize sizeHint()const;

protected:
    void respButtClicked(StandardButton butt);

private slots:
    void chooseFloder();

private:
    bool checkInputValid();
    void cancel();
    void confirm();

private:
    QSharedPointer<BaseInfoEditPrivate> d_ptr;

};

#endif // BASEINFOEDIT_H
