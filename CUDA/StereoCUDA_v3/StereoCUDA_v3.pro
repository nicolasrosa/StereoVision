# App Configuration
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

# Libraries
INCLUDEPATH += /usr/local/include/opencv
LIBS += `pkg-config --cflags --libs opencv` -lopencv_gpu -I/usr/local/cuda/include -I/usr/local/cuda-7.5/include

# Sources
SOURCES += \
    src/stereocuda_v3.cpp \
    src/params.cpp \
    src/app.cpp \
    src/StereoCalib.cpp

# Headers
HEADERS += \
    inc/params.h \
    inc/app.h \
    inc/StereoCalib.h

# Binaries Location
DESTDIR = bin
TARGET = stereoCUDA

# Temporary directories
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
UI_DIR = tmp/moc
RCC_DIR = tmp/rc


OTHER_FILES +=


