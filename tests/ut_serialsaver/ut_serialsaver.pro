include (../tests.pri)

TEMPLATE = app
TARGET = ../bin/ut_serialsaver
INCLUDEPATH += ../../src/plugins/defaultfilters
QMAKE_LIBDIR += ../../src/plugins/defaultfilters
LIBS += -ljpeg

# Input
HEADERS += ut_serialsaver.h \
           ../../src/plugins/defaultfilters/serialsaver.h
SOURCES += ut_serialsaver.cpp \
           ../../src/plugins/defaultfilters/serialsaver.cpp
