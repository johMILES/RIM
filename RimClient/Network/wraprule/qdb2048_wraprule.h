/*!
 *  @brief     2048数据封装/解析
 *  @details   用于对2048协议进行拆包和封装
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.10
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef QDB2048_WRAPRULE_H
#define QDB2048_WRAPRULE_H

#ifdef __LOCAL_CONTACT__

#include "network/wraprule/wraprule.h"
using namespace ClientNetwork;

class NETWORKSHARED_EXPORT QDB2048_WrapRule : public WrapRule
{
public:
    QDB2048_WrapRule();

    void wrap(ProtocolPackage & data);
    bool unwrap(const QByteArray & data,ProtocolPackage & result);
};

#endif

#endif // QDB2048_WRAPRULE_H
