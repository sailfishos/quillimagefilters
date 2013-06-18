
TARGET = crop

# --- input
HEADERS += crop.h cropplugin.h

SOURCES += crop.cpp cropplugin.cpp

equals(QT_MAJOR_VERSION, 5): OTHER_FILES += crop.json

include(../plugin.pri)
