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

/* Global Variables */
bool isStereoParamSetupTrackbarsCreated=false;
bool isTrackingObjects=true;

#endif /* reproject_Image_To_3D_LIB_H_ */
