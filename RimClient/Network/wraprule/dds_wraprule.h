/*!
 *  @brief     DDS数据封装/解析
 *  @details   用于对DDS接收的数据进行解析和封装
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.10
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef DDS_WRAPRULE_H
#define DDS_WRAPRULE_H

#ifdef __LOCAL_CONTACT__

#include "network/wraprule/wraprule.h"
using namespace ClientNetwork;

class DDS_WrapRule : public WrapRule
{
public:
    DDS_WrapRule();

    QByteArray wrap(const ProtocolPackage &package);
    QByteArray unwrap(const QByteArray &data);
};

#endif

#endif // DDS_WRAPRULE_H
