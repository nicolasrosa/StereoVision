/*
 * StereoConfigBM.cpp
 *
 *  Created on: May 4, 2015
 *      Author: nicolasrosa
 */

/* Libraries */
#include "inc/StereoConfigBM.h"

/* Constructor and Destructor */
StereoConfigBM::StereoConfigBM(){}

StereoConfigBM::~StereoConfigBM(){}

void StereoConfigBM::showConfigValues(){
    cout << "--------------------------------- " << methodName << " Config ------------------------------\n"
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
         << "stereoBM.yml Read Successfully.\n\n";
}


void StereoConfigBM::readStereoBMConfigFile(){
    cv::FileStorage fs(StereoBMConfigFileName, FileStorage::READ);
    if(!fs.isOpened()){
        cerr << "Failed to open stereoBM.yml file!" << endl;
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
