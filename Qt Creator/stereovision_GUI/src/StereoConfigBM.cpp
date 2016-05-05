/*
 * StereoConfigBM.cpp
 *
 *  Created on: May 4, 2015
 *      Author: nicolasrosa
 */

/* Libraries */
#include "StereoConfigBM.h"

/* Constructor and Destructor */
StereoConfigBM::StereoConfigBM(){}

StereoConfigBM::~StereoConfigBM(){}

void StereoConfigBM::showConfigValues(){
    cout << "------------------------------" << methodName << " Config----------------------------" << endl;
    cout << "preFilterSize: "       << preFilterSize          << endl;
    cout << "preFilterCap: "        << preFilterCap           << endl;
    cout << "SADWindowSize: "       << SADWindowSize          << endl;
    cout << "minDisparity: "        << minDisparity           << endl;
    cout << "numberOfDisparities: " << numberOfDisparities    << endl;
    cout << "textureThreshold: "    << textureThreshold       << endl;
    cout << "uniquenessRatio: "     << uniquenessRatio        << endl;
    cout << "speckleWindowSize: "   << speckleWindowSize      << endl;
    cout << "speckleRange: "        << speckleRange           << endl;
    cout << "disp12MaxDiff: "       << disp12MaxDiff          << endl;
    cout << "stereoBM.yml Read Successfully."  << endl << endl;
    cout << "----------------------------------------------------------------------" << endl << endl;
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
    cout << "stereoBM.yml Read Successfully."  << endl << endl;
    //cout << "----------------------------------------------------------------------" << endl << endl;
}
