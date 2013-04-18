TEMPLATE = app
TARGET = benchmark
DEPENDPATH += .
INCLUDEPATH += . ../../src/base
QMAKE_LIBDIR += ../../src/base
VPATH += ..

LIBS += -lquillimagefilter
CONFIG += debug

# --input
SOURCES += benchmark.cpp

# --- install
target.path = $$(DESTDIR)/usr/lib/quillimagefilter-benchmark/

tatam.files = runtests.sh
tatam.path  = $$(DESTDIR)/usr/share/quillimagefilter-benchmark/

tatamimage.files = benchmark12.jpg
tatamimage.files += thumb.jpg
tatamimage.files += redEye.jpg
tatamimage.path  = $$(DESTDIR)/usr/share/quillimagefilter-benchmark/

INSTALLS += target tatam tatamimage

# ---clean
QMAKE_CLEAN += \
        *.gcov *.gcno *.log
