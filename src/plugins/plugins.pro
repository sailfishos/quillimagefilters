##########
# src midlevel project file
##########

TEMPLATE = subdirs

CONFIG += ordered
MOC_DIR = .moc

SUBDIRS += \
brightnesscontrast\
crop\
flip\
freerotate\
gaussian\
lanczos\
levels\
redeyereduction\
rotate\
scale\
defaultfilters\
vintage
