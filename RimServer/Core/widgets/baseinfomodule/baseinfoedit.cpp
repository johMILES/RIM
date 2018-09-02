#include "baseinfoedit.h"

#include <QNetworkInterface>
#include <QList>
#include <QString>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QListView>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QGroupBox>
#include <QComboBox>

#include <climits>

#include "global.h"
#include "file/globalconfigfile.h"
#include "Base/selfwidget/rbutton.h"

class BaseInfoEditPrivate : public QObject
{
    Q_DECLARE_PUBLIC(BaseInfoEdit)
public:
    explicit BaseInfoEditPrivate(BaseInfoEdit * q):q_ptr(q){
        initView();
    }

    void initView();

private:
    BaseInfoEdit * q_ptr;

    QWidget * mainWidget;

    QGroupBox *groupBox;
    QLineEdit *listenPort;
    QSpacerItem *verticalSpacer;
    QLineEdit *fileRecvSavePath;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    RButton *chooseFloder;
    QComboBox *listenIp;
};

void BaseInfoEditPrivate::initView()
{
    mainWidget = new QWidget();

    groupBox = new QGroupBox(mainWidget);
    groupBox->setObjectName(QStringLiteral("groupBox"));

    QGridLayout * gridLayout = new QGridLayout(groupBox);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    gridLayout->setHorizontalSpacing(4);
    gridLayout->setContentsMargins(4, 4, 4, 4);

    //IP
    QLabel * label_2 = new QLabel(groupBox);
    label_2->setObjectName(QStringLiteral("label_2"));
    label_2->setMinimumSize(QSize(0, 26));
    label_2->setMaximumSize(QSize(100, 16777215));
    label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_2->setText(tr("Listen ip:"));

    gridLayout->addWidget(label_2, 0, 0, 1, 1);

    listenIp = new QComboBox(groupBox);
    listenIp->setObjectName(QStringLiteral("listenIp"));

    gridLayout->addWidget(listenIp, 0, 1, 1, 1);

    //port
    QLabel * label_3 = new QLabel(groupBox);
    label_3->setObjectName(QStringLiteral("label_3"));
    label_3->setMinimumSize(QSize(0, 26));
    label_3->setMaximumSize(QSize(100, 16777215));
    label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_3->setText(tr("Listen port:"));

    gridLayout->addWidget(label_3, 1, 0, 1, 1);

    listenPort = new QLineEdit(groupBox);
    listenPort->setObjectName(QStringLiteral("listenPort"));
    listenPort->setMinimumSize(QSize(0, 26));

    gridLayout->addWidget(listenPort, 1, 1, 1, 1);

    verticalSpacer = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout->addItem(verticalSpacer, 4, 0, 1, 1);

    //file path
    QLabel * label_5 = new QLabel(groupBox);
    label_5->setObjectName(QStringLiteral("label_5"));
    label_5->setMinimumSize(QSize(0, 26));
    label_5->setMaximumSize(QSize(100, 16777215));
    label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_5->setText(tr("File save path:"));

    gridLayout->addWidget(label_5, 2, 0, 1, 1);

    fileRecvSavePath = new QLineEdit(groupBox);
    fileRecvSavePath->setObjectName(QStringLiteral("fileRecvSavePath"));
    fileRecvSavePath->setMinimumSize(QSize(0, 26));

    gridLayout->addWidget(fileRecvSavePath, 2, 1, 1, 1);

    widget_2 = new QWidget(groupBox);
    widget_2->setObjectName(QStringLiteral("widget_2"));
    horizontalLayout = new QHBoxLayout(widget_2);
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);

    //choose floder
    chooseFloder = new RButton(widget_2);
    chooseFloder->setObjectName(QStringLiteral("chooseFloder"));
    chooseFloder->setText(tr("Choose floder"));

    horizontalLayout->addWidget(chooseFloder);

    connect(chooseFloder,SIGNAL(pressed()),q_ptr,SLOT(chooseFloder()));

    gridLayout->addWidget(widget_2, 3, 1, 1, 1);

    groupBox->setLayout(gridLayout);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(groupBox);
    mainWidget->setLayout(layout);

    listenIp->setView(new QListView());

    int currIpIndex = 0;
    bool existCurrIp = false;
    listenIp->addItem(QStringLiteral("请选择监听的ip地址:"));
    QList<QHostAddress> addrList = QNetworkInterface::allAddresses();
    std::for_each(addrList.begin(),addrList.end(),[&](const QHostAddress & addr){
        if(addr.protocol() == QAbstractSocket::IPv4Protocol){
            listenIp->addItem(addr.toString());
            if(addr.toString() == RGlobal::G_GlobalConfigFile->netSettingConfig.textIp){
                existCurrIp = true;
            }

            if(!existCurrIp){
                currIpIndex++;
            }
        }
    });

    if(existCurrIp)
        listenIp->setCurrentIndex(currIpIndex + 1);
    else
        listenIp->setCurrentIndex(0);

    listenPort->setText(QString::number(RGlobal::G_GlobalConfigFile->netSettingConfig.textListenPort));
    fileRecvSavePath->setText(RGlobal::G_GlobalConfigFile->fileServerSetting.fileRecvPath);
}

BaseInfoEdit::BaseInfoEdit(QWidget *parent) :
    DialogProxy(parent),d_ptr(new BaseInfoEditPrivate(this))
{
    setWindowTitle(tr("Baseinfo Setting"));

    setButton(DialogProxy::Cancel|DialogProxy::Save);
    setContentWidget(d_ptr->mainWidget);
}

BaseInfoEdit::~BaseInfoEdit()
{
}

QSize BaseInfoEdit::sizeHint() const
{
    return QSize(340,310);
}

void BaseInfoEdit::respButtClicked(DialogProxy::StandardButton butt)
{
    switch(butt)
    {
        case DialogProxy::Save:
            confirm();
            break;
        case DialogProxy::Cancel:
            cancel();
            break;
        default:
            break;
    }
}

/*!
 * @brief 校验用户信息输入
 * @return 返回校验结果，校验成功返回true，否则返回false
 */
bool BaseInfoEdit::checkInputValid()
{
    Q_D(BaseInfoEdit);
    if(d->listenIp->currentIndex() == 0){
        QMessageBox::warning(this,tr("warning"),tr("Please select ip!"));
        return false;
    }

    int listenPort = d->listenPort->text().toInt();
    if(listenPort <= 0 || listenPort > USHRT_MAX){
        QMessageBox::warning(this,tr("warning"),tr("Port doesn't valid!"));
        return false;
    }

    if(!QDir(d->fileRecvSavePath->text()).exists()){
        QMessageBox::warning(this,tr("warning"),tr("File path doesn't exist!"));
        return false;
    }

    return true;
}

void BaseInfoEdit::cancel()
{
    close();
}

void BaseInfoEdit::confirm()
{
    Q_D(BaseInfoEdit);
    if(!checkInputValid())
        return;

    Datastruct::SettingConfig netConfig;
    netConfig.textIp = d->listenIp->currentText();
    netConfig.textListenPort = d->listenPort->text().toUShort();

    Datastruct::FileServerSetting fileConfig;
    fileConfig.fileRecvPath = d->fileRecvSavePath->text();

    if(RGlobal::G_GlobalConfigFile->saveSettingConfig(netConfig) && RGlobal::G_GlobalConfigFile->saveFileServerConfig(fileConfig)){
        QMessageBox::information(this,tr("information"),tr("Save successfully! Modifications will take effect after reboot."));
        close();
        return;
    }

    QMessageBox::warning(this,tr("warning"),tr("Save failed!"));
}

void BaseInfoEdit::chooseFloder()
{
    Q_D(BaseInfoEdit);
     QString saveFilePath = QFileDialog::getExistingDirectory(this,tr("Choose save file path"),"");
     if(!saveFilePath.isEmpty()){
        d->fileRecvSavePath->setText(saveFilePath);
     }
}
