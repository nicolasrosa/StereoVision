/*
 * StereoUtils.h
 *
 *  Created on: Dec 16, 2015
 *      Author: nicolasrosa
 */

#ifndef STEREOUTILS_H
#define STEREOUTILS_H

/* Libraries */
#include <QThread>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <time.h>

using namespace cv;
using namespace std;

namespace StereoTime{
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
    void startClock(int64 *initial);
    void stopClock(int64 *final);
    void calculateFPS();
    void printElapsedTime(int64 initial,int64 final);
    void showFPS();
    int getFPS();

    int64 clockInitial;
    int64 clockFinal;

    /* Elapsed time calculating the disparities correspondenses */
    int64 clockInitial_d;
    int64 clockFinal_d;

    int64 d;
    double f;

private:
//    struct timespec start,end;
    double fps;
};

/* Delay */
class Sleeper : public QThread{
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};
}

#endif // STEREOUTILS_H
