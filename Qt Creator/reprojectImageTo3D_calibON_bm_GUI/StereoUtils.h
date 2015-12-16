/*
 * StereoUtils.h
 *
 *  Created on: Dec 16, 2015
 *      Author: nicolasrosa
 */

#ifndef STEREOUTILS_H
#define STEREOUTILS_H

#include <opencv2/opencv.hpp>
#include "time.h"

using namespace cv;
using namespace std;

class StereoUtils{
public:
    StereoUtils(); //Constructor

    /* Timing */
    void startClock();
    void stopClock();
    void showFPS();

private:
    int currentTime;
    int lastTime;
    int fps;
};

#endif // STEREOUTILS_H
