/*
 * StereoDisparityMap.h
 *
 *  Created on: Dec 3, 2015
 *      Author: nicolasrosa
 */

#ifndef STEREODISPARITYMAP_H
#define STEREODISPARITYMAP_H

/* Libraries */
#include <opencv2/opencv.hpp>

/* Custom Libraries */
#include "Reconstruction3D.h"
#include "StereoCalib.h"
#include "StereoUtils.h"

using namespace cv;
using namespace std;

class StereoDisparityMap{
public:
    /* Constructor and Destructor */
    StereoDisparityMap(Reconstruction3D *ptr3D,StereoCalib *ptrC, int *ptrX,int *ptrY);
    ~StereoDisparityMap();

    //void calculateDisparities();
    void computeDispDepthInformation();

    /* Pointers */
    Reconstruction3D *view3D_ptr;
    StereoCalib *calib_ptr;
    int *x_ptr,*y_ptr;

    /* Raw Disparities Maps */
    Mat disp_16S;
    Mat disp_8U;
    Mat disp_BGR;

    Mat disp_8U_last;
    Mat disp_8U_diff;

    /* Image Processing Results */
    //Mat disp_8U_eroded;
    //Mat disp_8U_eroded_dilated;
};

#endif // STEREODISPARITYMAP_H
