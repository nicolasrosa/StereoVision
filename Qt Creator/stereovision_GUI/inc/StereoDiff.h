/*
 * StereoDiff.h
 *
 *  Created on: Dec 3, 2015
 *      Author: nicolasrosa
 */

#ifndef STEREODIFF_H
#define STEREODIFF_H

/* Libraries */
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class StereoDiff{
public:
    /* Constructor and Destructor */
    StereoDiff();
    ~StereoDiff();

    void calculateDiff();

    void createDiffImage(Mat,Mat);
    void createResAND(Mat,Mat);
    void convertToBGR();
    void addRedLines();

    bool StartDiff;
    Mat diffImage;

    Mat res_AND;
    Mat imageL;
    Mat res_AND_BGR;
    Mat res_AND_BGR_channels[3];

    double alpha;
    double beta;
    Mat res_ADD;
};

#endif // STEREODIFF_H
