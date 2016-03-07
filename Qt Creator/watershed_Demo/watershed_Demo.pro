# App Configuration
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

# Libraries
INCLUDEPATH += /usr/local/include/opencv
LIBS += `pkg-config --cflags --libs opencv` -lopencv_gpu -I/usr/local/cuda/include -I/usr/local/cuda-7.5/include

SOURCES += \
    imageSegmentation.cpp

# Binaries Location
DESTDIR = bin
TARGET = imageSegmentation

# Temporary directories
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
UI_DIR = tmp/moc
RCC_DIR = tmp/rc


OTHER_FILES +=

HEADERS +=
