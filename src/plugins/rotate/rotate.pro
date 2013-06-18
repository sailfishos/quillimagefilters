
TARGET = rotate

# --- input
HEADERS += rotate.h \
    rotateplugin.h

SOURCES += rotate.cpp \
    rotateplugin.cpp

equals(QT_MAJOR_VERSION, 5): OTHER_FILES += rotate.json

include(../plugin.pri)
