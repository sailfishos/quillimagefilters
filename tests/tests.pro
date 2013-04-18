
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
tatam.files = tests.xml
tatam.path  = $$(DESTDIR)/usr/share/quillimagefilter-tests/

tatamimages.files += images/16_color_palette.png
tatamimages.files += images/16_color_palette.xcf
tatamimages.files += images/redeye.jpg
tatamimages.files += images/854x480_blue.png
tatamimages.path  = $$(DESTDIR)/usr/share/quillimagefilter-tests/images/

INSTALLS += tatam tatamimages
