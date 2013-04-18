
TARGET = brightnesscontrast

# --- input
HEADERS += brightnesscontrast.h \
           autocontrast.h \
           brightnesscontrastplugin.h

SOURCES += brightnesscontrast.cpp \
           autocontrast.cpp \
           brightnesscontrastplugin.cpp

include(../plugin.pri)
