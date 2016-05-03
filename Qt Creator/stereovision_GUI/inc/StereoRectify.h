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
    StereoRectify(StereoCalib *ptr,Mat *ptrL,Mat *ptrR);
    ~StereoRectify();

    void initRectification();
    void applyRectification();

    Mat getImageLr();
    Mat getImageRr();
//    Mat getImageLrc();
//    Mat getImageRrc();

    StereoCalib *calib_ptr;
    Mat *imageL_ptr;
    Mat *imageR_ptr;

private:
    enum{LEFT,RIGHT};
    enum{X,Y};

    Mat rmap[2][2];

    /* Results */
    Mat imageLr,imageRr;

    //TODO: Ativar caso deseja-se utilizar 'Cropped Images'
    //Mat imageLrc,imageRrc;
};

#endif // STEREORECTIFY_H
