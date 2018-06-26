﻿#include "udp_wraprule.h"

#ifdef __LOCAL_CONTACT__

#include "qdb61a_wraprule.h"
#include "tk205_wraprule.h"
#include "rsingleton.h"

UDP_WrapRule::UDP_WrapRule():WrapRule()
{

}

QByteArray UDP_WrapRule::wrap(const ProtocolPackage &package)
{
    ProtocolPackage tempPack = package;
    QByteArray wrapdata = RSingleton<TK205_WrapRule>::instance()->wrap(tempPack);
    tempPack.cFileData = wrapdata;
    wrapdata = RSingleton<QDB61A_WrapRule>::instance()->wrap(tempPack);
    return wrapdata;
}

ProtocolPackage UDP_WrapRule::unwrap(const QByteArray &data)
{
    ProtocolPackage package;
    package.cFileData = data;
    return package;
}
#endif
