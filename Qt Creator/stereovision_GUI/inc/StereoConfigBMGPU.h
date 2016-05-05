/*
 * StereoConfigBMGPU.h
 *
 *  Created on: May 4, 2016
 *      Author: nicolasrosa
 */

#ifndef STEREOCONFIGBMGPU_H
#define STEREOCONFIGBMGPU_H

/* Libraries */
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class StereoConfigBMGPU{
public:
    /* Constructor and Destructor */
    StereoConfigBMGPU();
    ~StereoConfigBMGPU();

    void showConfigValues();
    void readStereoBM_GPUConfigFile();

    /* Method Configuration Files Path */
    string StereoBM_GPUConfigFileName;

    //TODO: Checar se são todos os parâmetros possíveis para configurar o MethodBMGPU
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

#endif // STEREOCONFIGBMGPU_H
