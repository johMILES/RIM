#-------------------------------------------------
#
# Project created by QtCreator 2018-01-08T15:00:17
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = network
TEMPLATE = lib

DEFINES += NETWORK_LIBRARY

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

INCLUDEPATH += $$PWD/../
INCLUDEPATH += $$PWD/multitransmits/

win32-msvc2013{
    LIBS+= ../Lib/Util.lib
    LIBS+= ../Lib/DDS.lib
}

win32-g++{
    LIBS+= -L../Lib -lUtil
    QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

SOURCES += \
    netglobal.cpp \
    rsocket.cpp \
    rtask.cpp \
    aes/AES.cpp \
    aes/raes.cpp \
    wraprule/wraprule.cpp \
    wraprule/datapacketrule.cpp \
    win32net/rudpsocket.cpp \
    multitransmits/basetransmit.cpp \
    multitransmits/tcptransmit.cpp \
    win32net/msgsender.cpp \
    win32net/msgreceive.cpp \
    multitransmits/ddstransmit.cpp

HEADERS +=\
        network_global.h \
    netglobal.h \
    rsocket.h \
    rtask.h \
    aes/AES.h \
    aes/raes.h \
    wraprule/wraprule.h \
    wraprule/datapacketrule.h \
    win32net/rudpsocket.h \
    multitransmits/basetransmit.h \
    multitransmits/tcptransmit.h \
    win32net/msgreceive.h \
    win32net/msgsender.h \
    multitransmits/ddstransmit.h \
    3rdhead/DDS.h
