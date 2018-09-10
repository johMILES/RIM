#-------------------------------------------------
#
# Project created by QtCreator 2018-09-02T22:12:38
#
#-------------------------------------------------

QT += core gui xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RouteTable
TEMPLATE = lib

CONFIG -= app_bundle

DESTDIR = ../../Lib/plugins
DLLDESTDIR = ../../Bin

DEFINES += ROUTETABLE_LIBRARY
DEFINES += __LOCAL_CONTACT__

SOURCES += routetable.cpp \
    newclientnode.cpp \
    newservernode.cpp \
    routesettings.cpp \
    routetableclientdelegate.cpp \
    routetableclientmodel.cpp \
    routetablehead.cpp \
    routetableserverdelegate.cpp \
    routetableservermodel.cpp \
    routetableview.cpp

HEADERS += routetable.h\
        routetable_global.h \
    newclientnode.h \
    newservernode.h \
    routesettings.h \
    routetableclientdelegate.h \
    routetableclientmodel.h \
    routetablehead.h \
    routetableserverdelegate.h \
    routetableservermodel.h \
    routetableview.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += $$PWD/../../

win32-msvc2013{
    LIBS += ../../Lib/Base.lib
    LIBS += ../../Lib/Network.lib
}
