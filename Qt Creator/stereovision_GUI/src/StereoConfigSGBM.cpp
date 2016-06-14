/*
 * StereoConfigSGBM.cpp
 *
 *  Created on: May 4, 2015
 *      Author: nicolasrosa
 */

/* Libraries */
#include "inc/StereoConfigSGBM.h"

/* Constructor and Destructor */
StereoConfigSGBM::StereoConfigSGBM(){}

StereoConfigSGBM::~StereoConfigSGBM(){}

void StereoConfigSGBM::showConfigValues(){
    cout << "--------------------------------- " << methodName << " Config ----------------------------\n"
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
         << "stereoSGBM.yml Read Successfully.\n\n";
}


void StereoConfigSGBM::readStereoSGBMConfigFile(){
    FileStorage fs(StereoSGBMConfigFileName, FileStorage::READ);
    if(!fs.isOpened()){
        cerr << "Failed to open stereoSGBM.yml file!" << endl;
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
