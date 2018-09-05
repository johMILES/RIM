/*!
 *  @brief     数据库信息编辑
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.02
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef DATABASEEDIT_H
#define DATABASEEDIT_H

#include <QSharedPointer>
#include "Base/selfwidget/dialogproxy.h"

namespace Datastruct {
struct DatabaseConfigInfo;
}

class DatabaseEditPrivate;

class DatabaseEdit : public DialogProxy
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DatabaseEdit)
public:
    explicit DatabaseEdit(QWidget *parent = 0);
    ~DatabaseEdit();

    QSize sizeHint()const;

protected:
    void respButtClicked(StandardButton butt);

private slots:
    void cancel();
    void confirm();
    void testDatabaseConnect();

private:
    bool checkInputValid();
    bool validateText(QWidget * parent,QString & text,QString & uiName);
    void getDatabaseSetting(Datastruct::DatabaseConfigInfo * info);

private:
    QSharedPointer<DatabaseEditPrivate> d_ptr;
};

#endif // DATABASEEDIT_H
