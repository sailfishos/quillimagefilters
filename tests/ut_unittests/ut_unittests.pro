include (../tests.pri)

TEMPLATE = lib
TARGET = ../bin/ut_unittests
LIBS -= -lut_unittests

# Input
HEADERS += unittests.h
SOURCES += unittests.cpp

# Install
target.path = $$(DESTDIR)/usr/lib
