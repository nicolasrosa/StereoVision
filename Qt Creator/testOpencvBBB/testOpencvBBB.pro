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
    testOpencvBBB.cpp

# Binaries Location
DESTDIR = bin
TARGET = testOpencvBBB

# Temporary directories
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
UI_DIR = tmp/moc
RCC_DIR = tmp/rc


OTHER_FILES += \
    lena.jpg

HEADERS +=
