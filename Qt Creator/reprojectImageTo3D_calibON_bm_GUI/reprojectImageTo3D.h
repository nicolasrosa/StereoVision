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
#include <fstream>

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

/* Functions Scope */
void on_trackbar(int,void*);

//void stereoInit(StereoBM* bm);
//void stereoSetparams(Rect* roi1,Rect* roi2,StereoBM* bm,int numRows,bool showStereoBMparams);
//void readQMatrix(Mat &Q,double* focalLength, double* baseline,StereoProcessor* stereo);
//void calculateQMatrix(Mat &Q,Point2d imageCenter,double focalLength, double baseline);
void imageProcessing1(Mat img, Mat imgMedian, Mat imgMedianBGR);
void imageProcessing2(Mat src, Mat imgE, Mat imgED,Mat cameraFeedL,bool isTrackingObjects);

void resizeFrames(Mat* frame1,Mat* frame2);
void changeResolution(VideoCapture* cap_l,VideoCapture* cap_r);
void contrast_and_brightness(Mat &left,Mat &right,float alpha,float beta);


/* Global Variables */
bool isVideoFile=false,isImageFile=false,needCalibration=false,isStereoParamSetupTrackbarsCreated=false,isTrackingObjects=true;;
//bool showInputImages=true,showXYZ=false,showStereoParam=false,showStereoParamValues,showFPS=false,showDisparityMap=false,show3Dreconstruction=false,showTrackingObjectView=false,showDiffImage=false;

#endif /* reproject_Image_To_3D_LIB_H_ */
