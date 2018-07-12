﻿#include "tcp_wraprule.h"

#ifdef __LOCAL_CONTACT__

#include "qdb495_wraprule.h"
#include "qdb21_wraprule.h"
#include "qdb2051_wraprule.h"
#include "qdb2048_wraprule.h"
#include "Util/rsingleton.h"
#include <QDebug>

namespace ServerNetwork{

TCP_WrapRule::TCP_WrapRule():WrapRule()
{

}

void TCP_WrapRule::wrap(ProtocolPackage &data)
{
    if(data.usOrderNo == O_2051)
    {
        RSingleton<QDB2051_WrapRule>::instance()->wrap(data);
    }
    else if(data.usOrderNo == O_2048)
    {
        RSingleton<QDB2048_WrapRule>::instance()->wrap(data);
    }

    RSingleton<QDB21_WrapRule>::instance()->wrap(data);
}

bool TCP_WrapRule::unwrap(const QByteArray &data, ProtocolPackage &result)
{
    if(!RSingleton<QDB21_WrapRule>::instance()->unwrap(data,result))
    {
        return false;
    }

    if(result.usOrderNo == O_2051)
    {
        if(!RSingleton<QDB2051_WrapRule>::instance()->unwrap(result.data,result))
            return false;
    }
    else if(result.usOrderNo == O_2048)
    {
        if(!RSingleton<QDB2048_WrapRule>::instance()->unwrap(result.data,result))
            return false;
    }
    else
    {
        return false;
    }

    return true;
}

} //namespace ServerNetwork

#endif
