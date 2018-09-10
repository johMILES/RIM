/*!
 *  @brief     716tcp数据封装/解析
 *  @details   用于对采用TCP传输格式的数据进行封装和解包，此封装只用于将多种子封装格式进行组合。
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.10
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TCP_WRAPRULE_H
#define TCP_WRAPRULE_H

#ifdef __LOCAL_CONTACT__

#include "wraprule.h"
namespace ClientNetwork{

class NETWORKSHARED_EXPORT TCP_WrapRule : public WrapRule
{
public:
    TCP_WrapRule();

    void wrap(ProtocolPackage & data);
    bool unwrap(const QByteArray & data,ProtocolPackage & result);
};

} //namespace ClientNetwork

#endif

#endif // TCP_WRAPRULE_H
