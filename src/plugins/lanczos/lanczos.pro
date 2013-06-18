
TARGET = lanczos

# --- input
HEADERS += lanczos.h \
    lanczosplugin.h

SOURCES += lanczos.cpp \
    lanczosplugin.cpp

equals(QT_MAJOR_VERSION, 5): OTHER_FILES += lanczos.json

include(../plugin.pri)
