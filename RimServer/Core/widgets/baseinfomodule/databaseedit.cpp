#include "databaseedit.h"

#include <QListView>
#include <QMessageBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QSpacerItem>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>

#include <climits>

#include "global.h"
#include "sql/databasemanager.h"
#include "../../file/globalconfigfile.h"
#include "Base/selfwidget/rbutton.h"
#include "Util/regexp.h"
#include "rsingleton.h"

class DatabaseEditPrivate : public QObject
{
    Q_DECLARE_PUBLIC(DatabaseEdit)
public:
    explicit DatabaseEditPrivate(DatabaseEdit * q):q_ptr(q){
        initView();
    }

    void initView();

private:
    DatabaseEdit * q_ptr;
    QWidget * mainWidget;

    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QLineEdit *hostName;
    QLineEdit *databaseName;
    QLineEdit *userName;
    QLineEdit *password;
    QLineEdit *port;
    QSpacerItem *verticalSpacer_3;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    RButton *testConnect;
    QComboBox *databaseType;
};

void DatabaseEditPrivate::initView()
{
    mainWidget = new QWidget();
    groupBox_2 = new QGroupBox(mainWidget);
    groupBox_2->setObjectName(QStringLiteral("groupBox_2"));

    gridLayout_2 = new QGridLayout(groupBox_2);
    gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));

    //db type
    databaseType = new QComboBox(groupBox_2);
    databaseType->setObjectName(QStringLiteral("databaseType"));

    gridLayout_2->addWidget(databaseType, 0, 1, 1, 1);

    QLabel * label_8 = new QLabel(groupBox_2);
    label_8->setObjectName(QStringLiteral("label_8"));
    label_8->setMinimumSize(QSize(0, 26));
    label_8->setMaximumSize(QSize(100, 16777215));
    label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_8->setText(tr("Database type:"));

    gridLayout_2->addWidget(label_8, 0, 0, 1, 1);

    //hostname
    QLabel * label_9 = new QLabel(groupBox_2);
    label_9->setObjectName(QStringLiteral("label_9"));
    label_9->setMinimumSize(QSize(0, 26));
    label_9->setMaximumSize(QSize(100, 16777215));
    label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_9->setText(tr("Host name:"));

    gridLayout_2->addWidget(label_9, 1, 0, 1, 1);

    hostName = new QLineEdit(groupBox_2);
    hostName->setObjectName(QStringLiteral("hostName"));
    hostName->setMinimumSize(QSize(0, 26));

    gridLayout_2->addWidget(hostName, 1, 1, 1, 1);

    //db name
    QLabel * label_10 = new QLabel(groupBox_2);
    label_10->setObjectName(QStringLiteral("label_10"));
    label_10->setMinimumSize(QSize(0, 26));
    label_10->setMaximumSize(QSize(100, 16777215));
    label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_10->setText(tr("Database name:"));

    gridLayout_2->addWidget(label_10, 2, 0, 1, 1);

    databaseName = new QLineEdit(groupBox_2);
    databaseName->setObjectName(QStringLiteral("databaseName"));
    databaseName->setMinimumSize(QSize(0, 26));

    gridLayout_2->addWidget(databaseName, 2, 1, 1, 1);

    //username
    QLabel * label_11 = new QLabel(groupBox_2);
    label_11->setObjectName(QStringLiteral("label_11"));
    label_11->setMinimumSize(QSize(0, 26));
    label_11->setMaximumSize(QSize(100, 16777215));
    label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_11->setText(tr("User name:"));

    gridLayout_2->addWidget(label_11, 3, 0, 1, 1);

    userName = new QLineEdit(groupBox_2);
    userName->setObjectName(QStringLiteral("userName"));
    userName->setMinimumSize(QSize(0, 26));

    gridLayout_2->addWidget(userName, 3, 1, 1, 1);

    //password
    QLabel * label_12 = new QLabel(groupBox_2);
    label_12->setObjectName(QStringLiteral("label_12"));
    label_12->setMinimumSize(QSize(0, 26));
    label_12->setMaximumSize(QSize(100, 16777215));
    label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_12->setText(tr("Password:"));

    gridLayout_2->addWidget(label_12, 4, 0, 1, 1);

    password = new QLineEdit(groupBox_2);
    password->setObjectName(QStringLiteral("password"));
    password->setMinimumSize(QSize(0, 26));
    password->setEchoMode(QLineEdit::Password);

    gridLayout_2->addWidget(password, 4, 1, 1, 1);

    //port
    QLabel * label_13 = new QLabel(groupBox_2);
    label_13->setObjectName(QStringLiteral("label_13"));
    label_13->setMinimumSize(QSize(0, 26));
    label_13->setMaximumSize(QSize(100, 16777215));
    label_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_13->setText(tr("Port:"));

    gridLayout_2->addWidget(label_13, 5, 0, 1, 1);

    port = new QLineEdit(groupBox_2);
    port->setObjectName(QStringLiteral("port"));
    port->setMinimumSize(QSize(0, 26));

    gridLayout_2->addWidget(port, 5, 1, 1, 1);

    verticalSpacer_3 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout_2->addItem(verticalSpacer_3, 7, 0, 1, 1);

    widget_2 = new QWidget(groupBox_2);
    widget_2->setObjectName(QStringLiteral("widget_2"));
    horizontalLayout = new QHBoxLayout(widget_2);
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    horizontalLayout->setContentsMargins(1, 1, 1, 1);
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);

    testConnect = new RButton(widget_2);
    testConnect->setObjectName(QStringLiteral("testConnect"));
    testConnect->setText(tr("Test connection"));
    connect(testConnect,SIGNAL(pressed()),q_ptr,SLOT(testDatabaseConnect()));

    horizontalLayout->addWidget(testConnect);

    gridLayout_2->addWidget(widget_2, 6, 1, 1, 1);

    groupBox_2->setLayout(gridLayout_2);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(groupBox_2);
    mainWidget->setLayout(layout);

    databaseType->setView(new QListView());

    QStringList supportDbs = DatabaseManager::getSupportedDatabase();
    foreach(QString db,supportDbs){
        databaseType->addItem(db);
    }

    hostName->setText(RGlobal::G_GlobalConfigFile->databaseConfig.hostName);
    databaseName->setText(RGlobal::G_GlobalConfigFile->databaseConfig.dbName);
    userName->setText(RGlobal::G_GlobalConfigFile->databaseConfig.dbUser);
    password->setText(RGlobal::G_GlobalConfigFile->databaseConfig.dbPass);
    port->setText(QString::number(RGlobal::G_GlobalConfigFile->databaseConfig.port));
}

DatabaseEdit::DatabaseEdit(QWidget *parent) :
    DialogProxy(parent),d_ptr(new DatabaseEditPrivate(this))
{
    setButton(DialogProxy::Save|DialogProxy::Cancel);
    setContentWidget(d_ptr->mainWidget);
    setWindowTitle(tr("Database info Setting"));
}

DatabaseEdit::~DatabaseEdit()
{

}

QSize DatabaseEdit::sizeHint() const
{
    return QSize(375,356);
}

void DatabaseEdit::respButtClicked(DialogProxy::StandardButton butt)
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

void DatabaseEdit::cancel()
{
    close();
}

void DatabaseEdit::confirm()
{
    if(checkInputValid()){
        Datastruct::DatabaseConfigInfo dbConfig;
        getDatabaseSetting(&dbConfig);

        if(RGlobal::G_GlobalConfigFile->saveDatabaseConfig(dbConfig)){
            QMessageBox::information(this,tr("information"),tr("Save successfully! Modifications will take effect after reboot."));
            close();
            return;
        }

        QMessageBox::warning(this,tr("warning"),tr("Save failed!"));
    }
}

/*!
 * @brief 测试数据库是否连接
 */
void DatabaseEdit::testDatabaseConnect()
{
    if(checkInputValid()){
        Datastruct::DatabaseConfigInfo dbConfig;
        getDatabaseSetting(&dbConfig);
        if(DatabaseManager::testConnection(dbConfig))
            QMessageBox::information(this,tr("information"),tr("Connect database successfully!"));
        else
            QMessageBox::information(this,tr("information"),tr("Connect database failed!"));
    }
}

bool DatabaseEdit::checkInputValid()
{
    Q_D(DatabaseEdit);
    if(!validateText(this,d->hostName->text(),tr("Host name")))
        return false;

    if(!validateText(this,d->databaseName->text(),tr("Database name")))
        return false;

    if(!validateText(this,d->userName->text(),tr("User name")))
        return false;

    if(!validateText(this,d->password->text(),tr("Password")))
        return false;

    unsigned short port = d->port->text().toUShort();
    if(port <= 0 || port > USHRT_MAX ){
        QMessageBox::warning(this,tr("warning"),tr("Port doesn't valid!"));
        return false;
    }

    return true;
}

bool DatabaseEdit::validateText(QWidget *parent, QString &text, QString &uiName)
{
    if(text.size() == 0){
        QMessageBox::warning(parent,tr("warning"),tr("%1 content is empty!").arg(uiName));
        return false;
    }

    if(RSingleton<RegExp>::instance()->getValidate(RegExp::FULL_SPACE,text)){
        QMessageBox::warning(parent,tr("warning"),tr("%1 content contains space!").arg(uiName));
        return false;
    }

    return true;
}

void DatabaseEdit::getDatabaseSetting(DatabaseConfigInfo * dbConfig)
{
    Q_D(DatabaseEdit);
    dbConfig->dbName = d->databaseName->text();
    dbConfig->dbUser = d->userName->text();
    dbConfig->dbPass = d->password->text();
    dbConfig->hostName = d->hostName->text();
    dbConfig->port = d->port->text().toUShort();
    dbConfig->dbType = DatabaseManager::getDatabaseType(d->databaseType->currentText());
}
