#-------------------------------------------------
#
# Project created by QtCreator 2015-10-13T14:57:07
#
#-------------------------------------------------

# App Configuration
TEMPLATE = app
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Libraries
INCLUDEPATH += /usr/local/include/opencv \
               /usr/include/vtk-5.8 \
               /usr/include/boost
LIBS += `pkg-config --cflags --libs opencv ` -lopencv_gpu -lboost_system

# PCL Libraries
unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += pcl_registration-1.7
unix: PKGCONFIG += pcl_geometry-1.7
unix: PKGCONFIG += pcl_features-1.7
unix: PKGCONFIG += pcl_search-1.7
unix: PKGCONFIG += pcl_kdtree-1.7
unix: PKGCONFIG += pcl_filters-1.7
unix: PKGCONFIG += pcl_surface-1.7
unix: PKGCONFIG += pcl_octree-1.7
unix: PKGCONFIG += pcl_sample_consensus-1.7
unix: PKGCONFIG += pcl_segmentation-1.7
unix: PKGCONFIG += pcl_visualization-1.7
unix: PKGCONFIG += pcl_io-1.7
unix: PKGCONFIG += pcl_apps-1.7
unix: PKGCONFIG += pcl_keypoints-1.7
unix: PKGCONFIG += pcl_tracking-1.7

# Sources
SOURCES += src/main.cpp \
        src/MainWindow.cpp \
        src/trackObject.cpp \
        src/StereoProcessor.cpp \
        src/StereoCustom.cpp \
        src/StereoFlags.cpp \
        src/StereoDiff.cpp \
        src/StereoCalib.cpp \
        src/StereoDisparityMap.cpp \
        src/StereoUtils.cpp \
        src/Reconstruction3D.cpp \
        src/StereoConfig.cpp \
        src/StereoMorphology.cpp \
        src/StereoSetParamsWindow.cpp \
        src/StereoInput.cpp

HEADERS += \ inc/MainWindow.h \
        inc/StereoProcessor.h \
        inc/StereoCustom.h \
        inc/StereoFlags.h \
        inc/StereoDiff.h \
        inc/StereoCalib.h \
        inc/StereoDisparityMap.h \
        inc/StereoUtils.h \
        inc/StereoConfig.h \
        inc/StereoMorphology.h \
        inc/StereoSetParamsWindow.h \
        inc/trackObject.h \
        inc/Reconstruction3D.h \
        inc/StereoInput.h

FORMS   += forms/mainwindow.ui \
        forms/setstereoparams.ui \

# Binaries Location
DESTDIR = bin
TARGET = stereovision_GUI

# Temporary directories
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
UI_DIR = tmp/moc
RCC_DIR = tmp/rc

OTHER_FILES += \
        config/config.yml \
        config/stereoBM.yml \
        config/intrinsics.yml \
        config/Q.yml \
        config/stereoSGBM.yml \
        config/stereoBM_GPU.yml

RESOURCES += \
    res/icon_resource.qrc
