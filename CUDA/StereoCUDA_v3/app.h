#ifndef APP_H
#define APP_H

/* Libraries */
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>

#include <opencv2/core/utility.hpp>
#include <opencv2/cudastereo.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "params.h"

using namespace std;
using namespace cv;

class App{
public:
    App(const Params& p);
    void run();
    void handleKey(char key);
    void printParams() const;
    void open();
    void resizeFrames(Mat* frame1,Mat* frame2,Size resolution);
    void videoLooper();
    void startClock();
    void stopClock();
    string text() const;

private:
    Params p;
    int frameCounter;
    bool running;
    bool isVideoFile;
    bool isImageFile;

    VideoCapture capR;
    VideoCapture capL;

    Mat imageL_src, imageR_src;
    Mat imageL, imageR;
    cuda::GpuMat d_imageL, d_imageR;

    Ptr<cuda::StereoBM> bm;
    Ptr<cuda::StereoBeliefPropagation> bp;
    Ptr<cuda::StereoConstantSpaceBP> csbp;

    int64 clockInitial;
    int64 d;
    double f;
    double fps;
};

#endif // APP_H
