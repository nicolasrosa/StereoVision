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

    bool isVideoFile;
    bool isImageFile;

    //TODO: Fix hasQMatrix
    bool hasQMatrix;

    string intrinsicsFileName;
    string extrinsicsFileName;
    string QmatrixFileName;
    string StereoBMConfigFileName;
    string StereoSGBMConfigFileName;

    Size imageSize;
    Size imageSizeDesired;

    Point2d imageCenter;

    Mat K,Q;
    double focalLength;
    double baseline;
//    bool is320x240;
//    bool is640x480;
//    bool is1280x720;

    Mat M1,D1,M2,D2;
    Mat R,T,R1,P1,R2,P2;
    Rect roi1, roi2;
    bool isKcreated;
};

#endif // STEREOCALIB_H
