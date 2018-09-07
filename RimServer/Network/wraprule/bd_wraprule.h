#ifndef BD_WRAPRULE_H
#define BD_WRAPRULE_H

#ifdef __LOCAL_CONTACT__

#include "wraprule.h"
#include "../../BD/bduser.h"

namespace ServerNetwork{

class Handler;

class NETWORKSHARED_EXPORT BD_WrapRule : public WrapRule
{
public:
    BD_WrapRule();

    void registDataHandler(Handler * dataHandler);
    void wrap(ProtocolPackage & data);
    bool unwrap(const QByteArray & data,ProtocolPackage & result);
    bool unwrap_TXA(const QByteArray & data,USET_TXA & result);

private:
    Handler * textHandler;                /*!< 数据处理器 */
};

}

#endif

#endif // BD_WRAPRULE_H
