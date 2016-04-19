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
#include "MainWindow.h"
#include "Reconstruction3D.h"

#include "StereoInput.h"
#include "StereoCalib.h"
#include "StereoCustom.h"
#include "StereoConfig.h"
#include "StereoDiff.h"
#include "StereoDisparityMap.h"
#include "StereoFlags.h"
#include "StereoMorphology.h"
#include "StereoUtils.h"

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
    void readStereoBM_GPUConfigFile();

    void stereoBM_Init();
    void stereoSGBM_Init();
    void stereoBM_GPU_Init();

    void setStereoBM_Params();
    void setStereoSGBM_Params();
    void setStereoBM_GPU_Params();

    void setValues(int preFilterSize, int preFilterCap, int sadWindowSize, int minDisparity, int numOfDisparities, int textureThreshold, int uniquenessRatio, int speckleWindowSize, int speckleWindowRange, int disp12MaxDiff);

    void setNumRows(int value);
    void setNumChannels(int value);
    int getNumRows();
    int getNumChannels();

    void captureFrames();
    void applyRectification();
    void calculateDisparities();
    void calculateTrueMap();
    void calculate3DReconstruction();
    void saveLastFrames();
    void videoLooper();

    Mat imageL[2],imageR[2];
    cuda::GpuMat d_imageL,d_imageR,d_disp_16S;
    Mat	imageL_grey[2],imageR_grey[2];
    VideoCapture capL,capR;

    /* Stereo Methods */
    /* BM */
    Ptr<StereoBM> bm;
    StereoConfig cfgBM;

    /* SGBM */
    Ptr<StereoSGBM> sgbm;
    StereoConfig cfgSGBM;

    /* BM_GPU */
    Ptr<cuda::StereoBM> bm_gpu;
    StereoConfig cfgBM_GPU;

    enum {BM, SGBM, BM_GPU} method;

    //StereoInput input;
    StereoCalib calib;
    StereoDisparityMap disp;
    Reconstruction3D view3D;
    StereoDiff diff;
    StereoFlags flags;
    StereoUtils utils;
    StereoMorphology morph;


    int frameCounter;

    /* Mouse Coordinates */
    int x,y;

    /* Result */
    Mat true_dmap;

private:
    int inputNum;
    int numRows;
    int numChannels;
};

#endif // STEREOPROCESSOR_H
