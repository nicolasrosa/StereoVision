/*
 * StereoProcessor.h
 *
 *  Created on: Oct 20, 2015
 *      Author: nicolasrosa
 */

#ifndef STEREOPROCESSOR_H
#define STEREOPROCESSOR_H

/* Libraries */
#include <opencv2/opencv.hpp>

/* Custom Libraries */
#include "StereoCalib.h"
#include "StereoCustom.h"
#include "StereoConfig.h"
#include "StereoDiff.h"
#include "StereoDisparityMap.h"
#include "StereoFlags.h"
#include "StereoUtils.h"

#include "3DReconstruction.h"
#include "mainwindow.h"

using namespace cv;
using namespace std;

class StereoProcessor : public StereoConfig{
public:
    StereoProcessor(int inputNum); //Constructor
    int getInputNum();
    void openStereoSource(int inputNum);

    void readConfigFile();
    void readStereoConfigFile();

    void stereoBM_Init();
    void stereoSGBM_Init();
    void stereoCalib();
    void setStereoBM_Params();
    void setStereoSGBM_Params();

    void setValues(int preFilterSize, int preFilterCap, int sadWindowSize, int minDisparity, int numOfDisparities, int textureThreshold, int uniquenessRatio, int speckleWindowSize, int speckleWindowRange, int disp12MaxDiff);

    void imageProcessing(Mat src, Mat imgE, Mat imgED,Mat trackingView,bool isTrackingObjects);

    void saveLastFrames();

    void videoLooper();

    Mat imageL[2],imageR[2];
    Mat	imageL_grey[2],imageR_grey[2];
    VideoCapture capL,capR;

    Ptr<StereoBM> bm;
    Ptr<StereoSGBM> sgbm;

    StereoCalib calib;
    StereoConfig stereoBMcfg;
    StereoConfig stereoSGBMcfg;
    StereoDisparityMap disp;
    Reconstruction3D view3D;
    StereoDiff diff;
    StereoFlags flags;
    StereoUtils utils;
    Size imageSize;
    int numRows;
    int numChannels;
    int frameCounter;

    /* Results */
    Mat imgThreshold;
    Mat trackingView;

private:
    int inputNum;
};

#endif // STEREOPROCESSOR_H
