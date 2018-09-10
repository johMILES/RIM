/*!
 *  @brief     21数据封装/解析
 *  @details   用于对21协议进行拆包和封装
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.10
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef QDB21_WRAPRULE_H
#define QDB21_WRAPRULE_H

#ifdef __LOCAL_CONTACT__

#include "network/wraprule/wraprule.h"
using namespace ClientNetwork;

class NETWORKSHARED_EXPORT QDB21_WrapRule : public WrapRule
{
public:
    QDB21_WrapRule();

    void wrap(ProtocolPackage & data);
    bool unwrap(const QByteArray & data,ProtocolPackage & result);

private:
    void wrapTime(char *output, int intput, int length);
    void unwrapTime(int &output, char *input, int length);
};

#endif

#endif // QDB21_WRAPRULE_H
