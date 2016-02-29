# App Configuration
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

# Libraries
INCLUDEPATH += /usr/local/include/opencv
LIBS += `pkg-config --cflags --libs opencv` -lopencv_gpu

# Sources
SOURCES += \
    exampleGPU.cpp

# Binaries Location
DESTDIR = bin
TARGET = exampleGPU

# Temporary directories
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
UI_DIR = tmp/moc
RCC_DIR = tmp/rc


OTHER_FILES += \
    bin/teddy_l.png \
    ../../data/teddy_l.png

HEADERS +=
