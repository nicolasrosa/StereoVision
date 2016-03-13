/*
 * StereoFlags.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: nicolasrosa
 */

/* Libraries */
#include "inc/StereoFlags.h"

/* Constructor and Destructor */
StereoFlags::StereoFlags(){
    showInputImages=true;
    showXYZ=false;
    showStereoParam=false;
    showStereoParamValues=false;
    showFPS=false;
    showDisparityMap=false;
    show3Dreconstruction=false;
    showTrackingObjectView=false;
    showDiffImage=false;
    showWarningLines=false;
    showHistograms=false;
    showDispDepth=false;

    methodBM = true;
    methodSGBM = false;
    methodBM_GPU = false;
}

StereoFlags::~StereoFlags(){}
