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
#include "setstereoparams.h"
#include "ui_setstereoparams.h"

using namespace cv;
using namespace std;

///* Trackbars Variables
// * Initial min and max BM Parameters values.These will be changed using trackbars
// */
//int preFilterSize			 = 50;	const int preFilterSize_MAX		 	= 100;
//int preFilterCap			 = 100;	const int preFilterCap_MAX		 	= 100;
//int SADWindowSize			 = 25;	const int SADWindowSize_MAX		 	= 100;
//int minDisparity			 = 47;	const int minDisparity_MAX		 	= 100;
//int numberOfDisparities		 = 3;	const int numberOfDisparities_MAX 	= 16;
//int textureThreshold		 = 30;	const int textureThreshold_MAX		= 100;
//int uniquenessRatio			 = 0;	const int uniquenessRatio_MAX		= 100;
//int speckleWindowSize		 = 0;	const int speckleWindowSize_MAX	 	= 100;
//int speckleRange			 = 0;	const int speckleRange_MAX		 	= 100;
//int disp12MaxDiff			 = 1;	const int disp12MaxDiff_MAX		 	= 1;

const int preFilterSize_MAX		 	= 100;
const int preFilterCap_MAX		 	= 100;
const int SADWindowSize_MAX		 	= 100;
const int minDisparity_MAX		 	= 100;
const int numberOfDisparities_MAX 	= 16;
const int textureThreshold_MAX		= 100;
const int uniquenessRatio_MAX		= 100;
const int speckleWindowSize_MAX	 	= 100;
const int speckleRange_MAX		 	= 100;
const int disp12MaxDiff_MAX		 	= 1;

/* Custom Classes */
class StereoConfig : public SetStereoParams{
public:
    StereoConfig(); //Constructor
    //void setInitialTrackbarsValues();

    int preFilterSize;
    int preFilterCap;
    int SADWindowSize;
    int minDisparity;
    int numberOfDisparities;
    int textureThreshold;
    int uniquenessRatio;
    int speckleWindowSize;
    int speckleRange;
    int disp12MaxDiff;
};


class StereoCalib{
public:
    StereoCalib(); //Constructor
    void readQMatrix();
    void calculateQMatrix();
    void createKMatrix();

    string intrinsicsFileName;
    string extrinsicsFileName;
    string QmatrixFileName;
    string StereoParamFileName;

    Point2d imageCenter;

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

class StereoProcessor: public Ui_SetStereoParams{
public:
    StereoProcessor(int inputNum); //Constructor
    int getInputNum();

    void readConfigFile();
    void readStereoConfigFile();

    void stereoInit();
    void stereoCalib();
    void stereoSetParams();

    Mat imageL[2],imageR[2];
    Mat	imageL_grey[2],imageR_grey[2];
    VideoCapture capL,capR;

    Ptr<StereoBM> bm;
    StereoCalib calib;
    StereoConfig stereocfg;
    StereoDisparityMap disp;
    Reconstruction3D view3D;
    Size imageSize;
    int numRows;

    bool showStereoParamsValues;
private:
    int inputNum;
};

/* Global Variables */
const std::string trackbarWindowName = "Stereo Param Setup";

#endif // STEREOPROCESSOR_H
