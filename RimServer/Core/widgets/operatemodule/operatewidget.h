/*!
 *  @brief     操作信息显示窗口
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.24
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef OPERATEWIDGET_H
#define OPERATEWIDGET_H

#include <QWidget>

namespace Ui {
class OperateWidget;
}

class OperateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OperateWidget(QWidget *parent = 0);
    ~OperateWidget();

    QSize sizeHint()const;

private slots:
    void showRealRecord(QString content);

private:
    Ui::OperateWidget *ui;
};

#endif // OPERATEWIDGET_H
