#-------------------------------------------------
#
# Project created by QtCreator 2015-10-13T14:57:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = reprojectImageTo3D_calibON_bm_GUI
TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_photo -lopencv_superres -lopencv_ts -lopencv_videoio -lopencv_videostab -lopencv_shape -lopencv_stitching -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_flann

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
        StereoConfig.h

FORMS   += mainwindow.ui \
        setstereoparams.ui

OTHER_FILES += \
        config.yml \
        stereoBM.yml \
        intrinsics.yml \
        extrinsics.yml \
        Q.yml \
        stereoSGBM.yml
