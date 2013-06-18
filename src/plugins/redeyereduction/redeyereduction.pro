
TARGET = redeyereduction

# --- input
HEADERS += redeyereduction.h \
    redeyedetection.h \
    redeyereductionplugin.h

SOURCES += redeyereduction.cpp \
    redeyereductionplugin.cpp \
    redeyedetection.cpp

equals(QT_MAJOR_VERSION, 5): OTHER_FILES += redeyereduction.json

include(../plugin.pri)
