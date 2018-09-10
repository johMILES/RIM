TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    Base \
    Network \
    Plugin \
    Core

TRANSLATIONS += $${TARGET}_zh_CN.ts

win32-g++{
    QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder
}

## {
#    QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
#    QMAKE_LFLAGS_CONSOLE = /SUBSYSTEM:CONSOLE,5.01

#    DEFINES += _ATL_XP_TARGETING
#    QMAKE_CFLAGS += /D_USING_V140_SDK71_
#    QMAKE_CXXFLAGS += /D_USING_V140_SDK71_
#    LIBS += -L$$quote(C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib)
#    INCLUDEPATH += $$quote(C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Include)
#}
