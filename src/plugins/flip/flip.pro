
TARGET = flip

# --- input
HEADERS += flip.h \
    flipplugin.h

SOURCES += flip.cpp \
    flipplugin.cpp

equals(QT_MAJOR_VERSION, 5): OTHER_FILES += flip.json

include(../plugin.pri)
