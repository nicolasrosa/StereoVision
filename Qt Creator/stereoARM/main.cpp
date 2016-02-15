/*
 * stereoARM.cpp
 *
 *  Created on: Feb 14, 2016
 *      Author: nicolasrosa
 */

/* Libraries */
#include "stdio.h"
#include "opencv2/opencv.hpp"

#define STEREO_BM
//#define STEREO_SGBM

#define RESOLUTION_320x240
//#define RESOLUTION_640x480
//#define RESOLUTION_1280x960

#define EROSION_SIZE 3  //SAR
#define DILATE_SIZE 5  //SAR

using namespace cv;
using namespace std;

/* Classes */
class StereoProcessor{
public:
    StereoProcessor(); //Constructor
    void createBM();
    void open();
    void getImageSize();
    void resize();
    void readCalibrationFiles();
    void readIntrinsicsFile();
    void readExtrinsicsFile();
    void setParametersBM();

    void captureFrames();
    void applyRectification();
    void calculateDisparities();
    void applyMorphology();
    void videoLooper();

    string imageL_filename;
    string imageR_filename;

    Mat imageL,imageR;
    Mat imageL_grey,imageR_grey;
    VideoCapture capL,capR;

    Ptr<StereoBM> bm;
    Ptr<StereoSGBM> sgbm;

    Size imageSize;
    Size imageSizeDesired;

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
    int inputNum;
};

StereoProcessor::StereoProcessor(){
    ndisparities = 64;
    SADWindowSize = 31;
}

void StereoProcessor::createBM(){
    bm = StereoBM::create(ndisparities,SADWindowSize);
}

void StereoProcessor::open(){
//    capR.open(0);
//    capL.open(1);

    capR.open("../data/20004.avi");
    capL.open("../data/30004.avi");

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

void StereoProcessor::resize(){
    /* Defines the Desired Resolution */
    #ifdef RESOLUTION_320x240
        imageSizeDesired = Size(320,240);
    #endif

    #ifdef RESOLUTION_640x480
        imageSizeDesired = Size(640,480;
    #endif

    #ifdef RESOLUTION_1280x960
        imageSizeDesired. = Size(1280,960);
    #endif

    /* Resizes the VideoCapture Object */
    capL.set(CV_CAP_PROP_FRAME_WIDTH, imageSizeDesired.width);
    capL.set(CV_CAP_PROP_FRAME_HEIGHT,imageSizeDesired.height);
    capR.set(CV_CAP_PROP_FRAME_WIDTH, imageSizeDesired.width);
    capR.set(CV_CAP_PROP_FRAME_HEIGHT,imageSizeDesired.height);

    /* Memory Allocation for Disparities Matrices */
    disp_16S = Mat(imageSizeDesired.height,imageSizeDesired.width, CV_16S );
    disp_8U = Mat(imageSizeDesired.height,imageSizeDesired.width, CV_8UC1 );
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
    if(!this->M1.data || !this->D1.data || !this->M2.data || !this->D2.data || !this->R.data || !this->T.data){
        cerr << "Check instrinsics and extrinsics Matrixes content!" << endl;
        return;
    }

    /* Console Output */
    cout << "------------------------------Intrinsics------------------------------" << endl;
    cout << "M1: " << endl << this->M1 << endl;
    cout << "D1: " << endl << this->D1 << endl;
    cout << "M2: " << endl << this->M2 << endl;
    cout << "D2: " << endl << this->D2 << endl << endl;
    cout << "intrinsics.yml Read Successfully."  << endl << endl;

    cout << "------------------------------Extrinsics------------------------------" << endl;
    cout << "R: " << endl << this->R << endl;
    cout << "T: " << endl << this->T << endl << endl;
    cout << "extrinsics.yml Read Successfully."  << endl;
    cout << "----------------------------------------------------------------------" << endl << endl;
}

void StereoProcessor::readIntrinsicsFile(){
    FileStorage fs("intrinsics.yml", FileStorage::READ);
    if(!fs.isOpened()){
        cerr << "Failed to open intrinsics.yml file!" << endl;
        return;
    }

    fs["M1"] >> this->M1;
    fs["D1"] >> this->D1;
    fs["M2"] >> this->M2;
    fs["D2"] >> this->D2;

    fs.release();

    float scale = 1.f;
    this->M1 *= scale;
    this->M2 *= scale;
}

void StereoProcessor::readExtrinsicsFile(){
    FileStorage fs("extrinsics.yml", FileStorage::READ);
    if(!fs.isOpened()){
        cerr << "Failed to open extrinsics.yml file!" << endl;
        return;
    }

    fs["R"] >> this->R;
    fs["T"] >> this->T;

    fs.release();
}

void StereoProcessor::captureFrames(){
    cout << "Taking New Frame..." << endl;

    capL >> imageL;
    capR >> imageR;
}

void StereoProcessor::applyRectification(){
    imageSize = imageL.size();
    stereoRectify(M1,D1,M2,D2,imageSize,R,T,R1,R2,P1,P2,Q,CALIB_ZERO_DISPARITY,-1,imageSize,&roi1,&roi2);

    Mat rmap[2][2];
    initUndistortRectifyMap(M1, D1, R1, P1, imageSize, CV_16SC2, rmap[0][0], rmap[0][1]);
    initUndistortRectifyMap(M2, D2, R2, P2, imageSize, CV_16SC2, rmap[1][0], rmap[1][1]);

    Mat imageLr, imageRr;
    remap(imageL, imageLr, rmap[0][0], rmap[0][1], INTER_LINEAR);
    remap(imageR, imageRr, rmap[1][0], rmap[1][1], INTER_LINEAR);

    imageL = imageLr;
    imageR = imageRr;
}

void StereoProcessor::calculateDisparities(){
    cout << "Calculating Disparity..." << endl;

    // Convert BGR to Grey Scale
    cvtColor(imageL,imageL_grey,CV_BGR2GRAY);
    cvtColor(imageR,imageR_grey,CV_BGR2GRAY);

#ifdef STEREO_BM
        bm->compute(imageL_grey,imageR_grey,disp_16S);
#endif

#ifdef STEREO_SGBM
        sgbm->compute(imageL,imageR,disp.disp_16S);
#endif

    cout << "Computing Stereo Matching..." << endl;
    //bm(leftImage,rightImage,imgDisparity16S,CV_16S);
    bm->compute(imageL_grey,imageR_grey,disp_16S);

    normalize(disp_16S, disp_8U, 0, 255, CV_MINMAX, CV_8U);
    //disp.disp_16S.convertTo(disp.disp_8U, CV_8U, 255/(BMcfg.numberOfDisparities*16.));

    cout << "oi" << endl;
    //applyColorMap(disp_8U,disp_BGR, COLORMAP_JET);
    cout << "oi2" << endl;
}

void StereoProcessor::setParametersBM(){
    cout << "Setting StereoBM Parameters..." << endl;

    bm->setPreFilterSize(131);
    bm->setPreFilterCap(61);
    bm->setBlockSize(67);
    bm->setMinDisparity(0);
    bm->setNumDisparities(16);
    bm->setTextureThreshold(9600);
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

int main(){
    //Matrix to store each left and right frame of the video_l.avi and video_r.avi
    StereoProcessor stereo;

    stereo.createBM();
    stereo.open();
    stereo.getImageSize();
    stereo.resize();
    stereo.readCalibrationFiles();
    stereo.setParametersBM();

    while(1){
        stereo.captureFrames();
        stereo.applyRectification();

        stereo.calculateDisparities();
        //stereo.applyMorphology();



        //DepthMap GreyScale to RGB
        //    cvtColor(imgDisparity8U,result_bgr,CV_GRAY2BGR);
        //applyColorMap(result,result_bgr, COLORMAP_JET);


        //Mat *cv_image_pseudocolor = imread(imgDisparity8U.size(),IPL_DEPTH_8U, 3);

        /*Mat image = imread("Black&White.jpg");
        imshow("Image",image);
        waitKey(0);
        That should fix your problem, if not, try using the C interface

        IplImage* image = cvLoadImage("Black&White.jpg");
        cvNamedWindow( "Image", CV_WINDOW_AUTOSIZE );
        cvShowImage("Image", image);
        cvWaitKey(0);*/


        cout << "Streaming!" << endl;
        #ifdef SHOW_VIDEO_STREAMING
            //imshow("Left" ,img2);
            //imshow("Right",img1);

            #if defined(RESOLUTION_320x240) || defined(RESOLUTION_640x480)
                //moveWindow("Left" ,0,0);
                //moveWindow("Right",capR.get(CV_CAP_PROP_FRAME_WIDTH)+100,0);
            #endif
        #endif

            //imshow("DepthImage",imgDisparity8U);
            //imshow("Eroded Image",result);
            //imshow("DepthImage RGB",result_bgr);


        #if defined(RESOLUTION_320x240) || defined(RESOLUTION_640x480)
            //moveWindow("DepthImage",0,capR.get(CV_CAP_PROP_FRAME_HEIGHT)+100);
            //moveWindow("Eroded Image",capR.get(CV_CAP_PROP_FRAME_WIDTH)+100,capR.get(CV_CAP_PROP_FRAME_HEIGHT)+100);
            //moveWindow("DepthImage RGB",capR.get(CV_CAP_PROP_FRAME_WIDTH)*2+150,capR.get(CV_CAP_PROP_FRAME_HEIGHT)+100);
            //moveWindow(trackbarWindowName,capR.get(CV_CAP_PROP_FRAME_WIDTH)*3+250,0);
        #endif

        cout << "DONE..." << endl;

        // Save the result
        imwrite("disp_8U.jpg", stereo.disp_8U);

        if(waitKey(30) >= 0) break;
    }
    // The camera will be closed automatically in VideoCapture Destructor
    cout << "END" << endl;
    return 0;
}
