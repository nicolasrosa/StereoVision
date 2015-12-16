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



