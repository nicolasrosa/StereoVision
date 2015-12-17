/*
 * reprojectImageTo3D.h
 *
 *  Created on: Jun 18, 2015
 *      Author: nicolasrosa
 */

#ifndef reproject_Image_To_3D_LIB_H_
#define reproject_Image_To_3D_LIB_H_

/* Libraries */
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace cv;

/* Calibration */
#define CALIBRATION_ON

/* Functions Scope */
void on_trackbar(int,void*);

void imageProcessing1(Mat img, Mat imgMedian, Mat imgMedianBGR);
void imageProcessing2(Mat src, Mat imgE, Mat imgED,Mat cameraFeedL,bool isTrackingObjects);

//void resizeFrames(Mat* frame1,Mat* frame2);
//void changeResolution(VideoCapture* cap_l,VideoCapture* cap_r);
//void contrast_and_brightness(Mat &left,Mat &right,float alpha,float beta);

/* Global Variables */
bool isVideoFile=false;
bool isImageFile=false;
bool needCalibration=false;
bool isStereoParamSetupTrackbarsCreated=false;
bool isTrackingObjects=true;

#endif /* reproject_Image_To_3D_LIB_H_ */
