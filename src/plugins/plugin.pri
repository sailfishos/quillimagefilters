
TEMPLATE = lib
DEPENDPATH += .
INCLUDEPATH += ../../base
QMAKE_LIBDIR+= ../../base
QMAKE_LFLAGS+= -Wl,--as-needed
equals(QT_MAJOR_VERSION, 4): LIBS += -lquillimagefilter
equals(QT_MAJOR_VERSION, 5): LIBS += -lquillimagefilter-qt5
CONFIG += plugin link_pkgconfig
equals(QT_MAJOR_VERSION, 4): PKGCONFIG += quillmetadata
equals(QT_MAJOR_VERSION, 5): PKGCONFIG += quillmetadata-qt5

# this is for adding coverage information while doing qmake as "qmake COV_OPTION=on"
# message is shown when 'make' is executed
for(OPTION,$$list($$lower($$COV_OPTION))){
    isEqual(OPTION, on){
        message("TEST COVERAGE IS ENABLED")
        QMAKE_CXXFLAGS += -ftest-coverage -fprofile-arcs -fno-elide-constructors
        LIBS += -lgcov
    }
}

# --- install
headers.files = $$INSTALL_HEADERS
headers.path = $$[QT_INSTALL_HEADERS]/$$TARGET
target.path = $$[QT_INSTALL_PLUGINS]/quillimagefilters
INSTALLS += target headers

equals(QT_MAJOR_VERSION, 4) {
    #prf.files = quillimagefilter.prf
    prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
    INSTALLS += prf
}

# --- clean
QMAKE_CLEAN += \
        *.gcov *.gcno *.log

QMAKE_DISTCLEAN += *.gcda *.gcno *.gcov *.log *.xml coverage *.o moc_* Makefile*

