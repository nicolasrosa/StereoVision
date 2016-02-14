/*
 * StereoCustom.h
 *
 *  Created on: Dec 3, 2015
 *      Author: nicolasrosa
 */

#ifndef STEREOCUSTOM_H
#define STEREOCUSTOM_H

/* Libraries */
#include <string>

using namespace std;

/* Global Variables */
const string trackbarWindowName = "Stereo Param Setup";
//bool isVideoFile=false,isImageFile=false,needCalibration=false,isStereoParamSetupTrackbarsCreated=false,isTrackingObjects=true;;

/* Threshold, Erosion, Dilation and Blur Constants */
#define THRESH_VALUE   100
#define EROSION_SIZE     5
#define DILATION_SIZE    5
#define BLUR_SIZE        3

/* Trackbars Variables
 * Initial min and max BM Parameters values.These will be changed using trackbars
 */
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

#endif // STEREOCUSTOM_H
