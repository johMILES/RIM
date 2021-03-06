﻿#include "widgets/mainwindow.h"
#include <QApplication>

#include <iostream>

#include <QDebug>
#include <QMessageBox>
#include <QSettings>
#include <QTextCodec>
#include <QDateTime>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDir>
#include <QSqlDriver>
#include <QMessageBox>
#include <QTranslator>

#include "Base/util/rutil.h"
#include "Base/util/rlog.h"
#include "Base/util/rsingleton.h"
#include "thread/recvtextprocessthread.h"
#include "thread/sendtextprocessthread.h"
#include "thread/filesendqueuethread.h"
#include "Base/constants.h"
#include "Base/global.h"
#include "Base/file/xmlparse.h"
#include "Base/others/broadcastnode.h"
#include "Base/file/globalconfigfile.h"

#ifdef Q_OS_WIN
#pragma  comment(lib,"ws2_32.lib")
#endif

#include "Network/abstractserver.h"
#include "Network/win32net/tcpserver.h"
using namespace ServerNetwork;

#include "Base/sql/databasemanager.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <Dbghelp.h>
#pragma comment( lib, "DbgHelp")

inline void CreateMiniDump(PEXCEPTION_POINTERS pep, LPCTSTR strFileName)
{
    HANDLE hFile = CreateFile(strFileName, GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);


    if ((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE))
    {
        MINIDUMP_EXCEPTION_INFORMATION mdei;
        mdei.ThreadId = GetCurrentThreadId();
        mdei.ExceptionPointers = pep;
        mdei.ClientPointers = NULL;

        ::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, MiniDumpWithFullMemory, (pep != 0) ? &mdei : 0, NULL, 0);

        CloseHandle(hFile);
    }
}

LONG __stdcall MyUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
    CreateMiniDump(pExceptionInfo, L"core.dmp");
    return EXCEPTION_EXECUTE_HANDLER;
}

void parseCommandLine(QApplication & app,CommandParameter & result)
{
    QCommandLineOption serviceOption("s",QCoreApplication::translate("main","choose start service"),"","text");
    serviceOption.setValueName("service");

    QCommandLineOption transOption("m",QCoreApplication::translate("main","choose transmission mode"),"","net");
    transOption.setValueName("trans");

    QCommandLineOption databaseOption("d",QCoreApplication::translate("main","choose database type"),"","mysql");
    databaseOption.setValueName("database");

    QCommandLineParser commandParser;
    commandParser.addOption(serviceOption);
    commandParser.addOption(transOption);
    commandParser.addOption(databaseOption);
    const QCommandLineOption helpOption = commandParser.addHelpOption();
    const QCommandLineOption versionOption = commandParser.addVersionOption();

    if (!commandParser.parse(app.arguments()))
    {
        std::cerr<<commandParser.errorText().toLocal8Bit().data()<<std::endl;
        return;
    }

    if(commandParser.isSet(versionOption))
    {
        printf("%s \n%s %s\n",qPrintable(app.organizationName()), qPrintable(app.applicationName()),
                         qPrintable(app.applicationVersion()));
        result.parseResult = VIEW_PROGRAM;
        return;
    }

    if(commandParser.isSet(helpOption))
    {
        qDebug("%s \t\n"
               "用法: %s.exe [-s type][-m type][-d type][-v][-h] \n\n"
               "服务器支持两种信息服务，包括文本传输和文件传输，用户可使用-s设置服务模式。\n"
               "服务器支持两种通信传输信道，包括网络传输和电话线传输，用户可使用-m设置传输信道。\n"
               "服务器支持两种数据存储，包括MySql和Oracle，用户可使用-d设置数据库类型。\n"
               "\n"
               "选项:\n"
               " -s type \t 设置信息服务的类别,可选类型：\n"
               "         \t [1] text 文本传输服务(默认类型)\n"
               "         \t [2] file 文件传输服务\n"
               " -m type \t 设置传输信道,可选类型：\n"
               "         \t [1] net 以太网传输(默认类型)\n"
               "         \t [2] tel 电话信道传输\n"
               " -d type \t 设置数据库类型，可选类型：\n"
               "         \t [1] mysql msyql数据库(默认类型)\n"
               "         \t [2] oracle Oracle数据库\n"
               " -v      \t 显示程序版本信息\n"
               " -h      \t 显示帮助信息\n"
               "示例:\n"
               "1.以太网传输文本信息： %s.exe -s Text -m Net -d mysql"
               "",app.organizationName().toLocal8Bit().data(),app.applicationName().toLocal8Bit().data(),
               app.applicationName().toLocal8Bit().data());
        result.parseResult = VIEW_PROGRAM;
        return;
    }

    commandParser.process(app.arguments());

    //【1】解析服务类型
    if(commandParser.isSet(serviceOption))
    {
        QString stype = commandParser.value(serviceOption);
        if(stype.toLower() == "text")
        {
            result.serviceType = SERVICE_TEXT;
        }
        else if(stype.toLower() == "file")
        {
            result.serviceType = SERVICE_FILE;
        }
        else
        {
            qDebug("服务类型设置错误! ");
            return;
        }
    }

    //【2】解析传输方式
    if(commandParser.isSet(transOption))
    {
        QString stype = commandParser.value(transOption);
        if(stype.toLower() == "net")
        {
            result.transMode = TRANS_NET;
        }
        else if(stype.toLower() == "tel")
        {
            result.transMode = TRANS_TEL;
        }
        else
        {
            qDebug("传输信道设置错误! ");
            return;
        }
    }

    //【3】解析数据库类型
    if(commandParser.isSet(databaseOption))
    {
        QString dtype = commandParser.value(databaseOption);
        if(dtype.toLower() == "mysql")
        {
            result.dbType = DB_MYSQL;
        }
        else if(dtype.toLower() == "tel")
        {
            result.dbType = DB_ORACLE;
        }
        else
        {
            qDebug("数据库设置错误! ");
            return;
        }
    }

    result.parseResult = EXEC_PROGRAM;
}

void printProgramInfo(CommandParameter & result,QString ip,unsigned short port)
{
    QString serviceType;
    switch(result.serviceType)
    {
        case SERVICE_TEXT: serviceType = "Text Service";break;
        case SERVICE_FILE: serviceType = "File Service";break;
        default:break;
    }

    QString transType;
    switch(result.transMode)
    {
        case TRANS_NET: transType = "Net Trans";break;
        case TRANS_TEL: transType = "Tel Trans";break;
        default:break;
    }

    QString dbType;
    switch(result.dbType)
    {
        case DB_MYSQL: dbType = "MySQL";break;
        case DB_ORACLE: dbType = "Oracle";break;
        default:break;
    }

    qDebug(""
            "       ____                            \n"
            "      / __ \___  ____  ____ ___  __    \n"
            "     / /_/ / _ \/ __ \/ __ `/ / / /    \n"
            "    / _, _/  __/ / / / /_/ / /_/ /     \n"
            "   /_/ |_|\___/_/ /_/\__, /\__,_/      \n"
            "                    /____/             \n"
            "                                       \n"
            "[System working type: %s  %s  %s ]     \n"
            "[Listening ip and port: %s %d]         \n"
           ,serviceType.toLocal8Bit().data(),transType.toLocal8Bit().data(),dbType.toLocal8Bit().data(),ip.toLocal8Bit().data(),port);
}

//启动文本发送线程
void startTextSendThread(const SettingConfig & setting)
{
    for(int i = 0; i< setting.textSendProcCount;i++)
    {
        SendTextProcessThread * thread = new SendTextProcessThread;
        thread->start();
    }
}

//启动文件发送线程
void startFileSendThread(const SettingConfig & setting)
{
    for(int i = 0; i< setting.textSendProcCount;i++)
    {
#ifdef __LOCAL_CONTACT__
        FileSendQueueThread * thread = new FileSendQueueThread;
        thread->startMe();
#endif
    }
}

int main(int argc, char *argv[])
{
    SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
    QApplication a(argc, argv);

    QApplication::setApplicationName(Constant::ApplicationName);
    QApplication::setOrganizationDomain(QObject::tr("rengu.com"));
    QApplication::setOrganizationName(QObject::tr("NanJing RenGu"));
    QApplication::setApplicationVersion(Constant::Version);

    CommandParameter commandResult;
    parseCommandLine(a,commandResult);

#if QT_VERSION > 0x050000
    QTextCodec * codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
#endif

    if(commandResult.parseResult == PARSE_ERROR)
    {
        return -1;
    }
    else if(commandResult.parseResult == VIEW_PROGRAM)
    {
        return 0;
    }
    else if(commandResult.parseResult == EXEC_PROGRAM)
    {
        qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());

        QString configFullPath = qApp->applicationDirPath() + QString(Constant::PATH_ConfigPath);

        QSettings * settings = new QSettings(configFullPath+"/config.ini",QSettings::IniFormat);
        RUtil::setGlobalSettings(settings);

        //初始化配置文件解析
        RGlobal::G_GlobalConfigFile = new GlobalConfigFile;
        RGlobal::G_GlobalConfigFile->setSettings(settings);
        if(!RGlobal::G_GlobalConfigFile->parseFile()){
            QMessageBox::warning(NULL,QObject::tr("Warning"),QObject::tr("System INI file parsed false!"),QMessageBox::Yes,QMessageBox::Yes);
            return -1;
        }

        if(!RSingleton<RLog>::instance()->init(RGlobal::G_GlobalConfigFile->logConfig))
        {
            QMessageBox::warning(NULL,QObject::tr("Warning"),QObject::tr("Log module initialization failure!"),QMessageBox::Yes,QMessageBox::Yes);
        }

        Datastruct::SettingConfig localConfig = RGlobal::G_GlobalConfigFile->netSettingConfig;

#ifdef __LOCAL_CONTACT__
        RGlobal::G_RouteSettings = new ParameterSettings::RouteSetting;
        QString localConfigName = configFullPath + QDir::separator() + QStringLiteral("路由表.txt");
        a.setProperty(Constant::LOCAL_ROUTE_CONFIG_FILE,localConfigName);
        if(!RSingleton<XMLParse>::instance()->parseRouteSettings(localConfigName,RGlobal::G_RouteSettings)){
            QMessageBox::warning(NULL,QObject::tr("Warning"),QObject::tr("Route settings read failed,system exit!"),
                                 QMessageBox::Yes,QMessageBox::Yes);
            exit(-1);
        }
#endif
        //语言
        QTranslator translator;

        QString translationPath = configFullPath + QString(Constant::CONFIG_LocalePath);
        if(RUtil::createDir(translationPath))
        {
            QStringList uiLanguages;

    #if (QT_VERSION >= 0x040801) || (QT_VERSION >= 0x040800 && !defined(Q_OS_WIN))
            uiLanguages = QLocale::system().uiLanguages();
    #endif

            foreach(QString locale, uiLanguages)
            {
    #if (QT_VERSION >= 0x050000)
                locale = QLocale(locale).name();
    #else
                locale.replace(QLatin1Char('-'), QLatin1Char('_'));
    #endif
                if(translator.load(QString(Constant::ApplicationName)+"_"+ locale,translationPath))
                {
                    a.installTranslator(&translator);
                    a.setProperty("rimLocale", locale);
                    break;
                }
            }
        }

        //样式
        QFile blackStyleFile(":/resource/style/Black.qss");
        if(!blackStyleFile.open(QFile::ReadOnly)){
            RLOG_ERROR("style file read error!");
            return -1;
        }
        a.setStyleSheet(blackStyleFile.readAll());

        RGlobal::G_SERVICE_TYPE = commandResult.serviceType;

#ifndef __LOCAL_CONTACT__
        if(RGlobal::G_SERVICE_TYPE == SERVICE_FILE)
#endif
        {
            QDir fileDir(RGlobal::G_GlobalConfigFile->fileServerSetting.fileRecvPath);
            if(!fileDir.mkpath(RGlobal::G_GlobalConfigFile->fileServerSetting.fileRecvPath))
            {
                RLOG_ERROR("create file path error ! %s",RGlobal::G_GlobalConfigFile->fileServerSetting.fileRecvPath.toLocal8Bit().data());
                return -1;
            }
        }
        RGlobal::G_FILE_UPLOAD_PATH = RGlobal::G_GlobalConfigFile->fileServerSetting.fileRecvPath;

        RSingleton<DatabaseManager>::instance()->setConnectInfo(RGlobal::G_GlobalConfigFile->databaseConfig);
        RSingleton<DatabaseManager>::instance()->setDatabaseType(commandResult.dbType);

        memset((char *)&RGlobal::G_DB_FEATURE,0,sizeof(Datastruct::DBFeature));
        RGlobal::G_DB_FEATURE.lastInsertId = RSingleton<DatabaseManager>::instance()->hasFeature(QSqlDriver::LastInsertId);

#ifdef DB_TRANSACTIONS_CHECK
        RGlobal::G_DB_FEATURE.transactions = RSingleton<DatabaseManager>::instance()->hasFeature(QSqlDriver::Transactions);
        if(!RGlobal::G_DB_FEATURE.transactions){
            QMessageBox::warning(nullptr,QObject::tr("warning"),QObject::tr("Current database don't support transactions"));
        }
#endif

        //【1】启动文件传输服务器
        if(commandResult.serviceType == SERVICE_TEXT)
        {
            printProgramInfo(commandResult,localConfig.textIp,localConfig.textListenPort);
            AbstractServer * tcpTextServer = new TcpServer();
            tcpTextServer->startMe(localConfig.textIp.toLocal8Bit().data(),localConfig.textListenPort);
        }
        else if(commandResult.serviceType == SERVICE_FILE)
        {
            printProgramInfo(commandResult,localConfig.fileIp,localConfig.fileListenPort);
            AbstractServer * fileServer = new TcpServer();
            fileServer->startMe(localConfig.fileIp.toLocal8Bit().data(),localConfig.fileListenPort);
        }

        //【2】启动文本接收处理线程
        for(int i = 0; i < localConfig.textRecvProcCount;i++)
        {
            RecvTextProcessThread * thread = new RecvTextProcessThread;
            Database * dbs = RSingleton<DatabaseManager>::instance()->newDatabase();
            if(!dbs->isError())
            {
                thread->setDatabase(dbs);
                thread->start();
            }
            else
            {
                RLOG_ERROR(dbs->errorInfo().toLocal8Bit().data());
                return -1;
            }
        }

        //【3】启动文本发送处理线程
        if(commandResult.serviceType == SERVICE_TEXT)
        {
            startTextSendThread(localConfig);
#ifdef __LOCAL_CONTACT__
            startFileSendThread(localConfig);
#endif
        }
        else if(commandResult.serviceType == SERVICE_FILE)
        {
            startFileSendThread(localConfig);
        }

        MainWindow widget;
        widget.show();

        return a.exec();
    }
}
