#ifndef APP_H
#define APP_H

/* Libraries */
#include "main.h"
#include "params.h"
#include "StereoCalib.h"
#include "StereoUtils.h"

using namespace std;
using namespace cv;

class App{
public:
    App(const Params& p);
    void run();
    void printHelp();
    void open();
    void init();
    void loop();

    void handleKey(char key);
    void printParams() const;
    void stereoBMGPU_Init();
    void resizeFrames(Mat* frame1,Mat* frame2,Size resolution);
    void StereoRectificationInit();
    void videoLooper();
    void startClock();
    void stopClock();
    string text(int precision) const;

    StereoCalib calib;
    StereoUtils::Timer timer1;
    StereoUtils::Timer timer2;

private:
    Params p;
    int frameCounter;
    bool running;
    bool isVideoFile;
    bool isImageFile;

    VideoCapture capR;
    VideoCapture capL;

    Mat imageL, imageR;
    Mat imageL_grey, imageR_grey;

    /* Rectification */
    Mat rmap[2][2];

    /* Disparity Map Declaration */
    //FIXME: Inicializando da variável
    //cout << "imageL Size" << imageL.size() << endl;
    //Mat disp(imageL.size(), CV_8U);
    Mat disp;



#ifdef x64
    cuda::GpuMat d_imageL, d_imageR;

    Ptr<StereoBM> bm;
    Ptr<StereoSGBM> sgbm;
    Ptr<cuda::StereoBM> bm_gpu;
    Ptr<cuda::StereoBeliefPropagation> bp;
    Ptr<cuda::StereoConstantSpaceBP> csbp;

    //TODO: Initialize Variable with imageL.size() like: cuda::GpuMat d_disp(imageL.size(), CV_16S);
    cuda::GpuMat d_disp;

#endif

#ifdef jetsonTK1
    gpu::GpuMat d_imageL, d_imageR;

    gpu::StereoBM_GPU bm_gpu;

    //TODO: Initialize Variable with imageL.size() like: gpu::GpuMat d_disp(imageL.size(), CV_16S);
    gpu::GpuMat d_disp;
#endif

    double clockInitial;
    double d;
    double f;
    double fps;
};

#endif // APP_H
