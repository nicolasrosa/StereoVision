#-------------------------------------------------
#
# Project created by QtCreator 2015-10-13T14:57:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = bin
TARGET = stereovision_GUI

TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv
LIBS += `pkg-config --cflags --libs opencv`

SOURCES += main.cpp\
        mainwindow.cpp \
        trackObject.cpp \
        StereoProcessor.cpp \
        setstereoparams.cpp \
        StereoCustom.cpp \
        StereoFlags.cpp \
        StereoDiff.cpp \
        StereoCalib.cpp \
        StereoDisparityMap.cpp \
        StereoUtils.cpp \
        Reconstruction3D.cpp \
        StereoConfig.cpp

HEADERS += mainwindow.h \
        reprojectImageTo3D.h \
        trackObject.h \
        StereoProcessor.h \
        setstereoparams.h \
        StereoCustom.h \
        StereoFlags.h \
        StereoDiff.h \
        StereoCalib.h \
        StereoDisparityMap.h \
        StereoUtils.h \
        Reconstruction3D.h \
        StereoConfig.h \

FORMS   += mainwindow.ui \
        setstereoparams.ui

OTHER_FILES += \
        config.yml \
        stereoBM.yml \
        intrinsics.yml \
        extrinsics.yml \
        Q.yml \
        stereoSGBM.yml

RESOURCES += \
    icon_resource.qrc
