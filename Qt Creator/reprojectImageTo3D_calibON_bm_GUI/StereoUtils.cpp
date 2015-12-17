/*
 * StereoUtils.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: nicolasrosa
 */

#include "StereoUtils.h"

/* Constructor */
StereoUtils::StereoUtils(){
    currentTime=0;
    lastTime=0;
    fps=0;
}

void StereoUtils::startClock(){
    lastTime = clock();
}

void StereoUtils::stopClock(){
    currentTime = clock();
}

void StereoUtils::showFPS(){
    fps = (int) (1000/((currentTime/1000) - lastTime)); // time stuff
    lastTime = currentTime/1000;

    cout << "FPS: " << fps << endl;
}

void StereoUtils::writeMatToFile(cv::Mat& m, const char* filename){
    ofstream fout(filename);

    if(!fout){
        cout<<"File Not Opened"<<endl;  return;
    }

    for(int i=0; i<m.rows; i++){
        for(int j=0; j<m.cols; j++){
            fout<<m.at<float>(i,j)<<"\t";
        }
        fout<<endl;
    }

    fout.close();
}

void StereoUtils::change_resolution(VideoCapture* capL,VideoCapture* capR){
#ifdef RESOLUTION_320x240
    capL->set(CV_CAP_PROP_FRAME_WIDTH, 320);
    capL->set(CV_CAP_PROP_FRAME_HEIGHT,240);
    capR->set(CV_CAP_PROP_FRAME_WIDTH, 320);
    capR->set(CV_CAP_PROP_FRAME_HEIGHT,240);
#endif

#ifdef RESOLUTION_640x480
    capL->set(CV_CAP_PROP_FRAME_WIDTH, 640);
    capL->set(CV_CAP_PROP_FRAME_HEIGHT,480);
    capR->set(CV_CAP_PROP_FRAME_WIDTH, 640);
    capR->set(CV_CAP_PROP_FRAME_HEIGHT,480);
#endif

#ifdef RESOLUTION_1280x960
    capL->set(CV_CAP_PROP_FRAME_WIDTH,1280);
    capL->set(CV_CAP_PROP_FRAME_HEIGHT,720);
    capR->set(CV_CAP_PROP_FRAME_WIDTH,1280);
    capR->set(CV_CAP_PROP_FRAME_HEIGHT,720);
#endif

    cout << "Camera 1 Resolution: " << capL->get(CV_CAP_PROP_FRAME_WIDTH) << "x" << capL->get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
    cout << "Camera 2 Resolution: " << capR->get(CV_CAP_PROP_FRAME_WIDTH) << "x" << capR->get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
}

void StereoUtils::contrast_and_brightness(Mat &left,Mat &right,float alpha,float beta){
    //Contrast and Brightness. Do the operation: new_image(i,j) = alpha*image(i,j) + beta
    for( int y = 0; y < left.rows; y++ ){
        for( int x = 0; x < left.cols; x++ ){
            for( int c = 0; c < 3; c++ ){
                left .at<Vec3b>(y,x)[c] = saturate_cast<uchar>( alpha*( left .at<Vec3b>(y,x)[c] ) + beta );
                right.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( alpha*( right.at<Vec3b>(y,x)[c] ) + beta );
            }
        }
    }
}

void StereoUtils::resizeFrames(Mat* frame1,Mat* frame2){
    if(frame1->cols != 0 || !frame2->cols != 0){
#ifdef RESOLUTION_320x240
        resize(*frame1, *frame1, Size(320,240), 0, 0, INTER_CUBIC);
        resize(*frame2, *frame2, Size(320,240), 0, 0, INTER_CUBIC);
#endif

#ifdef RESOLUTION_640x480
        resize(*frame1, *frame1, Size(640,480), 0, 0, INTER_CUBIC);
        resize(*frame2, *frame2, Size(640,480), 0, 0, INTER_CUBIC);
#endif

#ifdef RESOLUTION_1280x720
        resize(*frame1, *frame1, Size(1280,720), 0, 0, INTER_CUBIC);
        resize(*frame2, *frame2, Size(1280,720), 0, 0, INTER_CUBIC);
#endif
    }
}

