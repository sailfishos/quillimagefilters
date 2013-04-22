INCLUDEPATH += . ../ut_unittests  ../../src/base
QMAKE_LIBDIR += ../../src/base ../bin

equals(QT_MAJOR_VERSION, 4): LIBS += -lquillimagefilter -lut_unittests
equals(QT_MAJOR_VERSION, 5): LIBS += -lquillimagefilter-qt5 -lut_unittests-qt5
QT += testlib gui
CONFIG += debug link_pkgconfig
equals(QT_MAJOR_VERSION, 4): PKGCONFIG += quillmetadata
equals(QT_MAJOR_VERSION, 5): PKGCONFIG += quillmetadata-qt5

# --- install
equals(QT_MAJOR_VERSION, 4): target.path = $$(DESTDIR)/usr/lib/quillimagefilter-tests/
equals(QT_MAJOR_VERSION, 5): target.path = $$(DESTDIR)/usr/lib/quillimagefilter-qt5-tests/
INSTALLS += target

# --- clean
QMAKE_CLEAN += \
	*.gcov *.gcno *.log

QMAKE_DISTCLEAN += *.gcda *.gcno *.gcov *.log *.xml coverage *.o moc_* Makefile*
