/*
 * StereoRectify.cpp
 *
 *  Created on: May 2, 2016
 *      Author: nicolasrosa
 */

/* Libraries */
#include "inc/StereoRectify.h"

/* Constructor and Destructor */
StereoRectify::StereoRectify(StereoCalib *ptr,Mat *ptrL,Mat *ptrR){
    /* Linking Pointers */
    calib_ptr = ptr;
    imageL_ptr = ptrL;
    imageR_ptr = ptrR;

    //TODO: Alocar memória após a variável 'imageL[0]' já tiver sido iniciada.
    /* Allocating Memory - Rectified Images */
    //imageLr = new Mat(imageL_ptr->rows,imageL_ptr->cols,CV_8UC1);
    //imageRr = new Mat(imageL_ptr->rows,imageL_ptr->cols,CV_8UC1);

    //TODO: Alocar memória após as variáveis ROI1 e ROI2 já tivere, sido iniciadas.
    //Mat imageLrc(calib_ptr->roi1.height,calib_ptr->roi1.width,CV_8UC1);
    //Mat imageRrc(calib_ptr->roi1.height,calib_ptr->roi1.width,CV_8UC1);	/// Cropped to roi1 & roi2
}


StereoRectify::~StereoRectify(){}

// TODO: Decidir onde ativar as linhas "initUndistortRectifyMap...".
//1) Rápido(~40 fps)
//2) Normal(~25 fps)
void StereoRectify::initRectification(){
    /* Debug */
    //cout << "Res: " << calib_ptr->getResolution() << endl;
    //cout << "ResD:" << calib_ptr->getResolutionDesired() << endl;

    stereoRectify(calib_ptr->M1,calib_ptr->D1,calib_ptr->M2,calib_ptr->D2,calib_ptr->getResolutionDesired(),calib_ptr->R,calib_ptr->T,calib_ptr->R1,calib_ptr->R2,calib_ptr->P1,calib_ptr->P2,calib_ptr->Q,CALIB_ZERO_DISPARITY,-1,calib_ptr->getResolutionDesired(),&calib_ptr->roi1,&calib_ptr->roi2);

    //initUndistortRectifyMap(calib_ptr->M1, calib_ptr->D1, calib_ptr->R1, calib_ptr->P1, calib_ptr->getResolutionDesired(), CV_16SC2, calib_ptr->rmap[0][0], calib_ptr->rmap[0][1]);
    //initUndistortRectifyMap(calib_ptr->M2, calib_ptr->D2, calib_ptr->R2, calib_ptr->P2, calib_ptr->getResolutionDesired(), CV_16SC2, calib_ptr->rmap[1][0], calib_ptr->rmap[1][1]);
}


void StereoRectify::applyRectification(){
    initUndistortRectifyMap(calib_ptr->M1, calib_ptr->D1, calib_ptr->R1, calib_ptr->P1, calib_ptr->getResolutionDesired(), CV_16SC2, rmap[LEFT][X], rmap[LEFT][Y]);
    initUndistortRectifyMap(calib_ptr->M2, calib_ptr->D2, calib_ptr->R2, calib_ptr->P2, calib_ptr->getResolutionDesired(), CV_16SC2, rmap[RIGHT][X], rmap[RIGHT][Y]);
    
    remap(*imageL_ptr, imageLr, rmap[LEFT][X] , rmap[LEFT][Y], INTER_LINEAR);	/// Do remapping of imageL to imageLr using map11, map12
    remap(*imageR_ptr, imageRr, rmap[RIGHT][X], rmap[RIGHT][Y], INTER_LINEAR);	/// Do remapping of imageR to imageRr using map21, map22

    //TODO: In case of utilizing the cropped images, check if the cropped images have the same size!
//    imageLr(calib_ptr->roi1).convertTo(imageLrc,imageLr.type(),1,0);	/// Crop imageLr to imageLrc using roi1
//    imageRr(calib_ptr->roi1).convertTo(imageRrc,imageLr.type(),1,0);	/// Crop imageRr to imageRrc using roi1
//    imageRr(calib_ptr->roi2).convertTo(imageRrc,imageLr.type(),1,0);	/// Crop imageRr to imageRrc using roi2

    *imageL_ptr = imageLr;
    *imageR_ptr = imageRr;
}

Mat StereoRectify::getImageLr(){
    return(imageLr);
}

Mat StereoRectify::getImageRr(){
    return(imageRr);
}

//Mat StereoRectify::getImageLrc(){
//    return(imageLrc);
//}

//Mat StereoRectify::getImageRrc(){
//    return(imageRrc);
//}


