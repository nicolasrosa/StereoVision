# App Configuration
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

# Libraries
INCLUDEPATH += /usr/local/include/opencv
LIBS += `pkg-config --cflags --libs opencv` -lopencv_gpu -I/usr/local/cuda/include -I/usr/local/cuda-7.5/include

# Sources
SOURCES += \
    src/params.cpp \
    src/app.cpp \
    src/main.cpp \
    src/StereoCalib.cpp \
    src/StereoUtils.cpp

# Headers
HEADERS += \
    inc/params.h \
    inc/app.h \
    inc/main.h \
    inc/StereoCalib.h \
    inc/StereoUtils.h

# Binaries Location
DESTDIR = bin
TARGET = stereoCUDA

# Temporary directories
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
UI_DIR = tmp/moc
RCC_DIR = tmp/rc


OTHER_FILES += \
    config/extrinsics.yml \
    config/intrinsics.yml \
    config/Q.yml
