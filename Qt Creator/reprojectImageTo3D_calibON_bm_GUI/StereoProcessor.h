/*
 * StereoProcessor.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: nicolasrosa
 */

#ifndef STEREOPROCESSOR_H
#define STEREOPROCESSOR_H

/* Libraries */
#include "opencv2/opencv.hpp"
#include "3DReconstruction.h"

using namespace cv;
using namespace std;

/* Custom Classes */
class StereoCalib{
public:
    StereoCalib(); //Constructor

    string intrinsicsFileName;
    string extrinsicsFileName;
    string QmatrixFileName;

    Mat K,Q;
    double focalLength;
    double baseline;
    bool is320x240;
    bool is640x480;
    bool is1280x720;

    Mat M1,D1,M2,D2;
    Mat R,T,R1,P1,R2,P2;
    Rect roi1, roi2;
    bool isKcreated;
};

class StereoDisparityMap{
public:
    StereoDisparityMap(); //Constructor

    Mat disp_16S;
    Mat disp_8U;
    Mat disp_BGR;
 };

class StereoProcessor{
public:
    StereoProcessor(int inputNum); //Constructor
    int getInputNum();
    void readConfigFile();
    void stereoInit();
    void stereoCalib();
    void createKMatrix();
    void readQMatrix();
    void calculateQMatrix();
    void stereoSetParams();

    Mat imageL[2],imageR[2];
    Mat	imageL_grey[2],imageR_grey[2];
    VideoCapture capL,capR;

    Ptr<StereoBM> bm;
    StereoCalib calib;
    StereoDisparityMap disp;
    Reconstruction3D view3D;
    Size imageSize;
    Point2d imageCenter;
    int numRows;

    bool showStereoParamsValues;
private:
    int inputNum;
};

/* Global Variables */
const std::string trackbarWindowName = "Stereo Param Setup";

#endif // STEREOPROCESSOR_H
