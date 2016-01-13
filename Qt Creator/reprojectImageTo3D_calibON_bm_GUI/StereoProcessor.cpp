/*
 * StereoProcessor.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: nicolasrosa
 */

#include "trackObject.h"
#include "mainwindow.h"

/* Constructor */
#include "StereoProcessor.h"

StereoProcessor::StereoProcessor(int number) {
    inputNum=number;
    frameCounter=0;
    isVideoFile=false;
    isImageFile=false;
}

//void StereoProcessor::openStereoSource(int inputNum){
//    string imageL_filename;
//    string imageR_filename;

//    new ui = MainWindow::ui;

//    // Create an object that decodes the input Video stream.
//    cout << "Enter Video Number(1,2,3,4,5,6,7,8,9): " << endl;
//    ui->txtOutputBox->appendPlainText(QString("Enter Video Number(1,2,3,4,5,6,7,8,9): "));
//    //	scanf("%d",&inputNum);
//    cout << "Input File: " << inputNum << endl;
//    ui->txtOutputBox->appendPlainText(QString("Input File: ")+QString::number(inputNum));
//    switch(inputNum){
//    case 1:
//        imageL_filename = "../../workspace/data/video10_l.avi";
//        imageR_filename = "../../workspace/data/video10_r.avi";
//        needCalibration=true;
//        //ui->txtOutputBox->appendPlainText(QString("video2_denoised_long.avi"));
//        break;
//    case 2:
//        imageL_filename = "../../workspace/data/video12_l.avi";
//        imageR_filename = "../../workspace/data/video12_r.avi";
//        needCalibration=true;
//        //ui->txtOutputBox->appendPlainText(QString( "video0.avi"));
//        break;
//    case 3:
//        imageL_filename = "../data/left/video1.avi";
//        imageR_filename = "../data/right/video1.avi";
//        needCalibration=true;
//        //ui->txtOutputBox->appendPlainText(QString( "video1.avi"));
//        break;
//    case 4:
//        imageL_filename = "../data/left/video2_noised.avi";
//        imageR_filename = "../data/right/video2_noised.avi";
//        needCalibration=true;
//        //ui->txtOutputBox->appendPlainText(QString( "video2_noised.avi"));
//        break;
//    case 5:
//        imageL_filename = "../data/left/20004.avi";
//        imageR_filename = "../data/right/30004.avi";
//        needCalibration=false;
//        break;
//    case 6:
//        imageL_filename = "../../workspace/data/left/video15.avi";
//        imageR_filename = "../../workspace/data/right/video15.avi";
//        needCalibration=true;
//        break;
//    case 7:
//        imageL_filename = "../../workspace/data/left/left1.png";
//        imageR_filename = "../../workspace/data/right/right1.png";
//        needCalibration=false;
//        break;
//    case 8:
//        imageL_filename = "../data/left/left2.png";
//        imageR_filename = "../data/right/right2.png";
//        needCalibration=false;
//        break;
//    case 9:
//        imageL_filename = "../data/left/left3.png";
//        imageR_filename = "../data/right/right3.png";
//        needCalibration=false;
//        break;
//    }

//    if(imageL_filename.substr(imageL_filename.find_last_of(".") + 1) == "avi"){
//        cout << "It's a Video file" << endl;
//        ui->txtOutputBox->appendPlainText(QString("It's a Video file"));
//        isVideoFile=true;

//        stereo->capL.open(imageL_filename);
//        stereo->capR.open(imageR_filename);

//        if(!stereo->capL.isOpened() || !stereo->capR.isOpened()){		// Check if we succeeded
//            cerr <<  "Could not open or find the input videos!" << endl ;
//            ui->txtOutputBox->appendPlainText(QString( "Could not open or find the input videos!"));
//            //return -1;
//        }

//        cout << "Input 1 Resolution: " << stereo->capR.get(CV_CAP_PROP_FRAME_WIDTH) << "x" << stereo->capR.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
//        cout << "Input 2 Resolution: " << stereo->capL.get(CV_CAP_PROP_FRAME_WIDTH) << "x" << stereo->capL.get(CV_CAP_PROP_FRAME_HEIGHT) << endl << endl;
//        ui->txtOutputBox->appendPlainText(QString("Input 1 Resolution: ") + QString::number(stereo->capL.get(CV_CAP_PROP_FRAME_WIDTH)) + QString("x") + QString::number(stereo->capL.get(CV_CAP_PROP_FRAME_HEIGHT)));
//        ui->txtOutputBox->appendPlainText(QString("Input 2 Resolution: ") + QString::number(stereo->capR.get(CV_CAP_PROP_FRAME_WIDTH)) + QString("x") + QString::number(stereo->capR.get(CV_CAP_PROP_FRAME_HEIGHT)));
//    }else{
//        cout << "It is not a Video file" << endl;
//        ui->txtOutputBox->appendPlainText(QString( "It is not a Video file"));
//        if(imageL_filename.substr(imageL_filename.find_last_of(".") + 1) == "jpg" || imageL_filename.substr(imageL_filename.find_last_of(".") + 1) == "png"){
//            cout << "It's a Image file" << endl;
//            ui->txtOutputBox->appendPlainText(QString( "It's a Image file"));
//            isImageFile=true;

//            stereo->imageL[0] = imread(imageL_filename, CV_LOAD_IMAGE_COLOR);	// Read the file
//            stereo->imageR[0] = imread(imageR_filename, CV_LOAD_IMAGE_COLOR);	// Read the file

//            if(!stereo->imageL[0].data || !stereo->imageR[0].data){                     // Check for invalid input
//                ui->txtOutputBox->appendPlainText(QString("Could not open or find the input images!"));
//                return;
//            }
//        }else{
//            cout << "It is not a Image file" << endl;
//            ui->txtOutputBox->appendPlainText(QString( "It is not a Image file"));
//        }
//    }
//}

int StereoProcessor::getInputNum(){
    return inputNum;
}

void StereoProcessor::readConfigFile(){
    //FileStorage fs("../reprojectImageTo3D_calibON_bm_GUI/config.yml", FileStorage::READ);
    FileStorage fs("/home/nicolas/repository/StereoVision/Qt Creator/reprojectImageTo3D_calibON_bm_GUI/config.yml", FileStorage::READ);

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

    if(this->flags.methodBM)
        this->bm->compute(this->imageL_grey[0],this->imageR_grey[0],this->disp.disp_16S);

    if(this->flags.methodSGBM)
        this->sgbm->compute(this->imageL[0],this->imageR[0],this->disp.disp_16S);

    normalize(this->disp.disp_16S, this->disp.disp_8U, 0, 255, CV_MINMAX, CV_8U);
    applyColorMap(this->disp.disp_8U,this->disp.disp_BGR, COLORMAP_JET);
}

void StereoProcessor::imageProcessing(Mat src, Mat imgE, Mat imgED,Mat cameraFeedL,bool isTrackingObjects){
    Mat erosionElement = getStructuringElement( MORPH_RECT,Size( 2*EROSION_SIZE + 1, 2*EROSION_SIZE+1 ),Point( EROSION_SIZE, EROSION_SIZE ) );
    Mat dilationElement = getStructuringElement( MORPH_RECT,Size( 2*DILATION_SIZE + 1, 2*DILATION_SIZE+1 ),Point( DILATION_SIZE, DILATION_SIZE ) );
    Mat imgEBGR,imgEDBGR;
    Mat imgEDMedian,imgEDMedianBGR;
    int x,y;

    //Mat imgThreshold;
    static Mat lastimgThreshold;
    int nPixels,nTotal;		  	//static int lastThresholdSum=0;

    // Near Object Detection

    //Prefiltering
    // Apply Erosion and Dilation to take out spurious noise
    erode(src,imgE,erosionElement);
    dilate(imgE,imgED,erosionElement);

    applyColorMap(imgE,imgEBGR, COLORMAP_JET);
    applyColorMap(imgED,imgEDBGR, COLORMAP_JET);

    // Apply Median Filter
    //GaussianBlur(imgED,imgEDMedian,Size(3,3),0,0);
    medianBlur(imgED,imgEDMedian,5);
    applyColorMap(imgEDMedian,imgEDMedianBGR, COLORMAP_JET);

    // Thresholding
    int T_Otsu = threshold(imgEDMedian, imgThreshold, 0, 255, THRESH_BINARY | THRESH_OTSU);
    erode(imgThreshold,imgThreshold,erosionElement);
    dilate(imgThreshold,imgThreshold,dilationElement);

    // Solving Lighting Noise Problem
    nPixels = sum(imgThreshold)[0]/255;
    nTotal = imgThreshold.total();

    //	cout << "Number of Pixels:" << nPixels << endl;
    //	cout << "Ratio is: " << ((float)nPixels)/nTotal << endl << endl;

    if((((float)nPixels)/nTotal)>0.5){
        //		sleep(1);
        //		cout << "Lighting Noise!!!" << endl;
        //		cout << "Number of Pixels:" << nPixels << endl;
        //		cout << "Ratio is: " << ((float)nPixels)/nTotal << endl << endl;

        // Invalidates the last frame
        imgThreshold = lastimgThreshold;
    }else{
        // Saves the last valid frame
        lastimgThreshold=imgThreshold;
        //lastThresholdSum = CurrentThresholdSum;
    }

    // Output
    //imshow("Eroded Image",imgE);
    //imshow("Eroded Image BGR",imgEBGR);
    //imshow("Eroded+Dilated Image",imgED);
    //imshow("Eroded+Dilated Image BGR",imgEDBGR);
    //imshow("Eroded+Dilated+Median Image",imgEDMedian);
    //imshow("Eroded+Dilated+Median Image BGR",imgEDMedianBGR);

    //imshow("Thresholded Image",imgThreshold);

    // Tracking Object
    if(isTrackingObjects){
        cameraFeedL.copyTo(trackingView);
        if(isVideoFile){
            trackFilteredObject(x,y,imgThreshold,trackingView);
            //imshow("Tracking Object",trackingView);
        }
    }

    imgThreshold.copyTo(imgThresholdDraw);
    putText(imgThresholdDraw,"T: "+intToString(T_Otsu),Point(0,25),1,1,Scalar(255,255,255),2);
}

//Saving Previous Frame
void StereoProcessor::saveLastFrames(){
    imageL[0].copyTo(imageL[1]);
    imageR[0].copyTo(imageR[1]);
    imageL_grey[0].copyTo(imageL_grey[1]);
    imageR_grey[0].copyTo(imageR_grey[1]);
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

    if(frameCounter == capR.get(CV_CAP_PROP_FRAME_COUNT)){
        frameCounter = 0;
        capL.set(CV_CAP_PROP_POS_FRAMES,0);
        capR.set(CV_CAP_PROP_POS_FRAMES,0);
    }
}



