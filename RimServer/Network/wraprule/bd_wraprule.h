#ifndef BD_WRAPRULE_H
#define BD_WRAPRULE_H

#include "wraprule.h"

namespace ServerNetwork{

class Handler;

class BD_WrapRule : public WrapRule
{
public:
    BD_WrapRule();

    void registDataHandler(Handler * dataHandler);
    void wrap(ProtocolPackage & data);
    bool unwrap(const QByteArray & data,ProtocolPackage & result);

private:
    Handler * textHandler;                /*!< 数据处理器 */
};

}

#endif // BD_WRAPRULE_H
