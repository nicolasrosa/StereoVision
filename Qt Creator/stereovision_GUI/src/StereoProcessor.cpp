/*
 * StereoProcessor.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: nicolasrosa
 */

/* Libraries */
#include "inc/MainWindow.h"
#include "inc/StereoProcessor.h"
#include "inc/trackObject.h"

/* Constructor and Destructor */
StereoProcessor::StereoProcessor(int number) {
    inputNum=number;
    frameCounter=0;

    //lastImgThreshold = Mat::zeros(this->calib.imageSizeDesired,CV_8U);

    x=0;
    y=0;
}

StereoProcessor::~StereoProcessor(){
    delete bm;
    delete sgbm;

    calib.~StereoCalib();
    BMcfg.~StereoConfig();
    SGBMcfg.~StereoConfig();
    disp.~StereoDisparityMap();
    view3D.~Reconstruction3D();
    diff.~StereoDiff();
    flags.~StereoFlags();
    utils.~StereoUtils();
    morph.~StereoMorphology();
}

int StereoProcessor::getInputNum(){
    return inputNum;
}

void StereoProcessor::readConfigFile(){
    //FileStorage fs("../stereovision_GUI/config/config.yml", FileStorage::READ);
    FileStorage fs("../config/config.yml", FileStorage::READ);

    if(!fs.isOpened()){
        cerr << "Failed to open config.yml file!" << endl;
        return;
    }
    fs["Intrinsics Path"] >> this->calib.intrinsicsFileName;
    fs["Extrinsics Path"] >> this->calib.extrinsicsFileName;
    fs["Q Matrix Path"]   >> this->calib.QmatrixFileName;
    fs["StereoBM Parameters Path"] >> this->calib.StereoBMConfigFileName;
    fs["StereoSGBM Parameters Path"] >> this->calib.StereoSGBMConfigFileName;

    fs.release();

    cout << "------------------------------Config.yml------------------------------"    << endl;
    cout << "Intrinsics Path: "             << this->calib.intrinsicsFileName           << endl;
    cout << "Extrinsics Path: "             << this->calib.extrinsicsFileName           << endl;
    cout << "Q Matrix Path: "               << this->calib.QmatrixFileName              << endl;
    cout << "StereoBM Parameters Path:"     << this->calib.StereoBMConfigFileName       << endl;
    cout << "StereoSGBM Parameters Path:"   << this->calib.StereoSGBMConfigFileName     << endl;
    cout << "Config.yml Read Successfully." << endl << endl ;
    //cout << "----------------------------------------------------------------------"    << endl;
}

void StereoProcessor::readStereoBMConfigFile(){
    FileStorage fs(this->calib.StereoBMConfigFileName, FileStorage::READ);
    if(!fs.isOpened()){
        cerr << "Failed to open stereoBM.yml file!" << endl;
        return;
    }

    fs["methodName"] >> this->BMcfg.methodName;
    fs["preFilterSize"] >> this->BMcfg.preFilterSize;
    fs["preFilterCap"] >> this->BMcfg.preFilterCap;
    fs["SADWindowSize"] >> this->BMcfg.SADWindowSize;
    fs["minDisparity"] >> this->BMcfg.minDisparity;
    fs["numberOfDisparities"] >> this->BMcfg.numberOfDisparities;
    fs["textureThreshold"] >> this->BMcfg.textureThreshold;
    fs["uniquenessRatio"] >> this->BMcfg.uniquenessRatio;
    fs["speckleWindowSize"] >> this->BMcfg.speckleWindowSize;
    fs["speckleRange"] >> this->BMcfg.speckleRange;
    fs["disp12MaxDiff"] >> this->BMcfg.disp12MaxDiff;

    fs.release();

    // Display
    this->BMcfg.showConfigValues();
    cout << "stereoBM.yml Read Successfully."  << endl << endl;
    //cout << "----------------------------------------------------------------------" << endl << endl;
}

void StereoProcessor::readStereoSGBMConfigFile(){
    FileStorage fs(this->calib.StereoSGBMConfigFileName, FileStorage::READ);
    if(!fs.isOpened()){
        cerr << "Failed to open stereoSGBM.yml file!" << endl;
        return;
    }

    fs["methodName"] >> this->SGBMcfg.methodName;
    fs["preFilterSize"] >> this->SGBMcfg.preFilterSize;
    fs["preFilterCap"] >> this->SGBMcfg.preFilterCap;
    fs["SADWindowSize"] >> this->SGBMcfg.SADWindowSize;
    fs["minDisparity"] >> this->SGBMcfg.minDisparity;
    fs["numberOfDisparities"] >> this->SGBMcfg.numberOfDisparities;
    fs["textureThreshold"] >> this->SGBMcfg.textureThreshold;
    fs["uniquenessRatio"] >> this->SGBMcfg.uniquenessRatio;
    fs["speckleWindowSize"] >> this->SGBMcfg.speckleWindowSize;
    fs["speckleRange"] >> this->SGBMcfg.speckleRange;
    fs["disp12MaxDiff"] >> this->SGBMcfg.disp12MaxDiff;

    fs.release();

    // Display
    this->SGBMcfg.showConfigValues();
    cout << "stereoSGBM.yml Read Successfully."  << endl << endl;
    //cout << "----------------------------------------------------------------------" << endl << endl;
}

/*** StereoBM Initialization function
  ** Description: Executes the PreSetup of parameters of the StereoBM object
  ** @param StereoBM bm: Correspondence Object
  ** Returns:     Nothing
  ***/
void StereoProcessor::stereoBM_Init(){
    /* Creating Stereo Block Matching Object */
    this->bm = StereoBM::create(16,9);

    /* Initializing Stereo Block Matching Object */
    this->bm->setPreFilterSize(this->BMcfg.preFilterSize);
    this->bm->setPreFilterCap(this->BMcfg.preFilterCap);
    this->bm->setBlockSize(this->BMcfg.SADWindowSize);
    this->bm->setMinDisparity(this->BMcfg.minDisparity);
    this->bm->setNumDisparities(this->BMcfg.numberOfDisparities);
    this->bm->setTextureThreshold(this->BMcfg.textureThreshold);
    this->bm->setUniquenessRatio(this->BMcfg.uniquenessRatio);
    this->bm->setSpeckleWindowSize(this->BMcfg.speckleWindowSize);
    this->bm->setSpeckleRange(this->BMcfg.speckleRange);
    this->bm->setDisp12MaxDiff(this->disp12MaxDiff);
}

void StereoProcessor::stereoSGBM_Init(){
    /* Creating Stereo Semi-Global Block-Matching Object */
    this->sgbm = StereoSGBM::create(0,16,3);

    /* Initializing Stereo Semi-Global Block-Matching Object */
    this->sgbm->setPreFilterCap(50);
    this->sgbm->setBlockSize(1);
    this->sgbm->setMinDisparity(50);
    this->sgbm->setNumDisparities(16);
    this->sgbm->setUniquenessRatio(0);
    this->sgbm->setSpeckleWindowSize(100);
    this->sgbm->setSpeckleRange(0);
    this->sgbm->setDisp12MaxDiff(1);
}

/*** StereoBM Parameters Configuration function
  ** Description: Executes the setup of parameters of the StereoBM object by changing the trackbars
  ** @param rect roi1: Region of Interest 1
  ** @param rect roi2: Region of Interest 2
  ** @param StereoBM bm: Correspondence Object
  ** @param int numRows: Number of Rows of the input Images
  ** @param bool showStereoBMparams
  ** Returns:     Nothing
  ***/
void StereoProcessor::setStereoBM_Params(){
    this->bm->setROI1(this->calib.roi1);
    this->bm->setROI1(this->calib.roi2);

    this->numRows = imageL[0].rows;

    if(this->BMcfg.preFilterSize%2==1){
        bm->setPreFilterSize(this->BMcfg.preFilterSize);
    }

    bm->setPreFilterCap(this->BMcfg.preFilterCap);

    if(this->BMcfg.SADWindowSize%2==1 && this->BMcfg.SADWindowSize<=numRows){
        bm->setBlockSize(this->BMcfg.SADWindowSize);
    }

    bm->setMinDisparity(this->BMcfg.minDisparity);

    if(this->BMcfg.numberOfDisparities%16==0){
        bm->setNumDisparities(this->BMcfg.numberOfDisparities);
    }

    bm->setTextureThreshold(this->BMcfg.textureThreshold);
    bm->setUniquenessRatio( this->BMcfg.uniquenessRatio);
    bm->setSpeckleWindowSize(this->BMcfg.speckleWindowSize);
    bm->setSpeckleRange(this->BMcfg.speckleRange);
    bm->setDisp12MaxDiff(this->BMcfg.disp12MaxDiff);
}

void StereoProcessor::setStereoSGBM_Params(){
    //int trackbarsAux[10];

//    trackbarsAux[0] = this->SGBMcfg.preFilterSize*2.5+5;
//    trackbarsAux[1] = this->SGBMcfg.preFilterCap*0.625+1;
//    trackbarsAux[2] = this->SGBMcfg.SADWindowSize*2.5+5;
//    trackbarsAux[3] = this->SGBMcfg.minDisparity*2.0-100;
//    trackbarsAux[4] = this->SGBMcfg.numberOfDisparities*16;
//    trackbarsAux[5] = this->SGBMcfg.textureThreshold*320;
//    trackbarsAux[6] = this->SGBMcfg.uniquenessRatio*2.555;
//    trackbarsAux[7] = this->SGBMcfg.speckleWindowSize*1.0;
//    trackbarsAux[8] = this->SGBMcfg.speckleRange*1.0;
//    trackbarsAux[9] = this->SGBMcfg.disp12MaxDiff*1.0;

//    trackbarsAux[0] = this->SGBMcfg.preFilterSize;
//    trackbarsAux[1] = this->SGBMcfg.preFilterCap;
//    trackbarsAux[2] = this->SGBMcfg.SADWindowSize;
//    trackbarsAux[3] = this->SGBMcfg.minDisparity;
//    trackbarsAux[4] = this->SGBMcfg.numberOfDisparities;
//    trackbarsAux[5] = this->SGBMcfg.textureThreshold;
//    trackbarsAux[6] = this->SGBMcfg.uniquenessRatio;
//    trackbarsAux[7] = this->SGBMcfg.speckleWindowSize;
//    trackbarsAux[8] = this->SGBMcfg.speckleRange;
//    trackbarsAux[9] = this->SGBMcfg.disp12MaxDiff;

//    cout << "0: " << trackbarsAux[0] << endl;
//    cout << "1: " << trackbarsAux[1] << endl;
//    cout << "2: " << trackbarsAux[2] << endl;
//    cout << "3: " << trackbarsAux[3] << endl;
//    cout << "4: " << trackbarsAux[4] << endl;
//    cout << "5: " << trackbarsAux[5] << endl;
//    cout << "6: " << trackbarsAux[6] << endl;
//    cout << "7: " << trackbarsAux[7] << endl;
//    cout << "8: " << trackbarsAux[8] << endl;
//    cout << "9: " << trackbarsAux[9] << endl;

    this->numChannels = imageL[0].channels();

    sgbm->setP1(8*numChannels*this->SGBMcfg.SADWindowSize*this->SGBMcfg.SADWindowSize);
    sgbm->setP2(32*numChannels*this->SGBMcfg.SADWindowSize*this->SGBMcfg.SADWindowSize);
    sgbm->setMode(StereoSGBM::MODE_SGBM);

    sgbm->setPreFilterCap(this->SGBMcfg.preFilterCap);

    if( this->SGBMcfg.SADWindowSize%2==1 &&  this->SGBMcfg.SADWindowSize<=numRows){
        sgbm->setBlockSize( this->SGBMcfg.SADWindowSize);
    }

    sgbm->setMinDisparity(this->SGBMcfg.minDisparity);

    if(this->SGBMcfg.numberOfDisparities%16==0){
        sgbm->setNumDisparities(this->SGBMcfg.numberOfDisparities);
    }

    sgbm->setUniquenessRatio(this->SGBMcfg.uniquenessRatio);
    sgbm->setSpeckleWindowSize(this->SGBMcfg.speckleWindowSize);
    sgbm->setSpeckleRange(this->SGBMcfg.speckleRange);
    sgbm->setDisp12MaxDiff(this->SGBMcfg.disp12MaxDiff);
}

void StereoProcessor::captureFrames(){
    /* Capture Frames from the VideoCap Object */
    this->capL >> this->imageL[0];
    this->capR >> this->imageR[0];

    /* Resizing the Input Resolution to the Desired Resolution */
    this->utils.resizeFrames(&this->imageL[0],&this->imageR[0]);
}

void StereoProcessor::applyRectification(){
    this->calib.imageSize = this->imageL[0].size();
    stereoRectify(this->calib.M1,this->calib.D1,this->calib.M2,this->calib.D2,this->calib.imageSize,this->calib.R,this->calib.T,this->calib.R1,this->calib.R2,this->calib.P1,this->calib.P2,this->calib.Q,CALIB_ZERO_DISPARITY,-1,this->calib.imageSize,&this->calib.roi1,&this->calib.roi2);

    Mat rmap[2][2];
    initUndistortRectifyMap(this->calib.M1, this->calib.D1, this->calib.R1, this->calib.P1, this->calib.imageSize, CV_16SC2, rmap[0][0], rmap[0][1]);
    initUndistortRectifyMap(this->calib.M2, this->calib.D2, this->calib.R2, this->calib.P2, this->calib.imageSize, CV_16SC2, rmap[1][0], rmap[1][1]);

    Mat imageLr, imageRr;
    remap(this->imageL[0], imageLr, rmap[0][0], rmap[0][1], INTER_LINEAR);
    remap(this->imageR[0], imageRr, rmap[1][0], rmap[1][1], INTER_LINEAR);

    this->imageL[0] = imageLr;
    this->imageR[0] = imageRr;
}

void StereoProcessor::calculateDisparities(){
    // Convert BGR to Grey Scale
    cvtColor(this->imageL[0],this->imageL_grey[0],CV_BGR2GRAY);
    cvtColor(this->imageR[0],this->imageR_grey[0],CV_BGR2GRAY);

//    this->utils.startClock();
    if(this->flags.methodBM)
        this->bm->compute(this->imageL_grey[0],this->imageR_grey[0],this->disp.disp_16S);
//    this->utils.stopClock();

//    cout << "t:" << 1.0/this->utils.d << endl;

    if(this->flags.methodSGBM)
        this->sgbm->compute(this->imageL[0],this->imageR[0],this->disp.disp_16S);

    normalize(this->disp.disp_16S, this->disp.disp_8U, 0, 255, CV_MINMAX, CV_8U);
    //this->disp.disp_16S.convertTo(this->disp.disp_8U, CV_8U, 255/(this->BMcfg.numberOfDisparities*16.));

    applyColorMap(this->disp.disp_8U,this->disp.disp_BGR, COLORMAP_JET);

    if(flags.showDispDepth){
        /* Forces the initialization of the 3DReconstruction Method for obtaining the Depth Values */
        cv::reprojectImageTo3D(disp.disp_16S,view3D.depth,calib.Q);

        float disparity = this->disp.disp_16S.at<uchar>(y,x);
        float depth = view3D.depth.at<Vec3f>(y,x)[2];

        cout << "P(" << x << "," << y << ")"<< "\t" << "Disparity: " << disparity << "\t" << "Depth: " << depth << endl;
    }
}

void StereoProcessor::calculate3DReconstruction(){
    //view3D.fillOcclusion(disp.disp_16S,16,false);

    cv::reprojectImageTo3D(disp.disp_16S,view3D.depth,calib.Q);
    view3D.xyz = view3D.depth.reshape(3,view3D.depth.size().area());

    view3D.lookat(view3D.viewpoint,view3D.lookatpoint,view3D.Rotation);
    view3D.t.at<double>(0,0)=view3D.viewpoint.x;
    view3D.t.at<double>(1,0)=view3D.viewpoint.y;
    view3D.t.at<double>(2,0)=view3D.viewpoint.z;

    if(flags.showXYZ){
        //cout<< view3D.t <<endl;

        cout << "x: " << view3D.t.at<double>(0,0) << endl;
        cout << "y: " << view3D.t.at<double>(1,0) << endl;
        cout << "z: " << view3D.t.at<double>(2,0) << endl;
    }

    view3D.t=view3D.Rotation*view3D.t;

    view3D.projectImagefromXYZ(disp.disp_BGR,view3D.disp3D_BGR,disp.disp_16S,view3D.disp3D,view3D.xyz,view3D.Rotation,view3D.t,calib.K,view3D.dist,view3D.isSub);

    view3D.disp3D.convertTo(view3D.disp3D_8U,CV_8U,0.5);
}

//Saving Previous Frame
void StereoProcessor::saveLastFrames(){
    imageL[0].copyTo(imageL[1]);
    imageR[0].copyTo(imageR[1]);
    imageL_grey[0].copyTo(imageL_grey[1]);
    imageR_grey[0].copyTo(imageR_grey[1]);
    disp.disp_8U.copyTo(disp.disp_8U_last);
}

void StereoProcessor::setValues(int preFilterSize, int preFilterCap, int sadWindowSize, int minDisparity, int numOfDisparities, int textureThreshold, int uniquenessRatio, int speckleWindowSize, int speckleWindowRange, int disp12MaxDiff) {
    BMcfg.preFilterSize = preFilterSize;
    BMcfg.preFilterCap = preFilterCap;
    BMcfg.SADWindowSize = sadWindowSize;
    BMcfg.minDisparity = minDisparity;
    BMcfg.numberOfDisparities = numOfDisparities;
    BMcfg.textureThreshold = textureThreshold;
    BMcfg.uniquenessRatio = uniquenessRatio;
    BMcfg.speckleRange = speckleWindowRange;
    BMcfg.speckleWindowSize = speckleWindowSize;
    BMcfg.disp12MaxDiff = disp12MaxDiff;

    SGBMcfg.preFilterSize = preFilterSize;
    SGBMcfg.preFilterCap = preFilterCap;
    SGBMcfg.SADWindowSize = sadWindowSize;
    SGBMcfg.minDisparity = minDisparity;
    SGBMcfg.numberOfDisparities = numOfDisparities;
    SGBMcfg.textureThreshold = textureThreshold;
    SGBMcfg.uniquenessRatio = uniquenessRatio;
    SGBMcfg.speckleRange = speckleWindowRange;
    SGBMcfg.speckleWindowSize = speckleWindowSize;
    SGBMcfg.disp12MaxDiff = disp12MaxDiff;

    //std::cout << "Set Values!\n";
}

void StereoProcessor::videoLooper(){
    frameCounter += 1;

    //Debug
    //cout << "Frames: " << frameCounter << "/" << capL.get(CV_CAP_PROP_FRAME_COUNT) << endl;

    if(frameCounter == capL.get(CV_CAP_PROP_FRAME_COUNT)){
        frameCounter = 0;
        capL.set(CV_CAP_PROP_POS_FRAMES,0);
        capR.set(CV_CAP_PROP_POS_FRAMES,0);
    }
}


