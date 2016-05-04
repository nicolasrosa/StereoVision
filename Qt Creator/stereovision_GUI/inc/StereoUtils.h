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

namespace StereoUtils{
class Time{
public:
    /* Constructor and Destructor */
    Time();
    ~Time();

    /* Timing */
    static void startClock(int64 *initial);
    static void stopClock(int64 *final);
    void calculateFPS();
    static void printElapsedTime(int64 initial,int64 final);
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

class Resizer{
public:
    static void resizeFrames(Mat *frame1,Mat *frame2,Size resolution);
    static void changeResolution(VideoCapture* capL,VideoCapture* capR,Size resolution);
private:
};

class Extras{
public:
    static void calculateHist(Mat &src, const string histName);
    void contrast_and_brightness(Mat &left,Mat &right,float alpha,float beta);

    void writeMatToFile(Mat &m, const char* filename);

    static string intToString(int number);
private:
};

class Sleeper : public QThread{
public:
    /* Delay */
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};
}

#endif // STEREOUTILS_H
