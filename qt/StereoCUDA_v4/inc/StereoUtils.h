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
class Timer{
public:
    /* Constructor and Destructor */
    Timer();
    ~Timer();

    /* Timing */
    void startClock();
    void stopClock();
    void calculateFPS();
    void calculateElapsedTime();
    void printElapsedTime();
    void printFPS();
    double getFPS() const;

    double clockInitial;
    double clockFinal;

    double d;
    double f;
    double elapsedTime;

private:
    double fps;
};

class Sleeper : public QThread{
public:
    /* Delay */
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
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
}

#endif // STEREOUTILS_H
