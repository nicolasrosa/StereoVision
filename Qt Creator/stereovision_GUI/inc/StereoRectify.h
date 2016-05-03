/*
 * StereoRectify.h
 *
 *  Created on: May 2, 2016
 *      Author: nicolasrosa
 */

#ifndef STEREORECTIFY_H
#define STEREORECTIFY_H

/* Libraries */
#include <opencv2/opencv.hpp>
#include <inc/StereoCalib.h>

using namespace cv;
using namespace std;

class StereoRectify{
public:
    /* Constructor and Destructor */
    StereoRectify();
    ~StereoRectify();

    void initRectification(StereoCalib *ptr);
    void applyRectification(Mat *imageL,Mat *imageR);

    StereoCalib *calib_ptr;

private:
    /* Results */
    Mat imageLr,imageRr;
};

#endif // STEREORECTIFY_H
