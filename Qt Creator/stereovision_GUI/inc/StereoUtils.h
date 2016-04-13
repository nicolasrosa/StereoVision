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

using namespace cv;
using namespace std;

class StereoUtils{
public:
    /* Constructor and Destructor */
    StereoUtils();
    ~StereoUtils();

    void resizeFrames(Mat *frame1,Mat *frame2,Size resolution);
    void change_resolution(VideoCapture* capL,VideoCapture* capR,Size resolution);
    void contrast_and_brightness(Mat &left,Mat &right,float alpha,float beta);
    void writeMatToFile(Mat &m, const char* filename);
    void calculateHist(Mat &src, const string histName);
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
