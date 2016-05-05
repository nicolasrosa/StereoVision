/*
 * StereoDisparityMap.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: nicolasrosa
 */

/* Libraries */
#include "inc/StereoDisparityMap.h"

/* Constructor and Destructor */
StereoDisparityMap::StereoDisparityMap(Reconstruction3D *ptr3D,StereoCalib *ptrC,int *ptrX,int *ptrY){
    /* Linking Pointers */
    view3D_ptr = ptr3D;
    calib_ptr = ptrC;
    x_ptr = ptrX;
    y_ptr = ptrY;

    // TODO: Allocate Memory
    //Mat disp     = Mat(imageR[0].rows, imageR[0].cols, CV_16UC1);
    //Mat disp_8U  = Mat(stereo->imageR[0].rows, stereo->imageR[0].cols, CV_8UC1);
    //Mat disp_BGR = Mat(stereo->imageR[0].rows, stereo->imageR[0].cols, CV_8UC3);
}

StereoDisparityMap::~StereoDisparityMap(){}


void StereoDisparityMap::computeDispDepthInformation(){
    /* Forces the initialization of the 3DReconstruction Method for obtaining the Depth Values */
    cv::reprojectImageTo3D(disp_16S,view3D_ptr->depth,calib_ptr->Q);

    float disparity = disp_16S.at<uchar>(*y_ptr,*x_ptr);
    float depth = view3D_ptr->depth.at<Vec3f>(*y_ptr,*x_ptr)[2];

    /* Draw White Cross */
    circle(disp_8U,Point(*x_ptr,*y_ptr),10,Scalar(255,255,255),2);
    if(*y_ptr-25>0)
        line(disp_8U,Point(*x_ptr,*y_ptr-5),Point(*x_ptr,*y_ptr-15),Scalar(255,255,255),2);
    else line(disp_8U,Point(*x_ptr,*y_ptr),Point(*x_ptr,0),Scalar(255,255,255),2);
    if(*y_ptr+25<480)
        line(disp_8U,Point(*x_ptr,*y_ptr+5),Point(*x_ptr,*y_ptr+15),Scalar(255,255,255),2);
    else line(disp_8U,Point(*x_ptr,*y_ptr),Point(*x_ptr,480),Scalar(255,255,255),2);
    if(*x_ptr-25>0)
        line(disp_8U,Point(*x_ptr-5,*y_ptr),Point(*x_ptr-15,*y_ptr),Scalar(255,255,255),2);
    else line(disp_8U,Point(*x_ptr,*y_ptr),Point(0,*y_ptr),Scalar(255,255,255),2);
    if(*x_ptr+25<640)
        line(disp_8U,Point(*x_ptr+5,*y_ptr),Point(*x_ptr+15,*y_ptr),Scalar(255,255,255),2);
    else line(disp_8U,Point(*x_ptr,*y_ptr),Point(640,*y_ptr),Scalar(255,255,255),2);

    /* CrossHair Information */
    putText(disp_8U,StereoUtils::Extras::intToString(disparity),Point(*x_ptr,*y_ptr-20),1,1,Scalar(255,255,255),2);
    //putText(disp_8U,utils.intToString((int)depth),Point(*x_ptr,*y_ptr-20),1,1,Scalar(255,255,255),2);
    putText(disp_8U,StereoUtils::Extras::intToString(*x_ptr)+","+StereoUtils::Extras::intToString(*y_ptr),Point(*x_ptr,*y_ptr+30),1,1,Scalar(255,255,255),2);

    cout << "P(" << *x_ptr << "," << *y_ptr << ")"<< "\t" << "Disparity: " << disparity << "\t" << "Depth: " << depth << endl;
}
