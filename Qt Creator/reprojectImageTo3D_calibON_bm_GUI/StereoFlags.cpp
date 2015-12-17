/*
 * StereoFlags.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: nicolasrosa
 */

#include "StereoFlags.h"

/* Constructor */
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

    methodBM = true;
    methodSGBM = false;
}
