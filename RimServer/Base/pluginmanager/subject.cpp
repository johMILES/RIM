#include "subject.h"

#include "observer.h"

Subject::Subject()
{

}

void Subject::attach(Observer *obj)
{
    std::lock_guard<std::mutex> lg(m_mutex);
    observers.push_back(obj);
}

void Subject::detach(Observer *obj)
{
    std::lock_guard<std::mutex> lg(m_mutex);
    observers.remove(obj);
}

void Subject::notify(MessageType::MessageType type)
{
    std::lock_guard<std::mutex> lg(m_mutex);

    std::list<Observer *>::iterator iter = observers.begin();
    while(iter != observers.end())
    {
        (*iter)->onMessage(type);
        iter++;
    }
}
