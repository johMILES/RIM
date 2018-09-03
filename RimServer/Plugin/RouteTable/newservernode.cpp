#include "newservernode.h"

#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QComboBox>
#include <QListView>
#include <QDebug>

#include "Base/selfwidget/rbutton.h"
#include "Base/protocol/datastruct.h"
#include "Base/util/scaleswitcher.h"

class NewServerNodePrivate : public QObject
{
    Q_DECLARE_PUBLIC(NewServerNode)
public:
    NewServerNodePrivate(NewServerNode * q):q_ptr(q){
        initView();
    }

    void initView();

private:
    NewServerNode * q_ptr;

    QWidget * mainWidget;

    QLineEdit * nodeEdit;
    QLineEdit * ipEdit;
    QLineEdit * portEdit;
    QComboBox * method;
    QComboBox * format;

    RButton * tmpButt;

};

void NewServerNodePrivate::initView()
{
    mainWidget = new QWidget();

    QGridLayout * grid = new QGridLayout;

    //node
    QLabel * nodeLabel = new QLabel;
    nodeLabel->setText(QObject::tr("Node id"));

    nodeEdit = new QLineEdit;

    //ip
    QLabel * ipLabel = new QLabel;
    ipLabel->setText(QObject::tr("Ip address"));

    ipEdit = new QLineEdit;

    //port
    QLabel * portLabel = new QLabel;
    portLabel->setText(QObject::tr("Port"));

    portEdit = new QLineEdit;

    //method
    QLabel * methodLabel = new QLabel;
    methodLabel->setText(QObject::tr("Method"));

    method = new QComboBox;
    method->setView(new QListView());
    method->addItem("0");

    //messageformat
    QLabel * formatLabel = new QLabel;
    formatLabel->setText(QObject::tr("Format"));

    format = new QComboBox;
    format->setView(new QListView());
    format->addItem("0");

    grid->addWidget(nodeLabel,0,0,1,1);
    grid->addWidget(nodeEdit,0,1,1,1);

    grid->addWidget(ipLabel,1,0,1,1);
    grid->addWidget(ipEdit,1,1,1,1);

    grid->addWidget(portLabel,2,0,1,1);
    grid->addWidget(portEdit,2,1,1,1);

    grid->addWidget(methodLabel,3,0,1,1);
    grid->addWidget(method,3,1,1,1);

    grid->addWidget(formatLabel,4,0,1,1);
    grid->addWidget(format,4,1,1,1);

    grid->setRowStretch(5,1);

    mainWidget->setLayout(grid);
}

NewServerNode::NewServerNode(QWidget *parent):DialogProxy(parent)
  ,d_ptr(new NewServerNodePrivate(this))
{
    setButton(DialogProxy::Cancel | DialogProxy::Save);
    setContentWidget(d_ptr->mainWidget);

    setWindowTitle(tr("New server node"));
}

NewServerNode::~NewServerNode()
{

}

QSize NewServerNode::sizeHint() const
{
    return QSize(250,300);
}

void NewServerNode::respButtClicked(DialogProxy::StandardButton butt)
{
    Q_D(NewServerNode);
    switch(butt){
        case DialogProxy::Save:
            {
                if(validateInput()){
                    ParameterSettings::NodeServer * server = new ParameterSettings::NodeServer;
                    server->nodeId = ScaleSwitcher::fromHexToDec(d->nodeEdit->text());
                    server->localIp = d->ipEdit->text();
                    server->localPort = d->portEdit->text().toUShort();
                    emit newServerNodeInfo(server);
                    close();
                }else{
                    QMessageBox::warning(this,tr("warning"),tr("Input invalid!"));
                }
            }
            break;
        case DialogProxy::Cancel:
            close();
            break;
        default:
            break;
    }
}

bool NewServerNode::validateInput()
{
    Q_D(NewServerNode);



    return true;
}
