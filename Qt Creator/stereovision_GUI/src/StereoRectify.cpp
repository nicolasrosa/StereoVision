/*
 * StereoRectify.cpp
 *
 *  Created on: May 2, 2016
 *      Author: nicolasrosa
 */

/* Libraries */
#include "inc/StereoRectify.h"

/* Constructor and Destructor */
StereoRectify::StereoRectify(){}


StereoRectify::~StereoRectify(){}

// TODO: Decidir onde ativar as linhas "initUndistortRectifyMap...".
//1) Rápido(~40 fps)
//2) Normal(~25 fps)

void StereoRectify::initRectification(StereoCalib *ptr){
    calib_ptr = ptr;

    /* Debug */
    //cout << "Res: " << calib_ptr->getResolution() << endl;
    //cout << "ResD:" << calib_ptr->getResolutionDesired() << endl;

    stereoRectify(calib_ptr->M1,calib_ptr->D1,calib_ptr->M2,calib_ptr->D2,calib_ptr->getResolutionDesired(),calib_ptr->R,calib_ptr->T,calib_ptr->R1,calib_ptr->R2,calib_ptr->P1,calib_ptr->P2,calib_ptr->Q,CALIB_ZERO_DISPARITY,-1,calib_ptr->getResolutionDesired(),&calib_ptr->roi1,&calib_ptr->roi2);

    //initUndistortRectifyMap(calib_ptr->M1, calib_ptr->D1, calib_ptr->R1, calib_ptr->P1, calib_ptr->getResolutionDesired(), CV_16SC2, calib_ptr->rmap[0][0], calib_ptr->rmap[0][1]);
    //initUndistortRectifyMap(calib_ptr->M2, calib_ptr->D2, calib_ptr->R2, calib_ptr->P2, calib_ptr->getResolutionDesired(), CV_16SC2, calib_ptr->rmap[1][0], calib_ptr->rmap[1][1]);
}


void StereoRectify::applyRectification(Mat *imageL,Mat *imageR){
    initUndistortRectifyMap(calib_ptr->M1, calib_ptr->D1, calib_ptr->R1, calib_ptr->P1, calib_ptr->getResolutionDesired(), CV_16SC2, calib_ptr->rmap[0][0], calib_ptr->rmap[0][1]);
    initUndistortRectifyMap(calib_ptr->M2, calib_ptr->D2, calib_ptr->R2, calib_ptr->P2, calib_ptr->getResolutionDesired(), CV_16SC2, calib_ptr->rmap[1][0], calib_ptr->rmap[1][1]);

    remap(*imageL, imageLr, calib_ptr->rmap[0][0], calib_ptr->rmap[0][1], INTER_LINEAR);
    remap(*imageR, imageRr, calib_ptr->rmap[1][0], calib_ptr->rmap[1][1], INTER_LINEAR);

    *imageL = imageLr;
    *imageR = imageRr;
}
