TARGET = gaussian

# --- input
HEADERS += gaussian.h \
    gaussianplugin.h

SOURCES += gaussian.cpp \
    gaussianplugin.cpp

equals(QT_MAJOR_VERSION, 5): OTHER_FILES += gaussian.json

include(../plugin.pri)
