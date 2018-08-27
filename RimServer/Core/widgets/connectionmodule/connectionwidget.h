/*!
 *  @brief     连接信息监控窗口
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.24
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef CONNECTIONWIDGET_H
#define CONNECTIONWIDGET_H

#include <QWidget>

namespace Ui {
class ConnectionWidget;
}

class ConnectionMonitorModel;
class QTimer;

class ConnectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionWidget(QWidget *parent = 0);
    ~ConnectionWidget();

private slots:
    void udpateClientInfo();

private:
    void initView();

private:
    Ui::ConnectionWidget *ui;

    ConnectionMonitorModel * tableModel;
    QTimer * clientFreshTimer;

};

#endif // CONNECTIONWIDGET_H
