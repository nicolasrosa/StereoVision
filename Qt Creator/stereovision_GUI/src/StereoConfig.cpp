/*
 * StereoConfig.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: nicolasrosa
 */

/* Libraries */
#include "inc/StereoConfig.h"

/* Constructor and Destructor */
StereoConfig::StereoConfig(){}

void StereoConfig::showConfigValues(){
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
}

StereoConfig::~StereoConfig(){}


// TODO: Implementar SGBM Configuration
/// *
// * StereoSGBMConfig.cpp
// *
// *  Created on: Dec 17, 2015
// *      Author: nicolasrosa
// */

//#include "StereoSGBMConfig.h"

//StereoSGBMConfig::StereoSGBMConfig(){}

//void StereoSGBMConfig::showConfigValues(){
//    cout << "------------------------------StereoSGBM Config----------------------------" << endl;
//    cout << "preFilterSize: "       << stereoSGBMcfg.preFilterSize          << endl;
//    cout << "preFilterCap: "        << stereoSGBMcfg.preFilterCap           << endl;
//    cout << "SADWindowSize: "       << stereoSGBMcfg.SADWindowSize          << endl;
//    cout << "minDisparity: "        << stereoSGBMcfg.minDisparity           << endl;
//    cout << "numberOfDisparities: " << stereoSGBMcfg.numberOfDisparities    << endl;
//    cout << "textureThreshold: "    << stereoSGBMcfg.textureThreshold       << endl;
//    cout << "uniquenessRatio: "     << stereoSGBMcfg.uniquenessRatio        << endl;
//    cout << "speckleWindowSize: "   << stereoSGBMcfg.speckleWindowSize      << endl;
//    cout << "speckleRange: "        << stereoSGBMcfg.speckleRange           << endl;
//    cout << "disp12MaxDiff: "       << stereoSGBMcfg.disp12MaxDiff          << endl;
//    cout << "stereoSGBM.yml Read Successfully."  << endl << endl;
//    cout << "----------------------------------------------------------------------" << endl << endl;
//}
