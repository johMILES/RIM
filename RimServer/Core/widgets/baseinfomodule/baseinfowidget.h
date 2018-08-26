/*!
 *  @brief     基本信息监视窗口
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.24
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef BASEINFOWIDGET_H
#define BASEINFOWIDGET_H

#include <QWidget>

namespace Ui {
class BaseInfoWidget;
}

class BaseInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BaseInfoWidget(QWidget *parent = 0);
    ~BaseInfoWidget();

    void updateBaseInfo();
    void updateDatabaseInfo();

private slots:
    void respBaseInfoEdit();
    void respDatabaseEdit();

private:
    void initView();

private:
    Ui::BaseInfoWidget *ui;
};

#endif // BASEINFOWIDGET_H
