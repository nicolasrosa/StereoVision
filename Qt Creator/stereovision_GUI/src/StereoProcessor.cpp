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

    //lastImgThreshold = Mat::zeros(calib.imageSizeDesired,CV_8U);

    x=0;
    y=0;
}

StereoProcessor::~StereoProcessor(){
    delete bm;
    delete sgbm;

    calib.~StereoCalib();
    cfgBM.~StereoConfig();
    cfgSGBM.~StereoConfig();
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
    fs["Intrinsics Path"] >> calib.intrinsicsFileName;
    fs["Extrinsics Path"] >> calib.extrinsicsFileName;
    fs["Q Matrix Path"]   >> calib.QmatrixFileName;
    fs["StereoBM Parameters Path"] >> calib.StereoBMConfigFileName;
    fs["StereoSGBM Parameters Path"] >> calib.StereoSGBMConfigFileName;

    fs.release();

    cout << "------------------------------Config.yml------------------------------"    << endl;
    cout << "Intrinsics Path: "             << calib.intrinsicsFileName           << endl;
    cout << "Extrinsics Path: "             << calib.extrinsicsFileName           << endl;
    cout << "Q Matrix Path: "               << calib.QmatrixFileName              << endl;
    cout << "StereoBM Parameters Path:"     << calib.StereoBMConfigFileName       << endl;
    cout << "StereoSGBM Parameters Path:"   << calib.StereoSGBMConfigFileName     << endl;
    cout << "Config.yml Read Successfully." << endl << endl ;
    //cout << "----------------------------------------------------------------------"    << endl;
}

void StereoProcessor::readStereoBMConfigFile(){
    FileStorage fs(calib.StereoBMConfigFileName, FileStorage::READ);
    if(!fs.isOpened()){
        cerr << "Failed to open stereoBM.yml file!" << endl;
        return;
    }

    fs["methodName"] >> cfgBM.methodName;
    fs["preFilterSize"] >> cfgBM.preFilterSize;
    fs["preFilterCap"] >> cfgBM.preFilterCap;
    fs["SADWindowSize"] >> cfgBM.SADWindowSize;
    fs["minDisparity"] >> cfgBM.minDisparity;
    fs["numberOfDisparities"] >> cfgBM.numberOfDisparities;
    fs["textureThreshold"] >> cfgBM.textureThreshold;
    fs["uniquenessRatio"] >> cfgBM.uniquenessRatio;
    fs["speckleWindowSize"] >> cfgBM.speckleWindowSize;
    fs["speckleRange"] >> cfgBM.speckleRange;
    fs["disp12MaxDiff"] >> cfgBM.disp12MaxDiff;

    fs.release();

    // Display
    cfgBM.showConfigValues();
    cout << "stereoBM.yml Read Successfully."  << endl << endl;
    //cout << "----------------------------------------------------------------------" << endl << endl;
}

void StereoProcessor::readStereoSGBMConfigFile(){
    FileStorage fs(calib.StereoSGBMConfigFileName, FileStorage::READ);
    if(!fs.isOpened()){
        cerr << "Failed to open stereoSGBM.yml file!" << endl;
        return;
    }

    fs["methodName"] >> cfgSGBM.methodName;
    fs["preFilterSize"] >> cfgSGBM.preFilterSize;
    fs["preFilterCap"] >> cfgSGBM.preFilterCap;
    fs["SADWindowSize"] >> cfgSGBM.SADWindowSize;
    fs["minDisparity"] >> cfgSGBM.minDisparity;
    fs["numberOfDisparities"] >> cfgSGBM.numberOfDisparities;
    fs["textureThreshold"] >> cfgSGBM.textureThreshold;
    fs["uniquenessRatio"] >> cfgSGBM.uniquenessRatio;
    fs["speckleWindowSize"] >> cfgSGBM.speckleWindowSize;
    fs["speckleRange"] >> cfgSGBM.speckleRange;
    fs["disp12MaxDiff"] >> cfgSGBM.disp12MaxDiff;

    fs.release();

    // Display
    cfgSGBM.showConfigValues();
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
    bm = StereoBM::create(16,9);

    /* Initializing Stereo Block Matching Object */
    bm->setPreFilterSize(cfgBM.preFilterSize);
    bm->setPreFilterCap(cfgBM.preFilterCap);
    bm->setBlockSize(cfgBM.SADWindowSize);
    bm->setMinDisparity(cfgBM.minDisparity);
    bm->setNumDisparities(cfgBM.numberOfDisparities);
    bm->setTextureThreshold(cfgBM.textureThreshold);
    bm->setUniquenessRatio(cfgBM.uniquenessRatio);
    bm->setSpeckleWindowSize(cfgBM.speckleWindowSize);
    bm->setSpeckleRange(cfgBM.speckleRange);
    bm->setDisp12MaxDiff(disp12MaxDiff);
}

void StereoProcessor::stereoSGBM_Init(){
    /* Creating Stereo Semi-Global Block-Matching Object */
    sgbm = StereoSGBM::create(0,16,3);

    /* Initializing Stereo Semi-Global Block-Matching Object */
    sgbm->setPreFilterCap(50);
    sgbm->setBlockSize(1);
    sgbm->setMinDisparity(50);
    sgbm->setNumDisparities(16);
    sgbm->setUniquenessRatio(0);
    sgbm->setSpeckleWindowSize(100);
    sgbm->setSpeckleRange(0);
    sgbm->setDisp12MaxDiff(1);
}

void StereoProcessor::stereoBM_GPU_Init(){
    // Set common parameters
    bm_gpu = cuda::createStereoBM(16);

    //    bm->setPreFilterSize(127);
    //    bm->setPreFilterCap(61);
    bm_gpu->setBlockSize(15);
    //    bm->setMinDisparity(0);
    //    bm->setNumDisparities(16);
    bm_gpu->setTextureThreshold(4);
    //    bm->setUniquenessRatio(0);
    //    bm->setSpeckleWindowSize(0);
    //    bm->setSpeckleRange(0);
    //    bm->setDisp12MaxDiff(1);
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
    bm->setROI1(calib.roi1);
    bm->setROI1(calib.roi2);

    numRows = imageL[0].rows;

    if(cfgBM.preFilterSize%2==1){
        bm->setPreFilterSize(cfgBM.preFilterSize);
    }

    bm->setPreFilterCap(cfgBM.preFilterCap);

    if(cfgBM.SADWindowSize%2==1 && cfgBM.SADWindowSize<=numRows){
        bm->setBlockSize(cfgBM.SADWindowSize);
    }

    bm->setMinDisparity(cfgBM.minDisparity);

    if(cfgBM.numberOfDisparities%16==0){
        bm->setNumDisparities(cfgBM.numberOfDisparities);
    }

    bm->setTextureThreshold(cfgBM.textureThreshold);
    bm->setUniquenessRatio( cfgBM.uniquenessRatio);
    bm->setSpeckleWindowSize(cfgBM.speckleWindowSize);
    bm->setSpeckleRange(cfgBM.speckleRange);
    bm->setDisp12MaxDiff(cfgBM.disp12MaxDiff);
}

void StereoProcessor::setStereoSGBM_Params(){
    //int trackbarsAux[10];

    //    trackbarsAux[0] = SGBMcfg.preFilterSize*2.5+5;
    //    trackbarsAux[1] = SGBMcfg.preFilterCap*0.625+1;
    //    trackbarsAux[2] = SGBMcfg.SADWindowSize*2.5+5;
    //    trackbarsAux[3] = SGBMcfg.minDisparity*2.0-100;
    //    trackbarsAux[4] = SGBMcfg.numberOfDisparities*16;
    //    trackbarsAux[5] = SGBMcfg.textureThreshold*320;
    //    trackbarsAux[6] = SGBMcfg.uniquenessRatio*2.555;
    //    trackbarsAux[7] = SGBMcfg.speckleWindowSize*1.0;
    //    trackbarsAux[8] = SGBMcfg.speckleRange*1.0;
    //    trackbarsAux[9] = SGBMcfg.disp12MaxDiff*1.0;

    //    trackbarsAux[0] = SGBMcfg.preFilterSize;
    //    trackbarsAux[1] = SGBMcfg.preFilterCap;
    //    trackbarsAux[2] = SGBMcfg.SADWindowSize;
    //    trackbarsAux[3] = SGBMcfg.minDisparity;
    //    trackbarsAux[4] = SGBMcfg.numberOfDisparities;
    //    trackbarsAux[5] = SGBMcfg.textureThreshold;
    //    trackbarsAux[6] = SGBMcfg.uniquenessRatio;
    //    trackbarsAux[7] = SGBMcfg.speckleWindowSize;
    //    trackbarsAux[8] = SGBMcfg.speckleRange;
    //    trackbarsAux[9] = SGBMcfg.disp12MaxDiff;

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

    numChannels = imageL[0].channels();

    sgbm->setP1(8*numChannels*cfgSGBM.SADWindowSize*cfgSGBM.SADWindowSize);
    sgbm->setP2(32*numChannels*cfgSGBM.SADWindowSize*cfgSGBM.SADWindowSize);
    sgbm->setMode(StereoSGBM::MODE_SGBM);

    sgbm->setPreFilterCap(cfgSGBM.preFilterCap);

    if( cfgSGBM.SADWindowSize%2==1 &&  cfgSGBM.SADWindowSize<=numRows){
        sgbm->setBlockSize( cfgSGBM.SADWindowSize);
    }

    sgbm->setMinDisparity(cfgSGBM.minDisparity);

    if(cfgSGBM.numberOfDisparities%16==0){
        sgbm->setNumDisparities(cfgSGBM.numberOfDisparities);
    }

    sgbm->setUniquenessRatio(cfgSGBM.uniquenessRatio);
    sgbm->setSpeckleWindowSize(cfgSGBM.speckleWindowSize);
    sgbm->setSpeckleRange(cfgSGBM.speckleRange);
    sgbm->setDisp12MaxDiff(cfgSGBM.disp12MaxDiff);
}

void StereoProcessor::captureFrames(){
    /* Capture Frames from the VideoCap Object */
    capL >> imageL[0];
    capR >> imageR[0];

    /* Resizing the Input Resolution to the Desired Resolution */
    utils.resizeFrames(&imageL[0],&imageR[0]);
}

void StereoProcessor::applyRectification(){
    calib.imageSize = imageL[0].size();
    stereoRectify(calib.M1,calib.D1,calib.M2,calib.D2,calib.imageSize,calib.R,calib.T,calib.R1,calib.R2,calib.P1,calib.P2,calib.Q,CALIB_ZERO_DISPARITY,-1,calib.imageSize,&calib.roi1,&calib.roi2);

    Mat rmap[2][2];
    initUndistortRectifyMap(calib.M1, calib.D1, calib.R1, calib.P1, calib.imageSize, CV_16SC2, rmap[0][0], rmap[0][1]);
    initUndistortRectifyMap(calib.M2, calib.D2, calib.R2, calib.P2, calib.imageSize, CV_16SC2, rmap[1][0], rmap[1][1]);

    Mat imageLr, imageRr;
    remap(imageL[0], imageLr, rmap[0][0], rmap[0][1], INTER_LINEAR);
    remap(imageR[0], imageRr, rmap[1][0], rmap[1][1], INTER_LINEAR);

    imageL[0] = imageLr;
    imageR[0] = imageRr;
}

void StereoProcessor::calculateDisparities(){
    /* Convert BGR images to Grey Scale */
    if(flags.methodBM || flags.methodBM_GPU){
        cvtColor(imageL[0],imageL_grey[0],CV_BGR2GRAY);
        cvtColor(imageR[0],imageR_grey[0],CV_BGR2GRAY);
    }

    /* Computing Disparities - Disparity Map */
    if(flags.methodBM)
        bm->compute(imageL_grey[0],imageR_grey[0],disp.disp_16S);

    if(flags.methodSGBM)
        sgbm->compute(imageL[0],imageR[0],disp.disp_16S);

    if(flags.methodBM_GPU){
        d_imageL.upload(imageL_grey[0]);
        d_imageR.upload(imageR_grey[0]);
        bm_gpu->compute(d_imageL,d_imageR,d_disp_16S);
        d_disp_16S.download(disp.disp_16S);
    }

    normalize(disp.disp_16S, disp.disp_8U, 0, 255, CV_MINMAX, CV_8U);
    //disp.disp_16S.convertTo(disp.disp_8U, CV_8U, 255/(BMcfg.numberOfDisparities*16.));

    applyColorMap(disp.disp_8U,disp.disp_BGR, COLORMAP_JET);

    if(flags.showDispDepth){
        /* Forces the initialization of the 3DReconstruction Method for obtaining the Depth Values */
        cv::reprojectImageTo3D(disp.disp_16S,view3D.depth,calib.Q);

        float disparity = disp.disp_16S.at<uchar>(y,x);
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
    cfgBM.preFilterSize = preFilterSize;
    cfgBM.preFilterCap = preFilterCap;
    cfgBM.SADWindowSize = sadWindowSize;
    cfgBM.minDisparity = minDisparity;
    cfgBM.numberOfDisparities = numOfDisparities;
    cfgBM.textureThreshold = textureThreshold;
    cfgBM.uniquenessRatio = uniquenessRatio;
    cfgBM.speckleRange = speckleWindowRange;
    cfgBM.speckleWindowSize = speckleWindowSize;
    cfgBM.disp12MaxDiff = disp12MaxDiff;

    cfgSGBM.preFilterSize = preFilterSize;
    cfgSGBM.preFilterCap = preFilterCap;
    cfgSGBM.SADWindowSize = sadWindowSize;
    cfgSGBM.minDisparity = minDisparity;
    cfgSGBM.numberOfDisparities = numOfDisparities;
    cfgSGBM.textureThreshold = textureThreshold;
    cfgSGBM.uniquenessRatio = uniquenessRatio;
    cfgSGBM.speckleRange = speckleWindowRange;
    cfgSGBM.speckleWindowSize = speckleWindowSize;
    cfgSGBM.disp12MaxDiff = disp12MaxDiff;

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


