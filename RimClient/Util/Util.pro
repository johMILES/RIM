#-------------------------------------------------
#
# Project created by QtCreator 2018-01-08T14:41:47
#
#-------------------------------------------------

QT  += core gui

contains(QT_MAJOR_VERSION, 5): QT += widgets gui-private

#CONFIG += build_all


TARGET = Util
TEMPLATE = lib

DESTDIR = ../Bin

DEFINES += UTIL_LIBRARY

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32-g++{
    QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder
}

INCLUDEPATH += $$PWD/../

SOURCES += \
    fileutils.cpp \
    imagemanager.cpp \
    rlog.cpp \
    rutil.cpp

HEADERS +=\
        util_global.h \
    fileutils.h \
    imagemanager.h \
    rlog.h \
    rutil.h
