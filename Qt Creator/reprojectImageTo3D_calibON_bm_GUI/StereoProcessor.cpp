/*
 * StereoProcessor.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: nicolasrosa
 */

#include "StereoProcessor.h"
#include "trackObject.h"

/* Begin: Constructors */
StereoProcessor::StereoProcessor(int number){
    inputNum=number;
}

StereoFlags::StereoFlags(){
    showInputImages=true;
    showXYZ=false;
    showStereoParam=false;
    showStereoParamValues=false;
    showFPS=false;
    showDisparityMap=false;
    show3Dreconstruction=false;
    showTrackingObjectView=false;
    showDiffImage=false;
    showWarningLines=false;
}

StereoDiff::StereoDiff(){
    StartDiff=false;
    alpha = 0.5;
    beta = (1.0-alpha);
}

StereoConfig::StereoConfig(){}

StereoCalib::StereoCalib(){}

StereoDisparityMap::StereoDisparityMap(){
    // Allocate Memory
    //Mat disp     = Mat(imageR[0].rows, imageR[0].cols, CV_16UC1);
    //Mat disp_8U  = Mat(stereo->imageR[0].rows, stereo->imageR[0].cols, CV_8UC1);
    //Mat disp_BGR = Mat(stereo->imageR[0].rows, stereo->imageR[0].cols, CV_8UC3);
}

/* End: Constructors */

void StereoDiff::createDiffImage(Mat input1, Mat input2){
    absdiff(input1,input2,diffImage);
}

void StereoDiff::createResAND(Mat input1,Mat input2){
    bitwise_and(input1,input2,this->res_AND);
}

void StereoDiff::convertToBGR(){
    cvtColor(res_AND,res_AND_BGR,CV_GRAY2BGR);
}

void StereoDiff::addRedLines(){
    split(res_AND_BGR,res_AND_BGR_channels);

    //Set the Blue and Green Channels to 0
    res_AND_BGR_channels[0] = Mat::zeros(res_AND.rows,res_AND.cols,CV_8UC1);
    res_AND_BGR_channels[1] = Mat::zeros(res_AND.rows,res_AND.cols,CV_8UC1);
    cv::merge(res_AND_BGR_channels,3,res_AND_BGR);

    addWeighted(this->imageL,alpha,res_AND_BGR,beta, 0.0,res_ADD);

    //imshow("Add",res_ADD);
}

//void StereoConfig::setInitialTrackbarsValues(){
//    this->setInitialStereoParamsUi(this->preFilterSize,
//                                   this->preFilterCap,
//                                   this->SADWindowSize,
//                                   this->minDisparity,
//                                   this->numberOfDisparities,
//                                   this->textureThreshold,
//                                   this->uniquenessRatio,
//                                   this->speckleWindowSize,
//                                   this->speckleRange,
//                                   this->disp12MaxDiff);
//}

int StereoProcessor::getInputNum(){
    return inputNum;
}

void StereoProcessor::readConfigFile(){
    FileStorage fs("../reprojectImageTo3D_calibON_bm_GUI/config.yml", FileStorage::READ);
    if(!fs.isOpened()){
        cerr << "Failed to open config.yml file!" << endl;
        return;
    }
    fs["Intrinsics Path"] >> this->calib.intrinsicsFileName;
    fs["Extrinsics Path"] >> this->calib.extrinsicsFileName;
    fs["Q Matrix Path"]   >> this->calib.QmatrixFileName;
    fs["Stereo Parameters Path"] >> this->calib.StereoParamFileName;

    fs.release();

    cout << "------------------------------Config.yml------------------------------" << endl;
    cout << "Intrinsics Path: "         << this->calib.intrinsicsFileName  << endl;
    cout << "Extrinsics Path: "         << this->calib.extrinsicsFileName  << endl;
    cout << "Q Matrix Path: "           << this->calib.QmatrixFileName     << endl;
    cout << "Stereo Parameters Path:"   << this->calib.StereoParamFileName << endl;
    cout << "Config.yml Read Successfully." << endl << endl ;
    cout << "----------------------------------------------------------------------" << endl;
}

void StereoProcessor::readStereoConfigFile(){
    FileStorage fs(this->calib.StereoParamFileName, FileStorage::READ);
    if(!fs.isOpened()){
        cerr << "Failed to open stereo.yml file!" << endl;
        return;
    }

    fs["preFilterSize"] >> this->stereocfg.preFilterSize;
    fs["preFilterCap"] >> this->stereocfg.preFilterCap;
    fs["SADWindowSize"] >> this->stereocfg.SADWindowSize;
    fs["minDisparity"] >> this->stereocfg.minDisparity;
    fs["numberOfDisparities"] >> this->stereocfg.numberOfDisparities;
    fs["textureThreshold"] >> this->stereocfg.textureThreshold;
    fs["uniquenessRatio"] >> this->stereocfg.uniquenessRatio;
    fs["speckleWindowSize"] >> this->stereocfg.speckleWindowSize;
    fs["speckleRange"] >> this->stereocfg.speckleRange;
    fs["disp12MaxDiff"] >> this->stereocfg.disp12MaxDiff;

    fs.release();

    // Display
    cout << "------------------------------StereoConfig----------------------------" << endl;
    cout << "preFilterSize: "       << this->stereocfg.preFilterSize          << endl;
    cout << "preFilterCap: "        << this->stereocfg.preFilterCap           << endl;
    cout << "SADWindowSize: "       << this->stereocfg.SADWindowSize          << endl;
    cout << "minDisparity: "        << this->stereocfg.minDisparity           << endl;
    cout << "numberOfDisparities: " << this->stereocfg.numberOfDisparities    << endl;
    cout << "textureThreshold: "    << this->stereocfg.textureThreshold       << endl;
    cout << "uniquenessRatio: "     << this->stereocfg.uniquenessRatio        << endl;
    cout << "speckleWindowSize: "   << this->stereocfg.speckleWindowSize      << endl;
    cout << "speckleRange: "        << this->stereocfg.speckleRange           << endl;
    cout << "disp12MaxDiff: "       << this->stereocfg.disp12MaxDiff          << endl;
    cout << "stereo.yml Read Successfully."  << endl << endl;
    cout << "----------------------------------------------------------------------" << endl << endl;
}

/*** Stereo Initialization function
  ** Description: Executes the PreSetup of parameters of the StereoBM object
  ** @param StereoBM bm: Correspondence Object
  ** Returns:     Nothing
  ***/
void StereoProcessor::stereoInit(){
    this->bm->setPreFilterCap(31);
    this->bm->setBlockSize(25 > 0 ? 25 : 9);
    this->bm->setMinDisparity(0);
    this->bm->setNumDisparities(3);
    this->bm->setTextureThreshold(10);
    this->bm->setUniquenessRatio(15);
    this->bm->setSpeckleWindowSize(100);
    this->bm->setSpeckleRange(32);
    this->bm->setDisp12MaxDiff(1);
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
void StereoProcessor::stereoCalib(){
    FileStorage fs(this->calib.intrinsicsFileName, FileStorage::READ);
    if(!fs.isOpened()){
        cerr << "Failed to open intrinsics.yml file!" << endl;
        return;
    }

    fs["M1"] >> this->calib.M1;
    fs["D1"] >> this->calib.D1;
    fs["M2"] >> this->calib.M2;
    fs["D2"] >> this->calib.D2;

    fs.release();

    float scale = 1.f;
    this->calib.M1 *= scale;
    this->calib.M2 *= scale;

    fs.open(this->calib.extrinsicsFileName, FileStorage::READ);
    if(!fs.isOpened()){
        cerr << "Failed to open extrinsics.yml file!" << endl;
        return;
    }

    fs["R"] >> this->calib.R;
    fs["T"] >> this->calib.T;

    fs.release();

    // Check
    if(!this->calib.M1.data || !this->calib.D1.data || !this->calib.M2.data || !this->calib.D2.data || !this->calib.R.data || !this->calib.T.data){
        cerr << "Check instrinsics and extrinsics Matrixes content!" << endl;
        return;
    }

    // Display
    cout << "------------------------------Intrinsics------------------------------" << endl;
    cout << "M1: " << endl << this->calib.M1 << endl;
    cout << "D1: " << endl << this->calib.D1 << endl;
    cout << "M2: " << endl << this->calib.M2 << endl;
    cout << "D2: " << endl << this->calib.D2 << endl << endl;
    cout << "intrinsics.yml Read Successfully."  << endl << endl;

    cout << "------------------------------Extrinsics------------------------------" << endl;
    cout << "R: " << endl << this->calib.R << endl;
    cout << "T: " << endl << this->calib.T << endl << endl;
    cout << "extrinsics.yml Read Successfully."  << endl;
    cout << "----------------------------------------------------------------------" << endl << endl;
}

void StereoCalib::createKMatrix(){
    this->K=Mat::eye(3,3,CV_64F);
    this->K.at<double>(0,0)=this->focalLength;
    this->K.at<double>(1,1)=this->focalLength;
    //this->K.at<double>(0,2)=(this->imageSize.width-1.0)/2.0;
    //this->K.at<double>(1,2)=(this->imageSize.height-1.0)/2.0;
    this->K.at<double>(0,2)=(0-1.0)/2.0;
    this->K.at<double>(1,2)=(0-1.0)/2.0;
    cout << "K:" << endl << this->K << endl;
}

/*** Read Q Matrix function
  ** Description: Reads the Q Matrix in the *.yml file
  ** Receives:    Matrices Addresses for storage
  ** Returns:     Nothing
  **
  ** Perspective transformation matrix(Q)
  ** [ 1  0    0	   -cx     ]
  ** [ 0  1    0 	   -cy     ]
  ** [ 0  0    0		f      ]
  ** [ 0  0  -1/Tx 	(cx-cx')/Tx]
  ***/
void StereoCalib::readQMatrix(){
    FileStorage fs(this->QmatrixFileName, FileStorage::READ);

    if(this->is640x480){
        if(!fs.isOpened()){
            cerr << "Failed to open Q.yml file" << endl;
            return;
        }

        fs["Q"] >> this->Q;
        // Check
        if(!this->Q.data){
            cerr << "Check Q Matrix Content!" << endl;
            return;
        }

        // Display
        cout << "Q:" << endl << this->Q << endl;

        this->focalLength = this->Q.at<double>(2,3);  cout << "f:" << this->focalLength << endl;
        this->baseline = -1.0/this->Q.at<double>(3,2); cout << "baseline: " << this->baseline << endl;
    }else{
        cerr << "Check Q.yml file!\n" << endl;
        return;
    }
}

void StereoCalib::calculateQMatrix(){
    this->Q = Mat::eye(4,4,CV_64F);
    this->Q.at<double>(0,3)=-this->imageCenter.x;
    this->Q.at<double>(1,3)=-this->imageCenter.y;
    this->Q.at<double>(2,3)=this->focalLength;
    this->Q.at<double>(3,3)=0.0;
    this->Q.at<double>(2,2)=0.0;
    this->Q.at<double>(3,2)=1.0/this->baseline;
    cout << "Q:" << endl << this->Q << endl;
}

/*** Stereo Parameters Configuration function
  ** Description: Executes the setup of parameters of the StereoBM object by changing the trackbars
  ** @param rect roi1: Region of Interest 1
  ** @param rect roi2: Region of Interest 2
  ** @param StereoBM bm: Correspondence Object
  ** @param int numRows: Number of Rows of the input Images
  ** @param bool showStereoBMparams
  ** Returns:     Nothing
  ***/
void StereoProcessor::setStereoParams(){
    int trackbarsAux[10];

    trackbarsAux[0] = getTrackbarPos("preFilterSize",trackbarWindowName)*2.5+5;
    trackbarsAux[1] = getTrackbarPos("preFilterCap",trackbarWindowName)*0.625+1;
    trackbarsAux[2] = getTrackbarPos("SADWindowSize",trackbarWindowName)*2.5+5;
    trackbarsAux[3] = getTrackbarPos("minDisparity",trackbarWindowName)*2.0-100;
    trackbarsAux[4] = getTrackbarPos("numberOfDisparities",trackbarWindowName)*16;
    trackbarsAux[5] = getTrackbarPos("textureThreshold",trackbarWindowName)*320;
    trackbarsAux[6] = getTrackbarPos("uniquenessRatio",trackbarWindowName)*2.555;
    trackbarsAux[7] = getTrackbarPos("speckleWindowSize",trackbarWindowName)*1.0;
    trackbarsAux[8] = getTrackbarPos("speckleRange",trackbarWindowName)*1.0;
    trackbarsAux[9] = getTrackbarPos("disp12MaxDiff",trackbarWindowName)*1.0;

    //Tentativa 1
    //    trackbarsAux[0] = this->preFilterSize_slider->value()*2.5+5;
    //    trackbarsAux[1] = this->preFilterCap_slider->value()*0.625+1;
    //    trackbarsAux[2] = this->SADWindowSize_slider->value()*2.5+5;
    //    trackbarsAux[3] = this->minDisparity_slider->value()*2.0-100;
    //    trackbarsAux[4] = this->numberOfDisparities_slider->value()*16;
    //    trackbarsAux[5] = this->textureThreshold_slider->value()*320;
    //    trackbarsAux[6] = this->uniquenessRatio_slider->value()*2.555;
    //    trackbarsAux[7] = this->speckleWindowSize_slider->value()*1.0;
    //    trackbarsAux[8] = this->speckleRange_slider->value()*1.0;
    //    trackbarsAux[9] = this->disp12MaxDiff_slider->value()*1.0;

    //Tentativa 2
    //    trackbarsAux[0] = this->stereocfg.preFilterSize*2.5+5;
    //    trackbarsAux[1] = this->stereocfg.preFilterCap*0.625+1;
    //    trackbarsAux[2] = this->stereocfg.SADWindowSize*2.5+5;
    //    trackbarsAux[3] = this->stereocfg.minDisparity*2.0-100;
    //    trackbarsAux[4] = this->stereocfg.numberOfDisparities*16;
    //    trackbarsAux[5] = this->stereocfg.textureThreshold*320;
    //    trackbarsAux[6] = this->stereocfg.uniquenessRatio*2.555;
    //    trackbarsAux[7] = this->stereocfg.speckleWindowSize*1.0;
    //    trackbarsAux[8] = this->stereocfg.speckleRange*1.0;
    //    trackbarsAux[9] = this->stereocfg.disp12MaxDiff*1.0;

    //cout << "preFilterSize: "<< this->stereocfg.preFilterSize << " track0:" << this->stereocfg.preFilterSize*2.5+5 << endl;
    //cout << "preFilterCap:  "<< this->stereocfg.preFilterCap  << " track1:" << this->stereocfg.preFilterCap*2.5+5 << endl;

    this->bm->setROI1(this->calib.roi1);
    this->bm->setROI1(this->calib.roi2);

    this->numRows = imageL[0].rows;

    if(trackbarsAux[0]%2==1 && trackbarsAux[0]>=5 && trackbarsAux[0]<=255){
        //bm.state->preFilterSize = trackbarsAux[0];
        bm->setPreFilterSize(trackbarsAux[0]);
    }

    if(trackbarsAux[1]>=1 && trackbarsAux[1]<=63){
        //bm.state->preFilterCap = trackbarsAux[1];
        bm->setPreFilterCap(trackbarsAux[1]);
    }

    if(trackbarsAux[2]%2==1 && trackbarsAux[2]>=5  && trackbarsAux[2]<=255 && trackbarsAux[2]<=numRows){
        //bm.state->SADWindowSize = trackbarsAux[2];
        bm->setBlockSize(trackbarsAux[2]);
    }

    if(trackbarsAux[3]>=-100 && trackbarsAux[3]<=100){
        //bm.state->minDisparity = trackbarsAux[3];
        bm->setMinDisparity(trackbarsAux[3]);
    }

    if(trackbarsAux[4]%16==0 && trackbarsAux[4]>=16 && trackbarsAux[4]<=256){
        //bm.state->numberOfDisparities = trackbarsAux[4];
        bm->setNumDisparities(trackbarsAux[4]);
    }

    if(trackbarsAux[5]>=0 && trackbarsAux[5]<=32000){
        //bm.state->textureThreshold = trackbarsAux[5];
        bm->setTextureThreshold(trackbarsAux[5]);
    }

    if(trackbarsAux[6]>=0 && trackbarsAux[6]<=255){
        //bm.state->uniquenessRatio = trackbarsAux[6];
        bm->setUniquenessRatio(trackbarsAux[6]);
    }

    if(trackbarsAux[7]>=0 && trackbarsAux[7]<=100){
        //bm.state->speckleWindowSize = trackbarsAux[7];
        bm->setSpeckleWindowSize(trackbarsAux[7]);
    }

    if(trackbarsAux[8]>=0 && trackbarsAux[8]<=100){
        //bm.state->speckleRange = trackbarsAux[8];
        bm->setSpeckleRange(trackbarsAux[8]);
    }

    if(trackbarsAux[9]>=0 && trackbarsAux[9]<=100){
        //bm.state->disp12MaxDiff = trackbarsAux[9];
        bm->setDisp12MaxDiff(trackbarsAux[9]);
    }

    if(showStereoParamsValues){
        cout << getTrackbarPos("preFilterSize",trackbarWindowName)			<< "\t" << trackbarsAux[0] << endl;
        cout << getTrackbarPos("preFilterCap",trackbarWindowName)			<< "\t" << trackbarsAux[1] << endl;
        cout << getTrackbarPos("SADWindowSize",trackbarWindowName)			<< "\t" << trackbarsAux[2] << endl;
        cout << getTrackbarPos("minDisparity",trackbarWindowName)			<< "\t" << trackbarsAux[3] << endl;
        cout << getTrackbarPos("numberOfDisparities",trackbarWindowName)	<< "\t" << trackbarsAux[4] << endl;
        cout << getTrackbarPos("textureThreshold",trackbarWindowName)		<< "\t" << trackbarsAux[5] << endl;
        cout << getTrackbarPos("uniquenessRatio",trackbarWindowName)		<< "\t" << trackbarsAux[6] << endl;
        cout << getTrackbarPos("speckleWindowSize",trackbarWindowName)		<< "\t" << trackbarsAux[7] << endl;
        cout << getTrackbarPos("speckleRange",trackbarWindowName)			<< "\t" << trackbarsAux[8] << endl;
        cout << getTrackbarPos("disp12MaxDiff",trackbarWindowName)			<< "\t" << trackbarsAux[9] << endl;
    }
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
    //adaptiveThreshold(imgEDMedian,imgThreshold,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,11,-1);
    //adaptiveThreshold(imgEDMedian,imgThreshold,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,11,0);
    threshold(imgEDMedian, imgThreshold, THRESH_VALUE, 255,THRESH_BINARY);
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
        trackFilteredObject(x,y,imgThreshold,trackingView);
        //imshow("Tracking Object",trackingView);
    }
}

//Saving Previous Frame
void StereoProcessor::saveLastFrames(){
    imageL[0].copyTo(imageL[1]);
    imageR[0].copyTo(imageR[1]);
    imageL_grey[0].copyTo(imageL_grey[1]);
    imageR_grey[0].copyTo(imageR_grey[1]);
}
