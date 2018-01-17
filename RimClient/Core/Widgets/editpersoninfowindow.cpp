﻿#include "editpersoninfowindow.h"

#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QDateEdit>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QGridLayout>
#include <QFileDialog>

#include "head.h"
#include "constants.h"
#include "datastruct.h"
#include "rsingleton.h"
#include "Util/imagemanager.h"
#include "toolbar.h"
#include "Util/rutil.h"
#include "widget/rbutton.h"
#include "widget/riconlabel.h"

#define EDIT_PERSON_WIDTH 380
#define EDIT_PERSON_HEIGHT 600

class EditPersonInfoWindowPrivate : public GlobalData<EditPersonInfoWindow>
{
    Q_DECLARE_PUBLIC(EditPersonInfoWindow)

private:
    EditPersonInfoWindowPrivate(EditPersonInfoWindow * q):q_ptr(q)
    {
        initWidget();
    }

    void initWidget();

    EditPersonInfoWindow * q_ptr;

    QWidget * contentWidget;

    RIconLabel * iconLabel;

    QLabel * m_account_label;
    QLineEdit * m_account_edit;

    QLabel * m_nickName_label;
    QLineEdit * m_nickName_edit;

    QLabel * m_sexual_label;
    QComboBox * m_sexual_box;

    QLabel * m_birth_label;
    QDateEdit * m_birth_dateEdit;

    QLabel * m_sign_label;
    QPlainTextEdit * m_sign_plainEdit;

    QLabel * m_address_label;
    QLineEdit * m_address_edit;

    QLabel * m_phone_label;
    QLineEdit * m_phone_edit;

    QLabel * m_mail_label;
    QLineEdit * m_mail_edit;

    QLabel * m_desc_label;
    QPlainTextEdit * m_desc_plainEdit;

    RButton * saveButton;
    RButton * closeButton;

    RButton * iconButton;
};

void EditPersonInfoWindowPrivate::initWidget()
{
    contentWidget = new QWidget;
    contentWidget->setObjectName("Widget_ContentWidget");

    const int Label_Width = 60;
    const int Label_Height = 20;
    const int Edit_Width = 110;

    QWidget * bodyWidget = new QWidget(contentWidget);

    m_account_label = new QLabel(bodyWidget);
    m_account_label->setAlignment(Qt::AlignCenter);
    m_account_label->setFixedSize(Label_Width,Label_Height);
    m_account_label->setText(QObject::tr("Account"));

    m_account_edit = new QLineEdit(bodyWidget);
    m_account_edit->setReadOnly(true);
    m_account_edit->setFixedSize(Edit_Width,Label_Height);

    m_nickName_label = new QLabel(bodyWidget);
    m_nickName_label->setAlignment(Qt::AlignCenter);
    m_nickName_label->setText(QObject::tr("Nick Name"));

    QWidget * iconWidget = new QWidget(bodyWidget);

    iconLabel = new RIconLabel(iconWidget);
    iconLabel->setToolTip(QObject::tr("Open system image"));
    iconLabel->setTransparency(true);
    iconLabel->setPixmap(RSingleton<ImageManager>::instance()->getSystemUserIcon());
    QObject::connect(iconLabel,SIGNAL(mousePressed()),q_ptr,SLOT(openSystemImage()));

    iconButton = new RButton(iconWidget);
    iconButton->setText(QObject::tr("Local Image"));
    QObject::connect(iconButton,SIGNAL(pressed()),q_ptr,SLOT(openLocalImage()));

    QVBoxLayout * iconVLayout = new QVBoxLayout;
    iconVLayout->setContentsMargins(0,0,0,0);
    iconVLayout->setSpacing(0);

    iconVLayout->addWidget(iconLabel);
    iconVLayout->addWidget(iconButton);

    QHBoxLayout * iconHLayout = new QHBoxLayout;
    iconHLayout->setContentsMargins(0,0,0,0);
    iconHLayout->setSpacing(0);

    iconHLayout->addStretch(1);
    iconHLayout->addLayout(iconVLayout);
    iconHLayout->addStretch(1);

    iconWidget->setLayout(iconHLayout);

    m_nickName_edit = new QLineEdit(bodyWidget);
    m_nickName_edit->setFixedSize(Edit_Width,Label_Height);

    m_sexual_label = new QLabel(bodyWidget);
    m_sexual_label->setAlignment(Qt::AlignCenter);
    m_sexual_label->setText(QObject::tr("Sex"));

    m_sexual_box = new QComboBox(bodyWidget);
    m_sexual_box->addItem(QObject::tr("Man"));
    m_sexual_box->addItem(QObject::tr("Woman"));

    m_sexual_box->setFixedSize(Edit_Width,Label_Height);

    m_birth_label = new QLabel(bodyWidget);
    m_birth_label->setAlignment(Qt::AlignCenter);
    m_birth_label->setText(QObject::tr("Birthday"));

    m_birth_dateEdit = new QDateEdit(bodyWidget);
    m_birth_dateEdit->setFixedSize(Edit_Width,Label_Height);

    m_sign_label = new QLabel(bodyWidget);
    m_sign_label->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    m_sign_label->setText(QObject::tr("Sign Name"));

    m_sign_plainEdit = new QPlainTextEdit(bodyWidget);
    m_sign_plainEdit->setFixedHeight(60);

    m_address_label = new QLabel(bodyWidget);
    m_address_label->setAlignment(Qt::AlignCenter);
    m_address_label->setText(QObject::tr("Address"));

    m_address_edit = new QLineEdit(bodyWidget);
    m_address_edit->setFixedHeight(Constant::ITEM_FIX_HEIGHT);

    m_phone_label = new QLabel(bodyWidget);
    m_phone_label->setAlignment(Qt::AlignCenter);
    m_phone_label->setText(QObject::tr("Phone Number"));

    m_phone_edit = new QLineEdit(bodyWidget);
    m_phone_edit->setFixedHeight(Constant::ITEM_FIX_HEIGHT);

    m_mail_label = new QLabel(bodyWidget);
    m_mail_label->setAlignment(Qt::AlignCenter);
    m_mail_label->setText(QObject::tr("Email"));

    m_mail_edit = new QLineEdit(bodyWidget);
    m_mail_edit->setFixedHeight(Constant::ITEM_FIX_HEIGHT);

    m_desc_label = new QLabel(bodyWidget);
    m_desc_label->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    m_desc_label->setText(QObject::tr("Desc"));

    m_desc_plainEdit = new QPlainTextEdit(bodyWidget);
    m_desc_plainEdit->setFixedHeight(60);

    QGridLayout * gridLayout = new QGridLayout();

    gridLayout->addWidget(m_account_label,0,0,1,1);             //账号
    gridLayout->addWidget(m_account_edit,0,1,1,1);

    gridLayout->addWidget(m_nickName_label,1,0,1,1);            //昵称
    gridLayout->addWidget(m_nickName_edit,1,1,1,1);

    gridLayout->addWidget(iconWidget,0,2,4,2);                  //头像

    gridLayout->addWidget(m_sexual_label,2,0,1,1);              //性别
    gridLayout->addWidget(m_sexual_box,2,1,1,1);

    gridLayout->addWidget(m_birth_label,3,0,1,1);               //生日
    gridLayout->addWidget(m_birth_dateEdit,3,1,1,1);

    gridLayout->addWidget(m_sign_label,4,0,2,1);                //签名
    gridLayout->addWidget(m_sign_plainEdit,4,1,2,3);

    gridLayout->addWidget(m_phone_label,6,0,1,1);               //手机
    gridLayout->addWidget(m_phone_edit,6,1,1,3);

    gridLayout->addWidget(m_address_label,7,0,1,1);             //地址
    gridLayout->addWidget(m_address_edit,7,1,1,3);

    gridLayout->addWidget(m_mail_label,8,0,1,1);                //邮箱
    gridLayout->addWidget(m_mail_edit,8,1,1,3);

    gridLayout->addWidget(m_desc_label,9,0,2,1);                //个人描述
    gridLayout->addWidget(m_desc_plainEdit,9,1,2,3);

    gridLayout->setRowStretch(11,1);

    gridLayout->setVerticalSpacing(10);
    gridLayout->setContentsMargins(10,10,20,0);

    bodyWidget->setLayout(gridLayout);

    QWidget * bottomWidget = new QWidget(contentWidget);

    saveButton = new RButton();
    saveButton->setText(QObject::tr("Save"));

    closeButton = new RButton();
    closeButton->setText(QObject::tr("Close"));

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(0,0,10,10);
    layout->addStretch(1);
    layout->addWidget(saveButton);
    layout->addWidget(closeButton);

    bottomWidget->setLayout(layout);

    QVBoxLayout * contentLayout = new QVBoxLayout;
    contentLayout->setContentsMargins(0,0,0,0);
    contentLayout->addWidget(bodyWidget);
    contentLayout->addWidget(bottomWidget);
    contentWidget->setLayout(contentLayout);

    q_ptr->setContentWidget(contentWidget);
}

EditPersonInfoWindow::EditPersonInfoWindow(QWidget *parent):
    d_ptr(new EditPersonInfoWindowPrivate(this)),
    Widget(parent)
{
    setWindowFlags(windowFlags() & ~Qt::Tool);
    setAttribute(Qt::WA_DeleteOnClose,true);
    setWindowTitle(tr("Edit personal information"));
    setWindowIcon(QIcon(RSingleton<ImageManager>::instance()->getWindowIcon(ImageManager::NORMAL)));

    ToolBar * bar = enableToolBar(true);
    if(bar)
    {
        bar->setToolFlags(ToolBar::TOOL_DIALOG);
        bar->setWindowIcon(RSingleton<ImageManager>::instance()->getWindowIcon(ImageManager::WHITE,ImageManager::ICON_SYSTEM_16));

        bar->setWindowTitle(tr("Edit personal information"));
    }

    RSingleton<Subject>::instance()->attach(this);

    setFixedSize(EDIT_PERSON_WIDTH,EDIT_PERSON_HEIGHT);
    QSize  screenSize = RUtil::screenSize();
    setGeometry((screenSize.width() - EDIT_PERSON_WIDTH)/2,(screenSize.height() - EDIT_PERSON_HEIGHT)/2,EDIT_PERSON_WIDTH,EDIT_PERSON_HEIGHT);
}

EditPersonInfoWindow::~EditPersonInfoWindow()
{

}

void EditPersonInfoWindow::onMessage(MessageType)
{

}

void EditPersonInfoWindow::openSystemImage()
{
    SystemUserImageWindow * systemImage = new SystemUserImageWindow();
    connect(systemImage,SIGNAL(selectedFileBaseName(QString)),this,SLOT(updateSystemIconInfo(QString)));
    systemImage->show();
}

void EditPersonInfoWindow::openLocalImage()
{
    QString imageFile = QFileDialog::getOpenFileName(this,tr("Local image"),"/home", tr("Image Files (*.png)"));
    if(!imageFile.isNull())
    {

    }
}

void EditPersonInfoWindow::updateSystemIconInfo(QString filename)
{
    MQ_D(EditPersonInfoWindow);
    d->iconLabel->setPixmap(RSingleton<ImageManager>::instance()->getSystemImageDir()+"/"+filename);
}

