/*
 * StereoConfigBM.h
 *
 *  Created on: May 4, 2016
 *      Author: nicolasrosa
 */

#ifndef STEREOCONFIGBM_H
#define STEREOCONFIGBM_H

/* Libraries */
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class StereoConfigBM{
public:
    /* Constructor and Destructor */
    StereoConfigBM();
    ~StereoConfigBM();

    void showConfigValues();
    void readStereoBMConfigFile();

    /* Method Configuration Files Path */
    string StereoBMConfigFileName;

    //TODO: Checar se são todos os parâmetros possíveis para configurar o MethodBM
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

#endif // STEREOCONFIGBM_H
