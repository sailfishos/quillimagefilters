
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
    htmldocs.path = $$INSTALLDIR/quillimagefilter-doc
    htmldocs.CONFIG += no_check_exist

    docimages.files = ./doc/html/*.png ./doc/html/*.gif
    docimages.path = $$INSTALLDIR/quillimagefilter-doc
    docimages.CONFIG += no_check_exist

    docmaps.files = ./doc/html/*.map
    docmaps.path = $$INSTALLDIR/quillimagefilter-doc
    docmaps.CONFIG += no_check_exist

    INSTALLS += htmldocs docimages docmaps
}
doc.depends = FORCE


