/*
 * stereoARM.cpp
 *
 *  Created on: Feb 14, 2016
 *      Author: nicolasrosa
 */

/* Libraries */
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define TARGET_PC
//#define TARGET_BBB

#define STEREO_BM
//#define STEREO_SGBM

//#define RESOLUTION_320x240
#define RESOLUTION_640x480
//#define RESOLUTION_1280x960

#define EROSION_SIZE 5 //SAR
#define DILATE_SIZE 5  //SAR

/* Classes */
class StereoUtils{
public:
    StereoUtils(); //Constructor
    /* Frame Size */
    void resizeFrames(Mat* frame1,Mat* frame2);

    /* Timing */
    void startClock();
    void stopClock();
    void getElapsedTime();

private:
    struct timespec start,end;
    int fps;
};

class StereoProcessor{
public:
    StereoProcessor(); //Constructor
    void createBM();
    void open();
    void getImageSize();
    void resizeVideoCapture();
    void readCalibrationFiles();
    void readIntrinsicsFile();
    void readExtrinsicsFile();
    void setParametersBM();

    void captureFrames();
    void applyRectification();
    void calculateDisparities();
    void applyMorphology();
    void videoLooper();

    StereoUtils utils;

    string imageL_filename;
    string imageR_filename;
    Size imageSize;
    Size imageSizeDesired;
    int frameCounter;

    /* Rectification*/
    Mat rmap[2][2];
    Mat imageLr, imageRr;

    Mat imageL,imageR;
    Mat imageL_grey,imageR_grey;
    VideoCapture capL,capR;

    Ptr<StereoBM> bm;
    Ptr<StereoSGBM> sgbm;

    /* BM Configuration Variables */
    Mat M1,D1,M2,D2;
    Mat R,T,R1,P1,R2,P2;
    Rect roi1, roi2;
    Mat Q;

    int ndisparities;
    int SADWindowSize;

    /* Results */
    Mat disp_16S;
    Mat disp_8U;
    Mat disp_BGR;

private:

};

/* Constructors */
StereoUtils::StereoUtils(){

}

StereoProcessor::StereoProcessor(){
    ndisparities = 64;
    SADWindowSize = 31;
}

void StereoProcessor::createBM(){
    bm = StereoBM::create(ndisparities,SADWindowSize);
}

void StereoProcessor::open(){
    //    capL.open(0);
    //    capR.open(1);

#ifdef TARGET_PC
    /* Run at PC */
    capL.open("/home/nicolas/workspace/data/20004.avi");
    capR.open("/home/nicolas/workspace/data/30004.avi");
#endif

#ifdef TARGET_BBB
    /* Run at BBB */
    capL.open("../data/20004.avi");
    capR.open("../data/30004.avi");
#endif

    if(!capR.isOpened()){ 					// Check if we succeeded
        cout << "Open Video Left Failed!" << endl;
        //return -1;
    }

    if(!capL.isOpened()){ 					// Check if we succeeded
        cout << "Open Video Right Failed!" << endl;
        //return -1;
    }
}

void StereoProcessor::getImageSize(){
    imageSize.width = capR.get(CV_CAP_PROP_FRAME_WIDTH);
    imageSize.height = capR.get(CV_CAP_PROP_FRAME_HEIGHT);

    cout << "Camera 1 Resolution: " << capR.get(CV_CAP_PROP_FRAME_WIDTH) << "x" << capR.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
    cout << "Camera 2 Resolution: " << capL.get(CV_CAP_PROP_FRAME_WIDTH) << "x" << capL.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
}

void StereoUtils::resizeFrames(Mat* frame1,Mat* frame2){
    if(frame1->cols != 0 || !frame2->cols != 0){
#ifdef RESOLUTION_320x240
        resize(*frame1, *frame1, Size(320,240), 0, 0, INTER_CUBIC);
        resize(*frame2, *frame2, Size(320,240), 0, 0, INTER_CUBIC);
#endif

#ifdef RESOLUTION_640x480
        resize(*frame1, *frame1, Size(640,480), 0, 0, INTER_CUBIC);
        resize(*frame2, *frame2, Size(640,480), 0, 0, INTER_CUBIC);
#endif

#ifdef RESOLUTION_1280x720
        resize(*frame1, *frame1, Size(1280,720), 0, 0, INTER_CUBIC);
        resize(*frame2, *frame2, Size(1280,720), 0, 0, INTER_CUBIC);
#endif
    }
}

void StereoProcessor::resizeVideoCapture(){
    /* Defines the Desired Resolution */
#ifdef RESOLUTION_320x240
    imageSizeDesired = Size(320,240);
#endif

#ifdef RESOLUTION_640x480
    imageSizeDesired = Size(640,480);
#endif

#ifdef RESOLUTION_1280x960
    imageSizeDesired = Size(1280,960);
#endif

    /* Resizes the VideoCapture Object */
    capL.set(CV_CAP_PROP_FRAME_WIDTH, imageSizeDesired.width);
    capL.set(CV_CAP_PROP_FRAME_HEIGHT,imageSizeDesired.height);
    capR.set(CV_CAP_PROP_FRAME_WIDTH, imageSizeDesired.width);
    capR.set(CV_CAP_PROP_FRAME_HEIGHT,imageSizeDesired.height);
}

/*** Stereo Calibration function
  ** Description: Reads the Calibrations in *.yml files
  ** Receives:    Matrices Addresses for storage
  ** @param Mat M1,M2: Intrinsic Matrices from camera 1 and 2
  ** @param Mat D1,D2: Distortion Coefficients from camera 1 and 2
  ** @param Mat R: Rotation Matrix
  ** @param Mat t: Translation Vector
  ** Returns:     Nothing
  ***/
void StereoProcessor::readCalibrationFiles(){
    cout << "Calibration: ON" << endl;

    readIntrinsicsFile();
    readExtrinsicsFile();

    /* Checking if the Reading Process was Successful */
    if(!M1.data || !D1.data || !M2.data || !D2.data || !R.data || !T.data){
        cerr << "Check instrinsics and extrinsics Matrixes content!" << endl;
        return;
    }

    /* Console Output */
    cout << "------------------------------Intrinsics------------------------------" << endl;
    cout << "M1: " << endl << M1 << endl;
    cout << "D1: " << endl << D1 << endl;
    cout << "M2: " << endl << M2 << endl;
    cout << "D2: " << endl << D2 << endl << endl;
    cout << "intrinsics.yml Read Successfully."  << endl << endl;

    cout << "------------------------------Extrinsics------------------------------" << endl;
    cout << "R: " << endl << R << endl;
    cout << "T: " << endl << T << endl << endl;
    cout << "extrinsics.yml Read Successfully."  << endl;
    cout << "----------------------------------------------------------------------" << endl << endl;
}

void StereoProcessor::readIntrinsicsFile(){
    FileStorage fs("../config/intrinsics.yml", FileStorage::READ);
    if(!fs.isOpened()){
        cerr << "Failed to open intrinsics.yml file!" << endl;
        return;
    }

    fs["M1"] >> M1;
    fs["D1"] >> D1;
    fs["M2"] >> M2;
    fs["D2"] >> D2;

    fs.release();

    float scale = 1.f;
    M1 *= scale;
    M2 *= scale;
}

void StereoProcessor::readExtrinsicsFile(){
    FileStorage fs("../config/extrinsics.yml", FileStorage::READ);
    if(!fs.isOpened()){
        cerr << "Failed to open extrinsics.yml file!" << endl;
        return;
    }

    fs["R"] >> R;
    fs["T"] >> T;

    fs.release();
}

void StereoProcessor::captureFrames(){
    /* Capture Frames from the VideoCap Object */
    capL >> imageL;
    capR >> imageR;

    /* Resizing the Input Resolution to the Desired Resolution */
    utils.resizeFrames(&imageL,&imageR);
}

void StereoProcessor::applyRectification(){
    imageSize = imageL.size();
    stereoRectify(M1,D1,M2,D2,imageSize,R,T,R1,R2,P1,P2,Q,CALIB_ZERO_DISPARITY,-1,imageSize,&roi1,&roi2);

    initUndistortRectifyMap(M1, D1, R1, P1, imageSize, CV_16SC2, rmap[0][0], rmap[0][1]);
    initUndistortRectifyMap(M2, D2, R2, P2, imageSize, CV_16SC2, rmap[1][0], rmap[1][1]);

    remap(imageL, imageLr, rmap[0][0], rmap[0][1], INTER_LINEAR);
    remap(imageR, imageRr, rmap[1][0], rmap[1][1], INTER_LINEAR);

    imageL = imageLr;
    imageR = imageRr;
}

void StereoProcessor::calculateDisparities(){
    // Convert BGR to Grey Scale
    cvtColor(imageL,imageL_grey,CV_BGR2GRAY);
    cvtColor(imageR,imageR_grey,CV_BGR2GRAY);

#ifdef STEREO_BM
    bm->compute(imageL_grey,imageR_grey,disp_16S);
#endif

#ifdef STEREO_SGBM
    sgbm->compute(imageL,imageR,disp.disp_16S);
#endif

    //cout << "Computing Stereo Matching..." << endl;
    bm->compute(imageL_grey,imageR_grey,disp_16S);

    normalize(disp_16S, disp_8U, 0, 255, CV_MINMAX, CV_8U);

    //DepthMap GreyScale to RGB
    //cvtColor(disp_8U,disp_BGR,CV_GRAY2BGR);
    //applyColorMap(disp_8U,disp_BGR, COLORMAP_JET);
}

void StereoProcessor::setParametersBM(){
    bm->setPreFilterSize(127);
    bm->setPreFilterCap(36);
    bm->setBlockSize(81);
    bm->setMinDisparity(7);
    bm->setNumDisparities(16);
    bm->setTextureThreshold(0);
    bm->setUniquenessRatio(0);
    bm->setSpeckleWindowSize(0);
    bm->setSpeckleRange(0);
    bm->setDisp12MaxDiff(1);
}

void StereoProcessor::applyMorphology(){
    //Erode the images
    /*Mat element(1,1,CV_8U,Scalar(1));
    erode(imgDisparity8U,result,element,Point(-1,-1),3);*/

    //SAR Erode and Dilate to take out spurious noise
    Mat element_erode = getStructuringElement( MORPH_RECT,Size( 2*EROSION_SIZE + 1, 2*EROSION_SIZE+1 ),
                                               Point( EROSION_SIZE, EROSION_SIZE ));
    Mat element_dilate = getStructuringElement( MORPH_RECT,Size( 2*DILATE_SIZE + 1, 2*DILATE_SIZE+1 ),
                                                Point( DILATE_SIZE, DILATE_SIZE ));
    cout << "Eroding and Dilating Result..." << endl;
    erode(disp_8U,disp_8U,element_erode);
    dilate(disp_8U,disp_8U,element_dilate);
}

void StereoProcessor::videoLooper(){
    frameCounter += 1;

    if(frameCounter == capR.get(CV_CAP_PROP_FRAME_COUNT)){
        frameCounter = 0;
        capL.set(CV_CAP_PROP_POS_FRAMES,0);
        capR.set(CV_CAP_PROP_POS_FRAMES,0);
    }
}

void StereoUtils::startClock(){
    //lastTime = clock();
    clock_gettime(CLOCK_REALTIME,&start);
}

void StereoUtils::stopClock(){
    //currentTime = clock();
    clock_gettime(CLOCK_REALTIME,&end);
}

void StereoUtils::getElapsedTime(){
    /* Calculate the Elapsed Time */
    double difference = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/1000000000.0d;
    fps = (int) 1/difference;

    /* ShowFPS */
    cout << "t: " << difference << "\tFPS:" << fps << endl;
}

int main(){
    //Matrix to store each left and right frame of the video_l.avi and video_r.avi
    StereoProcessor stereo;

    stereo.createBM();
    stereo.open();
    stereo.getImageSize();
    stereo.resizeVideoCapture();
    stereo.readCalibrationFiles();

    /* Setting StereoBM Parameters */
    stereo.setParametersBM();

    while(1){
        stereo.utils.startClock();
        /* (1)Taking New Frame */
        stereo.captureFrames();

        /* (2)Applying Rectification */
        //stereo.applyRectification();

        /* (3)Calculating Disparity */;
        stereo.calculateDisparities();

        /* (4) Applying Morphology */
        //stereo.applyMorphology();

        /* (5)Performance Measurement - FPS */
        stereo.utils.stopClock();
        stereo.utils.getElapsedTime();

        //cout << "DONE..." << endl << endl;

        /* (6) Video Looper */
        stereo.videoLooper();

#ifdef TARGET_PC
        imshow("ImageL",stereo.imageL);
        imshow("Disparity Map",stereo.disp_8U);
        imwrite("disp_8U.jpg", stereo.disp_8U);
#endif

#ifdef TARGET_BBB
        /* Save the result */
        //cout << "Saving Disparity Map..." << endl;
        imwrite("disp_8U.jpg", stereo.disp_8U);
#endif

        if(waitKey(30) >= 0) break;
    }
    // The camera will be closed automatically in VideoCapture Destructor
    cout << "END" << endl;
    return 0;
}
