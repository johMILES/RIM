#include "messdispatch.h"

MessDispatch::MessDispatch(QObject *parent) : QObject(parent)
{

}

void MessDispatch::onRecvRealRecord(QString content)
{
    emit realRecord(content);
}
