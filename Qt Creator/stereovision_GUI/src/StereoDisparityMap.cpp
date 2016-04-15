/*
 * StereoDisparityMap.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: nicolasrosa
 */

/* Libraries */
#include "inc/StereoDisparityMap.h"

/* Constructor and Destructor */
StereoDisparityMap::StereoDisparityMap(){
    // Allocate Memory
    //Mat disp     = Mat(imageR[0].rows, imageR[0].cols, CV_16UC1);
    //Mat disp_8U  = Mat(stereo->imageR[0].rows, stereo->imageR[0].cols, CV_8UC1);
    //Mat disp_BGR = Mat(stereo->imageR[0].rows, stereo->imageR[0].cols, CV_8UC3);
}

StereoDisparityMap::~StereoDisparityMap(){}


//TODO: void StereoDisparityMap::computeDispDepthInformation(){
//    /* Forces the initialization of the 3DReconstruction Method for obtaining the Depth Values */
//    cv::reprojectImageTo3D(disp_16S,view3D.depth,calib.Q);

//    float disparity = disp.disp_16S.at<uchar>(y,x);
//    float depth = view3D.depth.at<Vec3f>(y,x)[2];

//    /* Draw White Cross */
//    circle(disp.disp_8U,Point(x,y),10,Scalar(255,255,255),2);
//    if(y-25>0)
//        line(disp.disp_8U,Point(x,y-5),Point(x,y-15),Scalar(255,255,255),2);
//    else line(disp.disp_8U,Point(x,y),Point(x,0),Scalar(255,255,255),2);
//    if(y+25<480)
//        line(disp.disp_8U,Point(x,y+5),Point(x,y+15),Scalar(255,255,255),2);
//    else line(disp.disp_8U,Point(x,y),Point(x,480),Scalar(255,255,255),2);
//    if(x-25>0)
//        line(disp.disp_8U,Point(x-5,y),Point(x-15,y),Scalar(255,255,255),2);
//    else line(disp.disp_8U,Point(x,y),Point(0,y),Scalar(255,255,255),2);
//    if(x+25<640)
//        line(disp.disp_8U,Point(x+5,y),Point(x+15,y),Scalar(255,255,255),2);
//    else line(disp.disp_8U,Point(x,y),Point(640,y),Scalar(255,255,255),2);

//    /* CrossHair Information */
//    putText(disp.disp_8U,utils.intToString(disparity),Point(x,y-20),1,1,Scalar(255,255,255),2);
//    //putText(disp.disp_8U,utils.intToString((int)depth),Point(x,y-20),1,1,Scalar(255,255,255),2);
//    putText(disp.disp_8U,utils.intToString(x)+","+utils.intToString(y),Point(x,y+30),1,1,Scalar(255,255,255),2);

//    cout << "P(" << x << "," << y << ")"<< "\t" << "Disparity: " << disparity << "\t" << "Depth: " << depth << endl;
//}
