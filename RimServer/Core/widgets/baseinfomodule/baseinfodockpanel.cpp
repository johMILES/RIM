#include "baseinfodockpanel.h"

#include "../../constants.h"
#include "baseinfowidget.h"

BaseInfoDockPanel::BaseInfoDockPanel(QWidget *parent):
    RComponent(Constant::PLUGIN_BASEINFO,parent),infoWidget(nullptr)
{
    m_name = tr("Baseinfo panel");
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
}

BaseInfoDockPanel::~BaseInfoDockPanel()
{

}

bool BaseInfoDockPanel::initialize()
{
    infoWidget = new BaseInfoWidget();
    setWidget(infoWidget);

    return true;
}

void BaseInfoDockPanel::release()
{

}

void BaseInfoDockPanel::onMessage(Datastruct::MessageType type)
{
    switch(type){
        case Datastruct::MESS_BASEINFO_READY:
            infoWidget->updateBaseInfo();
        break;

        case Datastruct::MESS_DATABASEINFO_READY:
            infoWidget->updateDatabaseInfo();
        break;

        default:
            break;
    }
}

QSize BaseInfoDockPanel::sizeHint() const
{
    return QSize(350,200);
}
