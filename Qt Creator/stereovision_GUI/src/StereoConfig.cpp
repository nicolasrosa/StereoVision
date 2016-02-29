/*
 * StereoConfig.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: nicolasrosa
 */

#include "inc/StereoConfig.h"

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


// TODO: Implementar SGBM Configuration
/// *
// * StereoSGBMConfig.cpp
// *
// *  Created on: Dec 17, 2015
// *      Author: nicolasrosa
// */

//#include "StereoSGBMConfig.h"

///* Constructor */
//StereoSGBMConfig::StereoSGBMConfig(){}

//void StereoSGBMConfig::showConfigValues(){
//    cout << "------------------------------StereoSGBM Config----------------------------" << endl;
//    cout << "preFilterSize: "       << this->stereoSGBMcfg.preFilterSize          << endl;
//    cout << "preFilterCap: "        << this->stereoSGBMcfg.preFilterCap           << endl;
//    cout << "SADWindowSize: "       << this->stereoSGBMcfg.SADWindowSize          << endl;
//    cout << "minDisparity: "        << this->stereoSGBMcfg.minDisparity           << endl;
//    cout << "numberOfDisparities: " << this->stereoSGBMcfg.numberOfDisparities    << endl;
//    cout << "textureThreshold: "    << this->stereoSGBMcfg.textureThreshold       << endl;
//    cout << "uniquenessRatio: "     << this->stereoSGBMcfg.uniquenessRatio        << endl;
//    cout << "speckleWindowSize: "   << this->stereoSGBMcfg.speckleWindowSize      << endl;
//    cout << "speckleRange: "        << this->stereoSGBMcfg.speckleRange           << endl;
//    cout << "disp12MaxDiff: "       << this->stereoSGBMcfg.disp12MaxDiff          << endl;
//    cout << "stereoSGBM.yml Read Successfully."  << endl << endl;
//    cout << "----------------------------------------------------------------------" << endl << endl;
//}
