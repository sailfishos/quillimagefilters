include (../tests.pri)

TEMPLATE = lib
equals(QT_MAJOR_VERSION, 4) {
    TARGET = ../bin/ut_unittests
    LIBS -= -lut_unittests
}
equals(QT_MAJOR_VERSION, 5) {
    TARGET = ../bin/ut_unittests-qt5
    LIBS -= -lut_unittests-qt5
    CONFIG += create_prl no_install_prl
}

# Input
HEADERS += unittests.h
SOURCES += unittests.cpp

# Install
target.path = $$(DESTDIR)/usr/lib
