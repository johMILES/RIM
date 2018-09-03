/*!
 *  @brief     实现跨模块/跨线程之间的信号传递
 *  @details   便于在两个不直接关连的模块之间传递信号
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.03
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef MESSDIAPATCH_H
#define MESSDIAPATCH_H

#include <QObject>

class MessDispatch : public QObject
{
    Q_OBJECT
public:
    explicit MessDispatch(QObject *parent = 0);

    void onRecvRealRecord(QString content);

signals:
    void realRecord(QString content);

public slots:


};

#endif // MESSDIAPATCH_H
