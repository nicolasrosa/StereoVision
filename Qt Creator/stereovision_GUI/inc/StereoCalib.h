/*
 * StereoCalib.h
 *
 *  Created on: Dec 3, 2015
 *      Author: nicolasrosa
 */

#ifndef STEREOCALIB_H
#define STEREOCALIB_H

/* Libraries */
#include <string>
#include <opencv2/opencv.hpp>

/* Custom Libraries */
#include "inc/StereoInput.h"

using namespace cv;
using namespace std;

class StereoCalib{
public:
    /* Constructor and Destructor */
    StereoCalib();
    ~StereoCalib();

    void readCalibrationFiles();
    void readIntrinsicsFile();
    void readExtrinsicsFile();
    void readQMatrix();
    void calculateQMatrix();
    void createKMatrix();

    void setResolution(int width,int height);
    void setResolutionDesired(int width,int height);

    Size getResolution();
    Size getResolutionDesired();

    int getResolution_width();
    int getResolution_height();

    int getResolutionDesired_width();
    int getResolutionDesired_height();

    /* Input files Path */
    string imageL_FileName;
    string imageR_FileName;


    /* Calibration files Path */
    string intrinsicsFileName;
    string extrinsicsFileName;
    string QmatrixFileName;

    enum{VideoFile,ImageFile} inputType;

    bool needCalibration;
    bool hasQMatrix;

    /* Method Configuration Files Path */
    string StereoBMConfigFileName;
    string StereoSGBMConfigFileName;
    string StereoBM_GPUConfigFileName;

    Point2d imageCenter;

    Mat K,Q;
    double focalLength;
    double baseline;

    Mat M1,D1,M2,D2;
    Mat R,T,R1,P1,R2,P2;
    Rect roi1, roi2;
    bool isKcreated;

    //TODO: Fazer Privada
    Mat rmap[2][2];

private:
    Size resolution;
    Size resolutionDesired;

};

#endif // STEREOCALIB_H
