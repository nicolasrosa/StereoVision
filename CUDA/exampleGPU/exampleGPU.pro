TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR = bin
TARGET = exampleGPU

# Temporary dirs
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
UI_DIR = tmp/moc
RCC_DIR = tmp/rc

INCLUDEPATH += /usr/local/include/opencv
LIBS += `pkg-config --cflags --libs opencv` -lopencv_gpu -lopencv_features2d

SOURCES += \
    exampleGPU.cpp

OTHER_FILES += \
    bin/teddy_l.png

HEADERS +=

