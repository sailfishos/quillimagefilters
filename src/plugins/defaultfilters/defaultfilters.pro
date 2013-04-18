
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

include(../plugin.pri)
