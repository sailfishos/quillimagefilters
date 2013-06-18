##########
# the project file in src/base
##########

TEMPLATE = lib
equals(QT_MAJOR_VERSION, 4): TARGET = quillimagefilter
equals(QT_MAJOR_VERSION, 5): TARGET = quillimagefilter-qt5
DEPENDPATH += .
INCLUDEPATH += .

CONFIG += link_pkgconfig
equals(QT_MAJOR_VERSION, 4): PKGCONFIG += quillmetadata
equals(QT_MAJOR_VERSION, 5): PKGCONFIG += quillmetadata-qt5

# Generate pkg-config support by default
# Note that we HAVE TO also create prl config as QMake implementation
# mixes both of them together.
CONFIG += create_pc create_prl no_install_prl
QMAKE_PKGCONFIG_DESCRIPTION =  Quillimagefilter Library
equals(QT_MAJOR_VERSION, 4) {
    QMAKE_PKGCONFIG_NAME = Quillimagefilter
    QMAKE_PKGCONFIG_REQUIRES = QtGui
}
equals(QT_MAJOR_VERSION, 5) {
    QMAKE_PKGCONFIG_NAME = Quillimagefilter-qt5
    QMAKE_PKGCONFIG_REQUIRES = Qt5Gui
}
QMAKE_PKGCONFIG_INCDIR = $$[QT_INSTALL_HEADERS]/$$TARGET
QMAKE_PKGCONFIG_LIBDIR = $$[QT_INSTALL_LIBS]
QMAKE_PKGCONFIG_DESTDIR = pkgconfig

QMAKE_CXXFLAGS += -Werror
QMAKE_LFLAGS   += -Wl,--as-needed

# this is for adding coverage information while doing qmake as "qmake COV_OPTION=on"
# message is shown when 'make' is executed
for(OPTION,$$list($$lower($$COV_OPTION))){
    isEqual(OPTION, on){
        message("TEST COVERAGE IS ENABLED")
        QMAKE_CXXFLAGS += -ftest-coverage -fprofile-arcs -fno-elide-constructors
        LIBS += -lgcov
    }
}

# --- input
HEADERS += quillimage.h \
           quillimagefilter.h \
           quillimagefilterfactory.h \
           quillimagefilterinterface.h \
           quillimagefilterimplementation.h \
           quillimagefiltergenerator.h \
           quillimagefiltergeneratorimplementation.h \
           pluginloader.h

SOURCES += quillimage.cpp \
           quillimagefilter.cpp \
           quillimagefilterfactory.cpp \
           quillimagefilterimplementation.cpp \
           quillimagefiltergenerator.cpp \
           quillimagefiltergeneratorimplementation.cpp \
           pluginloader.cpp

INSTALL_HEADERS = \
           quillimage.h \
           QuillImage \
           quillimagefilter.h \
           QuillImageFilter \
           quillimagefilterfactory.h \
           QuillImageFilterFactory \
           quillimagefiltergenerator.h \
           QuillImageFilterGenerator \
           quillimagefiltererror.h \
           QuillImageFilterInterface \
           quillimagefilterinterface.h \
           QuillImageFilterImplementation \
           quillimagefilterimplementation.h \
           QuillImageFilterGeneratorImplementation \
           quillimagefiltergeneratorimplementation.h

# --- install
headers.files = $$INSTALL_HEADERS
headers.path = $$[QT_INSTALL_HEADERS]/$$TARGET
target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target headers

equals(QT_MAJOR_VERSION, 4) {
    prf.files = quillimagefilter.prf
    prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
    INSTALLS += prf
}

# --- clean
QMAKE_CLEAN += \
	*.gcov *.gcno *.log

QMAKE_DISTCLEAN += *.gcda *.gcno *.gcov *.log *.xml coverage *.o moc_* Makefile* pkgconfig
