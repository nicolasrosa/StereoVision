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
#include "inc/StereoUtils.h"

/* Constructor and Destructor */
StereoProcessor::StereoProcessor(int number):rect(&calib,&imageL[0],&imageR[0]){
    //StereoProcessor::StereoProcessor(int number):rect(&calib,&imageL[0],&imageR[0]),morph(&utils){
    method = BM;

    inputNum=number;
    frameCounter=0;

    //lastImgThreshold = Mat::zeros(calib.imageSizeDesired,CV_8U);

    x=0;
    y=0;
}

//FIXME: StereoProcessor Destructor
StereoProcessor::~StereoProcessor(){
    //    delete bm;
    //    delete sgbm;
    //    delete bm_gpu;

    //    calib.~StereoCalib();

    //    cfgBM.~StereoConfig();
    //    cfgSGBM.~StereoConfig();
    //    cfgBM_GPU.~StereoConfig();

    //    disp.~StereoDisparityMap();
    //    view3D.~Reconstruction3D();
    //    diff.~StereoDiff();
    //    flags.~StereoFlags();
    //    utils.~StereoUtils();
    //    morph.~StereoMorphology();
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
    //    fs["Intrinsics Path"] >> calib.intrinsicsFileName;
    //    fs["Extrinsics Path"] >> calib.extrinsicsFileName;
    fs["Q Matrix Path"]   >> calib.QmatrixFileName;
    fs["StereoBM Parameters Path"] >> cfgBM.StereoBMConfigFileName;
    fs["StereoSGBM Parameters Path"] >> cfgSGBM.StereoSGBMConfigFileName;
    fs["StereoBM_GPU Parameters Path"] >> cfgBM_GPU.StereoBM_GPUConfigFileName;

    fs.release();

    cout << "------------------------------Config.yml------------------------------"    << endl;
    cout << "Intrinsics Path: "             << calib.intrinsicsFileName           << endl;
    cout << "Extrinsics Path: "             << calib.extrinsicsFileName           << endl;
    cout << "Q Matrix Path: "               << calib.QmatrixFileName              << endl;
    cout << "StereoBM Parameters Path:"     << cfgBM.StereoBMConfigFileName       << endl;
    cout << "StereoSGBM Parameters Path:"   << cfgSGBM.StereoSGBMConfigFileName     << endl;
    cout << "StereoBM_GPU Parameters Path:" << cfgBM_GPU.StereoBM_GPUConfigFileName   << endl;
    cout << "Config.yml Read Successfully." << endl << endl ;
    //cout << "----------------------------------------------------------------------"    << endl;
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
    bm->setDisp12MaxDiff(cfgBM.disp12MaxDiff);
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
    bm_gpu->setNumDisparities(64);
    bm_gpu->setTextureThreshold(4);
    //    bm->setUniquenessRatio(0);
    //    bm->setSpeckleWindowSize(0);
    //    bm->setSpeckleRange(0);
    //    bm->setDisp12MaxDiff(1);
}

void StereoProcessor::setNumRows(int value){
    numRows = value;
}

int StereoProcessor::getNumRows(){
    return numRows;
}

void StereoProcessor::setNumChannels(int value){
    numChannels = value;
}

int StereoProcessor::getNumChannels(){
    return numChannels;
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
    sgbm->setP1(8*numChannels*cfgSGBM.SADWindowSize*cfgSGBM.SADWindowSize);
    sgbm->setP2(32*numChannels*cfgSGBM.SADWindowSize*cfgSGBM.SADWindowSize);
    sgbm->setMode(StereoSGBM::MODE_SGBM);

    sgbm->setPreFilterCap(cfgSGBM.preFilterCap);

    if(cfgSGBM.SADWindowSize%2==1 &&  cfgSGBM.SADWindowSize<=numRows){
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

void StereoProcessor::setStereoBM_GPU_Params(){
    bm_gpu->setROI1(calib.roi1);
    bm_gpu->setROI2(calib.roi2);

    if(cfgBM_GPU.preFilterSize%2==1){
        bm_gpu->setPreFilterSize(cfgBM_GPU.preFilterSize);
    }

    bm_gpu->setPreFilterCap(cfgBM_GPU.preFilterCap);

    if(cfgBM_GPU.SADWindowSize%2==1 && cfgBM_GPU.SADWindowSize<=51){
        bm_gpu->setBlockSize(cfgBM_GPU.SADWindowSize);
    }

    bm_gpu->setMinDisparity(cfgBM_GPU.minDisparity);

    if(cfgBM_GPU.numberOfDisparities%16==0){
        bm_gpu->setNumDisparities(cfgBM_GPU.numberOfDisparities);
    }

    bm_gpu->setTextureThreshold(cfgBM_GPU.textureThreshold);
    bm_gpu->setUniquenessRatio( cfgBM_GPU.uniquenessRatio);
    bm_gpu->setSpeckleWindowSize(cfgBM_GPU.speckleWindowSize);
    bm_gpu->setSpeckleRange(cfgBM_GPU.speckleRange);
    bm_gpu->setDisp12MaxDiff(cfgBM_GPU.disp12MaxDiff);
}


void StereoProcessor::captureFrames(){
    /* Capture Frames from the VideoCap Object */
    capL >> imageL[0];
    capR >> imageR[0];

    /* Resizing the Input Resolution to the Desired Resolution */
    StereoUtils::Resizer::resizeFrames(&imageL[0],&imageR[0],calib.getResolutionDesired());
}


void StereoProcessor::calculateDisparities(){
    /* Convert BGR images to Grey Scale */
    if(method == StereoProcessor::BM || method == StereoProcessor::BM_GPU){
        cvtColor(imageL[0],imageL_grey[0],CV_BGR2GRAY);
        cvtColor(imageR[0],imageR_grey[0],CV_BGR2GRAY);
    }

    /* Computing Disparities - Disparity Map */
    //#define MEASURE_TIME_BM
    //#define MEASURE_TIME_SGBM
    //#define MEASURE_TIME_BMGPU

    switch(method){
        case StereoProcessor::BM:
            #ifdef MEASURE_TIME_BM
                    //! START CLOCK !//
                    StereoUtils::Timer::startClock(&time.clockInitial_d);
                    bm->compute(imageL_grey[0],imageR_grey[0],disp.disp_16S);
                    StereoUtils::Timer::stopClock(&time.clockFinal_d);
                    //! STOP CLOCK !//

                    StereoUtils::Timer::printElapsedTime(time.clockInitial_d,time.clockFinal_d);
            #else
                    bm->compute(imageL_grey[0],imageR_grey[0],disp.disp_16S);
            #endif
            break;
        case StereoProcessor::SGBM:
            #ifdef MEASURE_TIME_SGBM
                    //! START CLOCK !//
                    StereoUtils::Timer::startClock(&time.clockInitial_d);
                    sgbm->compute(imageL[0],imageR[0],disp.disp_16S);
                    StereoUtils::Timer::stopClock(&time.clockFinal_d);
                    //! STOP CLOCK !//

                    StereoUtils::Timer::printElapsedTime(time.clockInitial_d,time.clockFinal_d);
            #else
                    sgbm->compute(imageL[0],imageR[0],disp.disp_16S);
            #endif
            break;
        case StereoProcessor::BM_GPU:
            #ifdef MEASURE_TIME_BMGPU
                    //! START CLOCK !//
                    StereoUtils::Timer::startClock(&time.clockInitial_d);
                    d_imageL.upload(imageL_grey[0]);
                    d_imageR.upload(imageR_grey[0]);
                    bm_gpu->compute(d_imageL,d_imageR,d_disp_16S);
                    d_disp_16S.download(disp.disp_16S);
                    StereoUtils::Timer::stopClock(&time.clockFinal_d);
                    //! STOP CLOCK !//

                    StereoUtils::Timer::printElapsedTime(time.clockInitial_d,time.clockFinal_d);
            #else
                    d_imageL.upload(imageL_grey[0]);
                    d_imageR.upload(imageR_grey[0]);
                    bm_gpu->compute(d_imageL,d_imageR,d_disp_16S);
                    d_disp_16S.download(disp.disp_16S);
            #endif
            break;
    }

    normalize(disp.disp_16S, disp.disp_8U, 0, 255, CV_MINMAX, CV_8U);
    //disp.disp_16S.convertTo(disp.disp_8U, CV_8U, 255/(BMcfg.numberOfDisparities*16.));

    applyColorMap(disp.disp_8U,disp.disp_BGR, COLORMAP_JET);

    if(flags.showDispDepth){
        /* Forces the initialization of the 3DReconstruction Method for obtaining the Depth Values */
        cv::reprojectImageTo3D(disp.disp_16S,view3D.depth,calib.Q);

        float disparity = disp.disp_16S.at<uchar>(y,x);
        float depth = view3D.depth.at<Vec3f>(y,x)[2];

        /* Draw White Cross */
        circle(disp.disp_8U,Point(x,y),10,Scalar(255,255,255),2);
        if(y-25>0)
            line(disp.disp_8U,Point(x,y-5),Point(x,y-15),Scalar(255,255,255),2);
        else line(disp.disp_8U,Point(x,y),Point(x,0),Scalar(255,255,255),2);
        if(y+25<480)
            line(disp.disp_8U,Point(x,y+5),Point(x,y+15),Scalar(255,255,255),2);
        else line(disp.disp_8U,Point(x,y),Point(x,480),Scalar(255,255,255),2);
        if(x-25>0)
            line(disp.disp_8U,Point(x-5,y),Point(x-15,y),Scalar(255,255,255),2);
        else line(disp.disp_8U,Point(x,y),Point(0,y),Scalar(255,255,255),2);
        if(x+25<640)
            line(disp.disp_8U,Point(x+5,y),Point(x+15,y),Scalar(255,255,255),2);
        else line(disp.disp_8U,Point(x,y),Point(640,y),Scalar(255,255,255),2);

        /* CrossHair Information */
        putText(disp.disp_8U,StereoUtils::Extras::intToString(disparity),Point(x,y-20),1,1,Scalar(255,255,255),2);
        //putText(disp.disp_8U,utils.intToString((int)depth),Point(x,y-20),1,1,Scalar(255,255,255),2);
        putText(disp.disp_8U,StereoUtils::Extras::intToString(x)+","+StereoUtils::Extras::intToString(y),Point(x,y+30),1,1,Scalar(255,255,255),2);

        cout << "P(" << x << "," << y << ")"<< "\t" << "Disparity: " << disparity << "\t" << "Depth: " << depth << endl;
    }
}

void StereoProcessor::calculateTrueMap(){
    disp.disp_16S.convertTo(true_dmap, CV_32F, 1.0/16.0, 0.0);
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
    switch(method){
    case StereoProcessor::BM:
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
        break;
    case StereoProcessor::SGBM:
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
        break;
    case StereoProcessor::BM_GPU:
        cfgBM_GPU.preFilterSize = preFilterSize;
        cfgBM_GPU.preFilterCap = preFilterCap;
        cfgBM_GPU.SADWindowSize = sadWindowSize;
        cfgBM_GPU.minDisparity = minDisparity;
        cfgBM_GPU.numberOfDisparities = numOfDisparities;
        cfgBM_GPU.textureThreshold = textureThreshold;
        cfgBM_GPU.uniquenessRatio = uniquenessRatio;
        cfgBM_GPU.speckleRange = speckleWindowRange;
        cfgBM_GPU.speckleWindowSize = speckleWindowSize;
        cfgBM_GPU.disp12MaxDiff = disp12MaxDiff;
        break;
    }

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


