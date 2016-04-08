#ifndef APP_H
#define APP_H

/* Libraries */
#include "stereocuda.h"
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

#ifdef x64
    cuda::GpuMat d_imageL, d_imageR;

    Ptr<cuda::StereoBM> bm;
    Ptr<cuda::StereoBeliefPropagation> bp;
    Ptr<cuda::StereoConstantSpaceBP> csbp;
#endif

#ifdef jetsonTK1
    gpu::GpuMat d_imageL, d_imageR;

    gpu::StereoBM_GPU bm;
#endif

    int64 clockInitial;
    int64 d;
    double f;
    double fps;
};

#endif // APP_H
