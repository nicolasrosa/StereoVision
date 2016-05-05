/*
 * StereoConfigSGBM.h
 *
 *  Created on: May 4, 2016
 *      Author: nicolasrosa
 */

#ifndef STEREOCONFIGSGBM_H
#define STEREOCONFIGSGBM_H

/* Libraries */
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class StereoConfigSGBM{
public:
    /* Constructor and Destructor */
    StereoConfigSGBM();
    ~StereoConfigSGBM();

    void showConfigValues();
    void readStereoSGBMConfigFile();

    /* Method Configuration Files Path */
    string StereoSGBMConfigFileName;

    //TODO: Checar se são todos os parâmetros possíveis para configurar o MethodSGBM
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

#endif // STEREOCONFIGSGBM_H
