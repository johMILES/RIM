﻿#ifndef UDP_WRAPRULE_H
#define UDP_WRAPRULE_H

#ifdef __LOCAL_CONTACT__

#include "wraprule.h"
namespace ServerNetwork{

class NETWORKSHARED_EXPORT UDP_WrapRule : public WrapRule
{
public:
    UDP_WrapRule();

    void wrap(ProtocolPackage & data);
    bool unwrap(const QByteArray & data,ProtocolPackage & result);
};

} //namespace ServerNetwork

#endif

#endif // UDP_WRAPRULE_H
