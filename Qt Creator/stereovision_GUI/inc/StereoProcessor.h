/*
 * StereoProcessor.h
 *
 *  Created on: Oct 20, 2015
 *      Author: nicolasrosa
 */

#ifndef STEREOPROCESSOR_H
#define STEREOPROCESSOR_H

/* Libraries */
#include <opencv2/opencv.hpp>
#include "opencv2/cudastereo.hpp"
#include "opencv2/cudaarithm.hpp"

/* Custom Libraries */
#include "StereoCalib.h"
#include "StereoCustom.h"
#include "StereoConfig.h"
#include "StereoDiff.h"
#include "StereoDisparityMap.h"
#include "StereoFlags.h"
#include "StereoUtils.h"
#include "StereoMorphology.h"

#include "Reconstruction3D.h"
#include "MainWindow.h"

using namespace cv;
using namespace std;

class StereoProcessor : public StereoConfig{
public:
    /* Constructor and Destructor */
    StereoProcessor(int inputNum);
    ~StereoProcessor();

    int getInputNum();

    void readConfigFile();
    void readStereoBMConfigFile();
    void readStereoSGBMConfigFile();

    void stereoBM_Init();
    void stereoSGBM_Init();
    void stereoBM_GPU_Init();
    void stereoCalib();
    void setStereoBM_Params();
    void setStereoSGBM_Params();

    void setValues(int preFilterSize, int preFilterCap, int sadWindowSize, int minDisparity, int numOfDisparities, int textureThreshold, int uniquenessRatio, int speckleWindowSize, int speckleWindowRange, int disp12MaxDiff);

    void captureFrames();
    void applyRectification();
    void calculateDisparities();
    void calculate3DReconstruction();
    void saveLastFrames();
    void videoLooper();

    string imageL_filename;
    string imageR_filename;

    Mat imageL[2],imageR[2];
    cuda::GpuMat d_imageL,d_imageR,d_disp_16S;
    Mat	imageL_grey[2],imageR_grey[2];
    VideoCapture capL,capR;

    Ptr<StereoBM> bm;
    Ptr<StereoSGBM> sgbm;
    Ptr<cuda::StereoBM> bm_gpu;

    //enum {BM, SGBM, BM_GPU} method;

    StereoCalib calib;
    StereoConfig cfgBM;
    StereoConfig cfgSGBM;
    StereoDisparityMap disp;
    Reconstruction3D view3D;
    StereoDiff diff;
    StereoFlags flags;
    StereoUtils utils;
    StereoMorphology morph;

    int numRows;
    int numChannels;
    int frameCounter;

    /* Mouse Coordinates */
    int x,y;

private:
    int inputNum;
};

#endif // STEREOPROCESSOR_H