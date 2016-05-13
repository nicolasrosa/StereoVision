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

    void printParams() const;

    void captureFrames();
    void stereoBMGPU_Init();

    void StereoRectificationInit();
    void StereoRectificationProcess();

    void calculateDisparitiesBM();
    void calculateDisparitiesSGBM();
    void calculateDisparitiesBMGPU();

    void videoLooper();
    void handleKey(char key);

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
    Mat imageLr, imageRr;
    Mat imageL_grey,imageR_grey;

    /* Rectification */
    Mat rmap[2][2];

    /* Disparity Map Declaration */
    //FIXME: Allocate Obj Memory:
    //cout << "imageL Size" << imageL.size() << endl;
    //Mat disp(imageL.size(), CV_8U);
    //Mat disp8U(imageL_grey.size(),CV_8U);
    Mat disp,disp8U,dispBGR;

#ifdef x64
    cuda::GpuMat d_imageL, d_imageR;

    //TODO: Initialize Variable with imageL.size() like: cuda::GpuMat d_disp(imageL.size(), CV_16S);
    //FIXME: Allocate Obj Memory: cuda::GpuMat d_disp8U(imageL_grey.size(),CV_8U);
    cuda::GpuMat d_disp,d_disp8U,d_dispBGR;

    Ptr<StereoBM> bm;
    Ptr<StereoSGBM> sgbm;
    Ptr<cuda::StereoBM> bm_gpu;
    Ptr<cuda::StereoBeliefPropagation> bp;
    Ptr<cuda::StereoConstantSpaceBP> csbp;
#endif

#ifdef jetsonTK1
    gpu::GpuMat d_imageL, d_imageR;

    //TODO: Allocate Obj Memory: gpu::GpuMat d_disp(imageL.size(), CV_16S);
    gpu::GpuMat d_disp;

    gpu::StereoBM_GPU bm_gpu;
#endif

};

#endif // APP_H
