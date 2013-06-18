
TARGET = freerotate

# --- input
HEADERS += freerotate.h \
    freerotateplugin.h

SOURCES += freerotate.cpp \
    freerotateplugin.cpp

equals(QT_MAJOR_VERSION, 5): OTHER_FILES += freerotate.json

include(../plugin.pri)
