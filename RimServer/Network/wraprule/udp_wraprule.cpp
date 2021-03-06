﻿#include "udp_wraprule.h"

#ifdef __LOCAL_CONTACT__

#include "qdb61a_wraprule.h"
#include "tk205_wraprule.h"
#include "Base/util/rsingleton.h"

namespace ServerNetwork{

UDP_WrapRule::UDP_WrapRule():WrapRule()
{

}

void UDP_WrapRule::wrap(ProtocolPackage &data)
{
    Q_UNUSED(data)
}

bool UDP_WrapRule::unwrap(const QByteArray &data, ProtocolPackage &result)
{
    Q_UNUSED(data)
    Q_UNUSED(result)
    return false;
}

} //namespace ServerNetwork

#endif
