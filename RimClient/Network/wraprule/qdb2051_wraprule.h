/*!
 *  @brief     2051数据封装/解析
 *  @details   用于对2051协议进行拆包和封装
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.10
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef QDB2051_WRAPRULE_H
#define QDB2051_WRAPRULE_H

#ifdef __LOCAL_CONTACT__

#include "network/wraprule/wraprule.h"

namespace ClientNetwork{

class NETWORKSHARED_EXPORT QDB2051_WrapRule : public WrapRule
{
public:
    QDB2051_WrapRule();

    void wrap(ProtocolPackage & data);
    bool unwrap(const QByteArray & data,ProtocolPackage & result);
};

}

#endif

#endif // QDB2051_WRAPRULE_H
