/*
 * StereoConfig.h
 *
 *  Created on: Dec 3, 2015
 *      Author: nicolasrosa
 */

#ifndef STEREOCONFIG_H
#define STEREOCONFIG_H

class StereoConfig{
public:
    StereoConfig(); //Constructor
    //StereoConfig getConfig();

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
