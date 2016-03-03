/*
 * StereoUtils.h
 *
 *  Created on: Dec 16, 2015
 *      Author: nicolasrosa
 */

#ifndef STEREOUTILS_H
#define STEREOUTILS_H

#include <opencv2/opencv.hpp>
#include <fstream>
#include <time.h>

/* Resolution */
//#define RESOLUTION_320x240
#define RESOLUTION_640x480
//#define RESOLUTION_1280x720

using namespace cv;
using namespace std;

class StereoUtils{
public:
    /* Constructor */
    StereoUtils();
    void resizeFrames(Mat *frame1,Mat *frame2);
    void change_resolution(VideoCapture* capL,VideoCapture* capR);
    void contrast_and_brightness(Mat &left,Mat &right,float alpha,float beta);
    void writeMatToFile(Mat &m, const char* filename);
    void calculateHist(Mat &src, const string histName);
    void setDisplayFPS();
    string intToString(int number);

    /* Timing */
    void startClock();
    void stopClock();
    void showFPS();
    int getFPS();

    int64 clockInitial;
    int64 d;
    double f;
    double fps;

private:
//    struct timespec start,end;
//    int fps;



};

#endif // STEREOUTILS_H
