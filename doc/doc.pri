
# DOXYGEN_BIN=$$findFile(doxygen)
DOXYGEN_BIN=doxygen

QMAKE_EXTRA_TARGETS += doc
doc.target = doc
isEmpty(DOXYGEN_BIN) {
    doc.commands = @echo "Unable to detect doxygen in PATH";
} else {
    doc.commands = @$${DOXYGEN_BIN} doc/quillimagefilter.cfg;
# install rules

    htmldocs.files = ./doc/html/*.html ./doc/html/*.css
    equals(QT_MAJOR_VERSION, 4): htmldocs.path = $$INSTALLDIR/quillimagefilter-doc
    equals(QT_MAJOR_VERSION, 5): htmldocs.path = $$INSTALLDIR/quillimagefilter-qt5-doc
    htmldocs.CONFIG += no_check_exist

    docimages.files = ./doc/html/*.png ./doc/html/*.gif
    equals(QT_MAJOR_VERSION, 4): docimages.path = $$INSTALLDIR/quillimagefilter-doc
    equals(QT_MAJOR_VERSION, 5): docimages.path = $$INSTALLDIR/quillimagefilter-qt5-doc
    docimages.CONFIG += no_check_exist

    docmaps.files = ./doc/html/*.map
    equals(QT_MAJOR_VERSION, 4): docmaps.path = $$INSTALLDIR/quillimagefilter-doc
    equals(QT_MAJOR_VERSION, 5): docmaps.path = $$INSTALLDIR/quillimagefilter-qt5-doc
    docmaps.CONFIG += no_check_exist

    # qt5 complains about empty installs
    equals(QT_MAJOR_VERSION, 4): INSTALLS += htmldocs docimages docmaps
}
doc.depends = FORCE


