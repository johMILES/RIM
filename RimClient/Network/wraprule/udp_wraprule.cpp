﻿#include "udp_wraprule.h"

#ifdef __LOCAL_CONTACT__

#include "qdb61a_wraprule.h"
#include "tk205_wraprule.h"
#include "util/rsingleton.h"
#include <QByteArray>

namespace ClientNetwork{

UDP_WrapRule::UDP_WrapRule():WrapRule()
{

}

void UDP_WrapRule::wrap(ProtocolPackage &data)
{

}

bool UDP_WrapRule::unwrap(const QByteArray &data, ProtocolPackage &result)
{

    return false;
}

} //namespace ClientNetwork

#endif
