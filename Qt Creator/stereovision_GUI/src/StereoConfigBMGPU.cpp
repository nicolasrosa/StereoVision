/*
 * StereoConfigBMGPU.cpp
 *
 *  Created on: May 4, 2015
 *      Author: nicolasrosa
 */

/* Libraries */
#include "inc/StereoConfigBMGPU.h"

/* Constructor and Destructor */
StereoConfigBMGPU::StereoConfigBMGPU(){}

StereoConfigBMGPU::~StereoConfigBMGPU(){}

void StereoConfigBMGPU::showConfigValues(){
    cout << "--------------------------------- " << methodName << " Config --------------------------\n"
         << "preFilterSize: "       << preFilterSize          << "\n"
         << "preFilterCap: "        << preFilterCap           << "\n"
         << "SADWindowSize: "       << SADWindowSize          << "\n"
         << "minDisparity: "        << minDisparity           << "\n"
         << "numberOfDisparities: " << numberOfDisparities    << "\n"
         << "textureThreshold: "    << textureThreshold       << "\n"
         << "uniquenessRatio: "     << uniquenessRatio        << "\n"
         << "speckleWindowSize: "   << speckleWindowSize      << "\n"
         << "speckleRange: "        << speckleRange           << "\n"
         << "disp12MaxDiff: "       << disp12MaxDiff          << "\n\n"
         << "stereoBMGPU.yml Read Successfully.\n\n";
}


void StereoConfigBMGPU::readStereoBM_GPUConfigFile(){
    FileStorage fs(StereoBM_GPUConfigFileName, FileStorage::READ);
    if(!fs.isOpened()){
        cerr << "Failed to open stereoBM_GPU.yml file!" << endl;
        return;
    }

    fs["methodName"] >> methodName;
    fs["preFilterSize"] >> preFilterSize;
    fs["preFilterCap"] >> preFilterCap;
    fs["SADWindowSize"] >> SADWindowSize;
    fs["minDisparity"] >> minDisparity;
    fs["numberOfDisparities"] >> numberOfDisparities;
    fs["textureThreshold"] >> textureThreshold;
    fs["uniquenessRatio"] >> uniquenessRatio;
    fs["speckleWindowSize"] >> speckleWindowSize;
    fs["speckleRange"] >> speckleRange;
    fs["disp12MaxDiff"] >> disp12MaxDiff;

    fs.release();

    // Display
    showConfigValues();
}
