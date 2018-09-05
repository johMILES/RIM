#include "rcomponent.h"

RComponent::RComponent(const char *id, QWidget *parent):
    QDockWidget(parent),m_id(id)
{

}

RComponent::~RComponent()
{

}

Id RComponent::id()
{
    return m_id;
}

QString RComponent::name()
{
    return m_name;
}
