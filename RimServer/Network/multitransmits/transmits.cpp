#include "transmits.h"

namespace ServerNetwork{

Transmits::Transmits(){

}

void Transmits::insert(BaseTransmitPtr tras)
{
    std::lock_guard<std::mutex> lg(lock);
    transmits.insert(std::pair<CommMethod,BaseTransmitPtr>(tras->type(),tras));
}

void Transmits::for_each(PairFunc func)
{
    std::lock_guard<std::mutex> lg(lock);
    std::for_each(transmits.begin(),transmits.end(),func);
}

}
