
TARGET = levels

# --- input
HEADERS += levels.h \
    levelsplugin.h \
    autolevels.h

SOURCES += levels.cpp \
    levelsplugin.cpp \
    autolevels.cpp

equals(QT_MAJOR_VERSION, 5): OTHER_FILES += levels.json

include(../plugin.pri)
