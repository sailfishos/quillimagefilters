
TEMPLATE = subdirs

DEPENDPATH += .
INCLUDEPATH += .

CONFIG += ordered

SUBDIRS += \
           ut_unittests \
           ut_autocontrast \
           ut_autolevels \
           ut_brightnesscontrast \
           ut_crop \
           ut_filterframework \
           ut_flip \
           ut_scale \
           ut_scalecrop \
           ut_quillimage \
           ut_rotate \
           ut_freerotate \
           ut_save \
           ut_serialsaver \
           ut_overlay \
           ut_redeyedetection \
           ut_redeyereduction \
           ut_load \
           ut_gaussian \
           ut_lanczos \
           ut_vintage \
           ut_plugininterface \
           benchmark

# --- install
equals(QT_MAJOR_VERSION, 4) {
    tatam.files = tests.xml
    tatam.path  = $$(DESTDIR)/usr/share/quillimagefilter-tests/
}
equals(QT_MAJOR_VERSION, 5) {
    tatam.files = qt5/tests.xml
    tatam.path  = $$(DESTDIR)/usr/share/quillimagefilter-qt5-tests/
}

tatamimages.files += images/16_color_palette.png
tatamimages.files += images/16_color_palette.xcf
tatamimages.files += images/redeye.jpg
tatamimages.files += images/854x480_blue.png
equals(QT_MAJOR_VERSION, 4): tatamimages.path  = $$(DESTDIR)/usr/share/quillimagefilter-tests/images/
equals(QT_MAJOR_VERSION, 5): tatamimages.path  = $$(DESTDIR)/usr/share/quillimagefilter-qt5-tests/images/

INSTALLS += tatam tatamimages
