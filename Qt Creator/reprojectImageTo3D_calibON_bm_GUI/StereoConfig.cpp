/*
 * StereoConfig.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: nicolasrosa
 */

#include "StereoConfig.h"

/* Constructor */
StereoConfig::StereoConfig(){}

void StereoConfig::showConfigValues(){
    cout << "------------------------------" << this->methodName << " Config----------------------------" << endl;
    cout << "preFilterSize: "       << this->preFilterSize          << endl;
    cout << "preFilterCap: "        << this->preFilterCap           << endl;
    cout << "SADWindowSize: "       << this->SADWindowSize          << endl;
    cout << "minDisparity: "        << this->minDisparity           << endl;
    cout << "numberOfDisparities: " << this->numberOfDisparities    << endl;
    cout << "textureThreshold: "    << this->textureThreshold       << endl;
    cout << "uniquenessRatio: "     << this->uniquenessRatio        << endl;
    cout << "speckleWindowSize: "   << this->speckleWindowSize      << endl;
    cout << "speckleRange: "        << this->speckleRange           << endl;
    cout << "disp12MaxDiff: "       << this->disp12MaxDiff          << endl;
}
