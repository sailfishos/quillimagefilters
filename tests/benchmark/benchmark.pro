TEMPLATE = app
TARGET = benchmark
DEPENDPATH += .
INCLUDEPATH += . ../../src/base
QMAKE_LIBDIR += ../../src/base
VPATH += ..

equals(QT_MAJOR_VERSION, 4): LIBS += -lquillimagefilter
equals(QT_MAJOR_VERSION, 5): LIBS += -lquillimagefilter-qt5
CONFIG += debug

equals(QT_MAJOR_VERSION, 5): QT += gui

# --input
SOURCES += benchmark.cpp

# --- install
equals(QT_MAJOR_VERSION, 4): target.path = $$(DESTDIR)/usr/lib/quillimagefilter-benchmark/
equals(QT_MAJOR_VERSION, 5): target.path = $$(DESTDIR)/usr/lib/quillimagefilter-qt5-benchmark/

equals(QT_MAJOR_VERSION, 4) {
    tatam.files = runtests.sh
    tatam.path  = $$(DESTDIR)/usr/share/quillimagefilter-benchmark/
}
equals(QT_MAJOR_VERSION, 5) {
    tatam.files = runtests-qt5.sh
    tatam.path  = $$(DESTDIR)/usr/share/quillimagefilter-qt5-benchmark/
}

tatamimage.files = benchmark12.jpg
tatamimage.files += thumb.jpg
tatamimage.files += redEye.jpg
equals(QT_MAJOR_VERSION, 4): tatamimage.path  = $$(DESTDIR)/usr/share/quillimagefilter-benchmark/
equals(QT_MAJOR_VERSION, 5): tatamimage.path  = $$(DESTDIR)/usr/share/quillimagefilter-qt5-benchmark/

INSTALLS += target tatam tatamimage

# ---clean
QMAKE_CLEAN += \
        *.gcov *.gcno *.log
