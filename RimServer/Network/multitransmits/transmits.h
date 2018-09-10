/*!
 *  @brief     传输方式管理
 *  @details   提供对不同的传输链路的信息维护
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.10
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TRANSMITS_H_2018_09_10
#define TRANSMITS_H_2018_09_10

#include "basetransmit.h"
#include <map>
#include <mutex>
#include <functional>

namespace ServerNetwork{

/*!
 *  @brief 传输方式管理
 *  @details 负责维护建立的连接，管理维护其生命周期
 */
class NETWORKSHARED_EXPORT Transmits{

public:
    Transmits();

    typedef std::pair<CommMethod,BaseTransmitPtr> PairTransmit;
    typedef std::function<void(PairTransmit)> PairFunc;

    void insert(BaseTransmitPtr tras);

    unsigned int size(){
        std::lock_guard<std::mutex> lg(lock);
        return transmits.size();
    }

    void for_each(PairFunc func);

    template<class T>
    std::shared_ptr<T> at(CommMethod method);

public:
    std::map<CommMethod,std::shared_ptr<BaseTransmit>> transmits;
    std::mutex lock;
};

template<class T>
std::shared_ptr<T> Transmits::at(CommMethod method)
{
    if(transmits.at(method).get() != NULL){
        std::shared_ptr<T> pt = std::dynamic_pointer_cast<T>(transmits.at(method));
        return pt;
    }
    return NULL;
}

}

#endif // TRANSMITS_H
