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

using namespace cv;
using namespace std;

class StereoDisparityMap{
public:
    StereoDisparityMap(); //Constructor

    Mat disp_16S;
    Mat disp_8U;
    Mat disp_BGR;
};

#endif // STEREODISPARITYMAP_H
