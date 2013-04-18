##########
# main quillimagefilter project file
##########

TEMPLATE = subdirs

CONFIG += ordered
CONFIG += release

SUBDIRS = src \
           tests
#          src/benchmark \

contains( doc, no ) {
    message( "Not building the documentation ..." )
}
else {
    include(doc/doc.pri)
    }
