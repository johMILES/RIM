#include "newclientnode.h"

#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QListView>
#include <QMessageBox>

#include "Base/protocol/datastruct.h"
#include "Base/selfwidget/rbutton.h"
#include "Base/util/scaleswitcher.h"

class NewClientNodePrivate : public QObject
{
    Q_DECLARE_PUBLIC(NewClientNode)
public:
    NewClientNodePrivate(NewClientNode * q):q_ptr(q){
        initView();
    }

    void initView();

private:
    NewClientNode * q_ptr;

    QWidget * mainWidget;

    QLineEdit * nodeEdit;
    QLineEdit * commEdit;
    QComboBox * format;
    QComboBox * channel;
    QComboBox * dist;

    RButton * tmpButt;

};

void NewClientNodePrivate::initView()
{
    mainWidget = new QWidget();

    QGridLayout * grid = new QGridLayout;

    //node
    QLabel * nodeLabel = new QLabel;
    nodeLabel->setText(QObject::tr("Node id"));

    nodeEdit = new QLineEdit;

    //ip
    QLabel * commLabel = new QLabel;
    commLabel->setText(QObject::tr("Communication method"));

    commEdit = new QLineEdit;

    //format
    QLabel * formatLabel = new QLabel;
    formatLabel->setText(QObject::tr("Message format"));

    format = new QComboBox;
    format->setView(new QListView());
    format->addItem("0");

    //channel
    QLabel * channelLabel = new QLabel;
    channelLabel->setText(QObject::tr("Channel"));

    channel = new QComboBox;
    channel->setView(new QListView());
    channel->addItem("0");

    //dist
    QLabel * distLabel = new QLabel;
    distLabel->setText(QObject::tr("Distribute message type"));

    dist = new QComboBox;
    dist->setView(new QListView());
    dist->addItem("0");

    grid->addWidget(nodeLabel,0,0,1,1);
    grid->addWidget(nodeEdit,0,1,1,1);

    grid->addWidget(commLabel,1,0,1,1);
    grid->addWidget(commEdit,1,1,1,1);

    grid->addWidget(formatLabel,2,0,1,1);
    grid->addWidget(format,2,1,1,1);

    grid->addWidget(channelLabel,3,0,1,1);
    grid->addWidget(channel,3,1,1,1);

    grid->addWidget(distLabel,4,0,1,1);
    grid->addWidget(dist,4,1,1,1);

    grid->setRowStretch(5,1);

    mainWidget->setLayout(grid);
}

NewClientNode::NewClientNode(QWidget *parent):DialogProxy(parent),
    d_ptr(new NewClientNodePrivate(this))
{
    setButton(DialogProxy::Cancel | DialogProxy::Save);
    setContentWidget(d_ptr->mainWidget);

    setWindowTitle(tr("New client node"));
}

NewClientNode::~NewClientNode()
{

}

QSize NewClientNode::sizeHint() const
{
    return QSize(250,300);
}

bool NewClientNode::validateInput()
{
    return true;
}

void NewClientNode::respButtClicked(DialogProxy::StandardButton butt)
{
    Q_D(NewClientNode);
    switch(butt){
        case DialogProxy::Save:
            {
                if(validateInput()){

                    ParameterSettings::NodeClient * client = new ParameterSettings::NodeClient;
                    client->nodeId = ScaleSwitcher::fromHexToDec(d->nodeEdit->text());

                    emit newClientNodeInfo(client);

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
