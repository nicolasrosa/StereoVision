/*
 * StereoConfig.h
 *
 *  Created on: Dec 3, 2015
 *      Author: nicolasrosa
 */

#ifndef STEREOCONFIG_H
#define STEREOCONFIG_H

/* Libraries */
#include <string>
#include <iostream>

//using namespace cv;
using namespace std;

class StereoConfig{
public:
    /* Constructor and Destructor */
    StereoConfig();
    ~StereoConfig();

    void showConfigValues();

    string methodName;
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

#endif // STEREOCONFIG_H
