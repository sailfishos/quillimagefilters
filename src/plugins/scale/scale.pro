TARGET = scale

# --- input
HEADERS += scale.h \
    scaleplugin.h

SOURCES += scale.cpp \
    scaleplugin.cpp

OTHER_FILES += scale.json

include(../plugin.pri)
