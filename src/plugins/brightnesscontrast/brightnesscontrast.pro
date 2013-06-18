
TARGET = brightnesscontrast

# --- input
HEADERS += brightnesscontrast.h \
           autocontrast.h \
           brightnesscontrastplugin.h

SOURCES += brightnesscontrast.cpp \
           autocontrast.cpp \
           brightnesscontrastplugin.cpp

equals(QT_MAJOR_VERSION, 5): OTHER_FILES += brightnesscontrast.json

include(../plugin.pri)
