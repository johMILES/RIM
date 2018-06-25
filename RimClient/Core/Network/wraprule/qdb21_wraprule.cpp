﻿#include "qdb21_wraprule.h"
#include <QTime>

#ifdef __LOCAL_CONTACT__
#include "localprotocoldata.h"
using namespace QDB21;

QDB21_WrapRule::QDB21_WrapRule():WrapRule()
{

}

QByteArray QDB21_WrapRule::wrap(const ProtocolPackage &package)
{
    QDB21_Head qdb21_Head;
    memset(&qdb21_Head,0,sizeof(QDB21_Head));
    qdb21_Head.cTypeNum =1;
    qdb21_Head.ulPackageLen = sizeof(QDB21_Head) + package.cFileData.size();
    qdb21_Head.usOrderNo = 2051;

    QByteArray wrap;
    wrap.append((char*)&qdb21_Head,sizeof(QDB21_Head));
    wrap.append(package.cFileData);
    return wrap;
}

QByteArray QDB21_WrapRule::unwrap(const QByteArray &data)
{
    QDB21_Head qdb21_Head;
    memset(&qdb21_Head,0,sizeof(QDB21_Head));
    memcpy(&qdb21_Head,data.data(),sizeof(QDB21_Head));

    QByteArray tempdata = data.right(data.size() - QDB21_Head_Length);

    return tempdata;
}

#endif