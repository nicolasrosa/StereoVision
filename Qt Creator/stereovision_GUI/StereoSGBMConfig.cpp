/*
 * StereoSGBMConfig.cpp
 *
 *  Created on: Dec 17, 2015
 *      Author: nicolasrosa
 */

#include "StereoSGBMConfig.h"

/* Constructor */
StereoSGBMConfig::StereoSGBMConfig(){}

void StereoSGBMConfig::showConfigValues(){
    cout << "------------------------------StereoSGBM Config----------------------------" << endl;
    cout << "preFilterSize: "       << this->stereoSGBMcfg.preFilterSize          << endl;
    cout << "preFilterCap: "        << this->stereoSGBMcfg.preFilterCap           << endl;
    cout << "SADWindowSize: "       << this->stereoSGBMcfg.SADWindowSize          << endl;
    cout << "minDisparity: "        << this->stereoSGBMcfg.minDisparity           << endl;
    cout << "numberOfDisparities: " << this->stereoSGBMcfg.numberOfDisparities    << endl;
    cout << "textureThreshold: "    << this->stereoSGBMcfg.textureThreshold       << endl;
    cout << "uniquenessRatio: "     << this->stereoSGBMcfg.uniquenessRatio        << endl;
    cout << "speckleWindowSize: "   << this->stereoSGBMcfg.speckleWindowSize      << endl;
    cout << "speckleRange: "        << this->stereoSGBMcfg.speckleRange           << endl;
    cout << "disp12MaxDiff: "       << this->stereoSGBMcfg.disp12MaxDiff          << endl;
    cout << "stereoSGBM.yml Read Successfully."  << endl << endl;
    cout << "----------------------------------------------------------------------" << endl << endl;
}
