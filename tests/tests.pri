INCLUDEPATH += . ../ut_unittests  ../../src/base
QMAKE_LIBDIR += ../../src/base ../bin

LIBS += -lquillimagefilter -lut_unittests
QT += testlib
CONFIG += quillmetadata
CONFIG += debug

# --- install
target.path = $$(DESTDIR)/usr/lib/quillimagefilter-tests/
INSTALLS += target

# --- clean
QMAKE_CLEAN += \
	*.gcov *.gcno *.log

QMAKE_DISTCLEAN += *.gcda *.gcno *.gcov *.log *.xml coverage *.o moc_* Makefile*
