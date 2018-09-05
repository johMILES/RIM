#include "operetepanel.h"

#include "../../constants.h"
#include "operatewidget.h"

OperetePanel::OperetePanel(QWidget * parent):
    RComponent(Constant::PLUGIN_OPERATE,parent)
{
    m_name = tr("Operate panel");
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
}

OperetePanel::~OperetePanel()
{

}

bool OperetePanel::initialize()
{
    OperateWidget * widget = new OperateWidget();
    setWidget(widget);
    return true;
}

void OperetePanel::release()
{

}

void OperetePanel::onMessage(MessageType::MessageType type)
{

}

QSize OperetePanel::sizeHint() const
{
    return QSize(250,200);
}
