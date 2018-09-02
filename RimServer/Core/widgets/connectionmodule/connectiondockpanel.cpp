#include "connectiondockpanel.h"

#include "../../constants.h"
#include "connectionwidget.h"

#include <QDebug>
#include <QResizeEvent>

ConnectionPanel::ConnectionPanel(QWidget * parent):
    RComponent(Constant::PLUGIN_CONNECTION,parent)
{
    m_name = tr("Connection panel");
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

ConnectionPanel::~ConnectionPanel()
{

}

bool ConnectionPanel::initialize()
{
    ConnectionWidget * widget = new ConnectionWidget;
    setWidget(widget);
    return true;
}

void ConnectionPanel::release()
{

}

void ConnectionPanel::onMessage(MessageType::MessageType type)
{

}

void ConnectionPanel::resizeEvent(QResizeEvent *event)
{

}
