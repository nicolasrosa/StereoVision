/*
 * StereoProcessor.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: nicolasrosa
 */

#ifndef STEREOPROCESSOR_H
#define STEREOPROCESSOR_H

/* Libraries */
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

/* Custom Classes*/
class StereoProcessor{
public:
    StereoProcessor(int inputNum); //Constructor
    int getInputNum();
    void readConfigFile();
    void readQMatrix(Mat &Q,double* focalLength,double* baseline,bool is640x480);
    void stereoInit();

    string intrinsicsFileName;
    string extrinsicsFileName;
    string QmatrixFileName;

    Ptr<StereoBM> bm;
private:
    int inputNum;
};

#endif // STEREOPROCESSOR_H
