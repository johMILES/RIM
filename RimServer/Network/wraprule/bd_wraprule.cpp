#include "bd_wraprule.h"

namespace ServerNetwork{

BD_WrapRule::BD_WrapRule()
{

}

void BD_WrapRule::registDataHandler(Handler *dataHandler)
{
    textHandler = dataHandler;
}

void BD_WrapRule::wrap(ProtocolPackage &data)
{

}

bool BD_WrapRule::unwrap(const QByteArray &data, ProtocolPackage &result)
{
    return false;
}

}
