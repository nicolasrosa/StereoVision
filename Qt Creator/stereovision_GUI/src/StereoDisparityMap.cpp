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

//void StereoDisparityMap::calculateDisparities(){
//    /* Convert BGR images to Grey Scale */
//    if(method == StereoProcessor::BM || method == StereoProcessor::BM_GPU){
//        cvtColor(imageL[0],imageL_grey[0],CV_BGR2GRAY);
//        cvtColor(imageR[0],imageR_grey[0],CV_BGR2GRAY);
//    }

//    /* Computing Disparities - Disparity Map */
//    //#define MEASURE_TIME_BM
//    //#define MEASURE_TIME_SGBM
//    //#define MEASURE_TIME_BMGPU

//    switch(method){
//        case StereoProcessor::BM:
//            #ifdef MEASURE_TIME_BM
//                    //! START CLOCK !//
//                    StereoUtils::Timer::startClock(&time.clockInitial_d);
//                    bm->compute(imageL_grey[0],imageR_grey[0],disp.disp_16S);
//                    StereoUtils::Timer::stopClock(&time.clockFinal_d);
//                    //! STOP CLOCK !//

//                    StereoUtils::Timer::printElapsedTime(time.clockInitial_d,time.clockFinal_d);
//            #else
//                    bm->compute(imageL_grey[0],imageR_grey[0],disp.disp_16S);
//            #endif
//            break;
//        case StereoProcessor::SGBM:
//            #ifdef MEASURE_TIME_SGBM
//                    //! START CLOCK !//
//                    StereoUtils::Timer::startClock(&time.clockInitial_d);
//                    sgbm->compute(imageL[0],imageR[0],disp.disp_16S);
//                    StereoUtils::Timer::stopClock(&time.clockFinal_d);
//                    //! STOP CLOCK !//

//                    StereoUtils::Timer::printElapsedTime(time.clockInitial_d,time.clockFinal_d);
//            #else
//                    sgbm->compute(imageL[0],imageR[0],disp.disp_16S);
//            #endif
//            break;
//        case StereoProcessor::BM_GPU:
//            #ifdef MEASURE_TIME_BMGPU
//                    //! START CLOCK !//
//                    StereoUtils::Timer::startClock(&time.clockInitial_d);
//                    d_imageL.upload(imageL_grey[0]);
//                    d_imageR.upload(imageR_grey[0]);
//                    bm_gpu->compute(d_imageL,d_imageR,d_disp_16S);
//                    d_disp_16S.download(disp.disp_16S);
//                    StereoUtils::Timer::stopClock(&time.clockFinal_d);
//                    //! STOP CLOCK !//

//                    StereoUtils::Timer::printElapsedTime(time.clockInitial_d,time.clockFinal_d);
//            #else
//                    d_imageL.upload(imageL_grey[0]);
//                    d_imageR.upload(imageR_grey[0]);
//                    bm_gpu->compute(d_imageL,d_imageR,d_disp_16S);
//                    d_disp_16S.download(disp.disp_16S);
//            #endif
//            break;
//    }

//    normalize(disp.disp_16S, disp.disp_8U, 0, 255, CV_MINMAX, CV_8U);
//    //disp.disp_16S.convertTo(disp.disp_8U, CV_8U, 255/(BMcfg.numberOfDisparities*16.));

//    applyColorMap(disp.disp_8U,disp.disp_BGR, COLORMAP_JET);

//    if(flags.showDispDepth){
//        disp.computeDispDepthInformation();
//    }
//}

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
