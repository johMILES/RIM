QT += core network

CONFIG += c++11

TARGET = Network
CONFIG += console
CONFIG -= app_bundle

DEFINES += NETWORK_LIBRARY

TEMPLATE = lib
DESTDIR += ../Bin

HEADERS += \
    msg.h \
    netglobal.h \
    network_global.h \
    tcpsocket.h \
    tcpserver.h \
    tcpclient.h

SOURCES += \
    netglobal.cpp \
    tcpsocket.cpp \
    tcpserver.cpp \
    tcpclient.cpp

INCLUDEPATH += $$PWD/../../RimClient/

win32-msvc2013{
    LIBS += E:\Git/build-RimClient-Qt570vs64-Debug/Bin/Util.lib
}

unix{
    LIBS+= -L../Bin/ -lNetwork -lUtil
}
