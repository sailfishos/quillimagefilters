
TARGET = levels

# --- input
HEADERS += levels.h \
    levelsplugin.h \
    autolevels.h

SOURCES += levels.cpp \
    levelsplugin.cpp \
    autolevels.cpp

include(../plugin.pri)
