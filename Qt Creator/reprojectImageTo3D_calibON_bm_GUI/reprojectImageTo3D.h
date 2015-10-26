/*
 * reprojectImageTo3D.h
 *
 *  Created on: Jun 18, 2015
 *      Author: nicolasrosa
 */

#ifndef reproject_Image_To_3D_LIB_H_
#define reproject_Image_To_3D_LIB_H_

/* Libraries */
#include "cv.h"
#include "highgui.h"
#include "cvaux.h"
#include "stdio.h"
#include "opencv2/opencv.hpp"

/* Custom Libraries */
#include "trackObject.h"
#include "StereoProcessor.h"
#include "3DReconstruction.h"
#include "setstereoparams.h"

using namespace cv;
using namespace std;

/* Calibration */
#define RESOLUTION_640x480
//#define RESOLUTION_1280x720
#define CALIBRATION_ON

/* Threshold, Erosion, Dilation and Blur Constants */
#define THRESH_VALUE 100
#define EROSION_SIZE 5  //SAR
#define DILATION_SIZE  5	//SAR
#define BLUR_SIZE 3

/* Functions Scope */
void on_trackbar(int,void*);
bool createTrackbars();


//void stereoInit(StereoBM* bm);
void stereoSetparams(Rect* roi1,Rect* roi2,StereoBM* bm,int numRows,bool showStereoBMparams);
//void readQMatrix(Mat &Q,double* focalLength, double* baseline,StereoProcessor* stereo);
//void calculateQMatrix(Mat &Q,Point2d imageCenter,double focalLength, double baseline);
void imageProcessing1(Mat img, Mat imgMedian, Mat imgMedianBGR);
void imageProcessing2(Mat src, Mat imgE, Mat imgED,Mat cameraFeedL,bool isTrackingObjects);

void resizeFrames(Mat* frame1,Mat* frame2);
void changeResolution(VideoCapture* cap_l,VideoCapture* cap_r);
void contrast_and_brightness(Mat &left,Mat &right,float alpha,float beta);


/* Global Variables */
const std::string trackbarWindowName = "Stereo Param Setup";

bool isVideoFile=false,isImageFile=false,needCalibration=false,isStereoParamSetupTrackbarsCreated=false,isTrackingObjects=true;;
bool showInputImages=true,showXYZ=false,showStereoParam=false,showStereoParamValues,showFPS=false,showDisparityMap=false,show3Dreconstruction=false,showDiffImage=false;

/* Trackbars Variables
 * Initial min and max BM Parameters values.These will be changed using trackbars
 */
int preFilterSize			 = 50;	const int preFilterSize_MAX		 	= 100;
int preFilterCap			 = 100;	const int preFilterCap_MAX		 	= 100;
int SADWindowSize			 = 25;	const int SADWindowSize_MAX		 	= 100;
int minDisparity			 = 47;	const int minDisparity_MAX		 	= 100;
int numberOfDisparities		 = 3;	const int numberOfDisparities_MAX 	= 16;
int textureThreshold		 = 30;	const int textureThreshold_MAX		= 100;
int uniquenessRatio			 = 0;	const int uniquenessRatio_MAX		= 100;
int speckleWindowSize		 = 0;	const int speckleWindowSize_MAX	 	= 100;
int speckleRange			 = 0;	const int speckleRange_MAX		 	= 100;
int disp12MaxDiff			 = 1;	const int disp12MaxDiff_MAX		 	= 1;

#endif /* reproject_Image_To_3D_LIB_H_ */
