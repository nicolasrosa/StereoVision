#-------------------------------------------------
#
# Project created by QtCreator 2013-07-22T22:57:54
#
#-------------------------------------------------

QT       += core gui

CONFIG += debug
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opencvqt
TEMPLATE = app


INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core

SOURCES += main.cpp\
        mainwindow.cpp \
    opencview.cpp \
    targetfinder.cpp

HEADERS  += mainwindow.h \
    opencview.h \
    targetfinder.h

FORMS    += mainwindow.ui
