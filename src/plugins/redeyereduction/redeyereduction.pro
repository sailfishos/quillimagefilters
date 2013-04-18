
TARGET = redeyereduction

# --- input
HEADERS += redeyereduction.h \
    redeyedetection.h \
    redeyereductionplugin.h

SOURCES += redeyereduction.cpp \
    redeyereductionplugin.cpp \
    redeyedetection.cpp

include(../plugin.pri)
