#-------------------------------------------------
#
# Project created by QtCreator 2018-01-08T17:23:21
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += xml
QT       += network

#CONFIG+= console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RimServer
TEMPLATE = app

#读取本地联系人列表，为了解决并需求变动带来的冲突问题
DEFINES += __LOCAL_CONTACT__

RC_ICONS += $${TARGET}.ico

CONFIG(debug, debug|release) {
#  TARGET = $$join(TARGET,,,d)           #为debug版本生成的文件增加d的后缀

  contains(TEMPLATE, "lib") {
    DESTDIR = ../Lib
    DLLDESTDIR = ../Bin
  } else {
    DESTDIR = ../Bin
  }
} else {
  contains(TEMPLATE, "lib") {
    DESTDIR = ../Lib
    DLLDESTDIR = ../Bin
  } else {
    DESTDIR = ../Bin
  }
}

SOURCES += main.cpp\
    sql/sqlprocess.cpp \
    thread/recvtextprocessthread.cpp \
    thread/sendtextprocessthread.cpp \
    jsonkey.cpp \
    Network/msgwrap/binary_wrapformat.cpp \
    Network/msgwrap/json_wrapformat.cpp \
    Network/msgwrap/msgwrap.cpp \
    Network/msgparse/dataparse.cpp \
    Network/msgparse/json_msgparse.cpp \
    Network/msgparse/msgparsefactory.cpp \
    Network/msgprocess/dataprocess.cpp \
    Network/msgwrap/wrapformat.cpp \
    Network/msgwrap/basemsgwrap.cpp \
    Network/msgwrap/wrapfactory.cpp \
    Network/msgwrap/localmsgwrap.cpp \
    Network/msgwrap/binary716_wrapfromat.cpp \
    Network/msgparse/binary716_msgparse.cpp \
    Network/msgprocess/data716process.cpp \
    thread/netconnector.cpp \
    thread/filesendqueuethread.cpp \
    widgets/mainwindow.cpp \
    widgets/baseinfomodule/baseinfodockpanel.cpp \
    widgets/connectionmodule/connectiondockpanel.cpp \
    widgets/operatemodule/operetepanel.cpp \
    widgets/baseinfomodule/baseinfowidget.cpp \
    widgets/connectionmodule/connectionwidget.cpp \
    widgets/operatemodule/operatewidget.cpp \
    widgets/connectionmodule/mode/connectionmonitormodel.cpp \
    widgets/baseinfomodule/baseinfoedit.cpp \
    widgets/baseinfomodule/databaseedit.cpp \
    messdispatch.cpp

HEADERS  += \
    sql/sqlprocess.h \
    thread/recvtextprocessthread.h \
    thread/sendtextprocessthread.h \
    jsonkey.h \
    Network/msgwrap/binary_wrapformat.h \
    Network/msgwrap/json_wrapformat.h \
    Network/msgwrap/msgwrap.h \
    Network/msgparse/dataparse.h \
    Network/msgparse/json_msgparse.h \
    Network/msgparse/msgparsefactory.h \
    Network/msgprocess/dataprocess.h \
    Network/msgwrap/wrapformat.h \
    Network/msgwrap/basemsgwrap.h \
    Network/msgwrap/wrapfactory.h \
    Network/msgwrap/localmsgwrap.h \
    Network/msgwrap/binary716_wrapfromat.h \
    Network/msgparse/binary716_msgparse.h \
    Network/msgprocess/data716process.h \
    thread/netconnector.h \
    thread/filesendqueuethread.h \
    widgets/mainwindow.h \
    widgets/baseinfomodule/baseinfodockpanel.h \
    widgets/connectionmodule/connectiondockpanel.h \
    widgets/operatemodule/operetepanel.h \
    widgets/baseinfomodule/baseinfowidget.h \
    widgets/connectionmodule/connectionwidget.h \
    widgets/operatemodule/operatewidget.h \
    widgets/connectionmodule/mode/connectionmonitormodel.h \
    widgets/baseinfomodule/baseinfoedit.h \
    widgets/baseinfomodule/databaseedit.h \
    widgets/baseinfomodule/baseinfoconstant.h \
    messdispatch.h

FORMS    += \
    mainwindow.ui \
    widgets/baseinfomodule/baseinfowidget.ui \
    widgets/connectionmodule/connectionwidget.ui \
    widgets/operatemodule/operatewidget.ui

win32-msvc2013{
    LIBS += ../Lib/Base.lib
    LIBS += ../Lib/Network.lib
    LIBS += ../Lib/plugins/RouteTable.lib
}

win32-g++{
    LIBS+= -L../Lib/ -lBase
    QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder
}

unix{
    LIBS+= -L../Lib/ -lNetwork -lBase
}

INCLUDEPATH += $$PWD/../
INCLUDEPATH += $$PWD/../Plugin

#message($$INCLUDEPATH)

RESOURCES += \
    res.qrc
