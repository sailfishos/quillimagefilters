
TARGET = defaultfilters

# --- used by serialsaver
LIBS += -ljpeg

# --- input
HEADERS += loadfilter.h \
           overlay.h \
	   savefilter.h \
           scalecrop.h \
           serialsaver.h \
           defaultfiltersplugin.h \
           mimetype.h


SOURCES += loadfilter.cpp \
           overlay.cpp \
	   savefilter.cpp \
           scalecrop.cpp \
           serialsaver.cpp \
           defaultfiltersplugin.cpp \
           mimetype.cpp

equals(QT_MAJOR_VERSION, 5): OTHER_FILES += defaultfilters.json

include(../plugin.pri)
