/*
 * StereoProcessor.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: nicolasrosa
 */

#include "StereoProcessor.h"

/* Begin: Constructors */
StereoProcessor::StereoProcessor(int number){
    inputNum=number;
}

StereoCalib::StereoCalib(){}

StereoDisparityMap::StereoDisparityMap(){
    // Allocate Memory
    //Mat disp     = Mat(imageR[0].rows, imageR[0].cols, CV_16UC1);
    //Mat disp_8U  = Mat(stereo->imageR[0].rows, stereo->imageR[0].cols, CV_8UC1);
    //Mat disp_BGR = Mat(stereo->imageR[0].rows, stereo->imageR[0].cols, CV_8UC3);
}

/* End: Constructors */


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

    fs.release();

    cout << "------------------------------Config.yml------------------------------" << endl;
    cout << "Intrinsics Path: " << this->calib.intrinsicsFileName << endl;
    cout << "Extrinsics Path: " << this->calib.extrinsicsFileName << endl;
    cout << "Q Matrix Path: "   << this->calib.QmatrixFileName    << endl;
    cout << "Config.yml Read Successfully." << endl << endl ;
    cout << "----------------------------------------------------------------------" << endl;
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

void StereoProcessor::createKMatrix(){
    this->calib.K=Mat::eye(3,3,CV_64F);
    this->calib.K.at<double>(0,0)=this->calib.focalLength;
    this->calib.K.at<double>(1,1)=this->calib.focalLength;
    //this->calib.K.at<double>(0,2)=(this->imageSize.width-1.0)/2.0;
    //this->calib.K.at<double>(1,2)=(this->imageSize.height-1.0)/2.0;
    this->calib.K.at<double>(0,2)=(0-1.0)/2.0;
    this->calib.K.at<double>(1,2)=(0-1.0)/2.0;
    cout << "K:" << endl << this->calib.K << endl;
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

void StereoProcessor::readQMatrix(){
    FileStorage fs(this->calib.QmatrixFileName, FileStorage::READ);

    if(this->calib.is640x480){
        if(!fs.isOpened()){
            cerr << "Failed to open Q.yml file" << endl;
            return;
        }

        fs["Q"] >> this->calib.Q;
        // Check
        if(!this->calib.Q.data){
            cerr << "Check Q Matrix Content!" << endl;
            return;
        }

        // Display
        cout << "Q:" << endl << this->calib.Q << endl;

        this->calib.focalLength = this->calib.Q.at<double>(2,3);  cout << "f:" << this->calib.focalLength << endl;
        this->calib.baseline = -1.0/this->calib.Q.at<double>(3,2); cout << "baseline: " << this->calib.baseline << endl;
    }else{
        cerr << "Check Q.yml file!\n" << endl;
        return;
    }
}

void StereoProcessor::calculateQMatrix(){
    this->calib.Q = Mat::eye(4,4,CV_64F);
    this->calib.Q.at<double>(0,3)=-this->imageCenter.x;
    this->calib.Q.at<double>(1,3)=-this->imageCenter.y;
    this->calib.Q.at<double>(2,3)=this->calib.focalLength;
    this->calib.Q.at<double>(3,3)=0.0;
    this->calib.Q.at<double>(2,2)=0.0;
    this->calib.Q.at<double>(3,2)=1.0/this->calib.baseline;
    cout << "Q:" << endl << this->calib.Q << endl;
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
void StereoProcessor::stereoSetParams(){
    int trackbarsAux[10];

    trackbarsAux[0]= getTrackbarPos("preFilterSize",trackbarWindowName)*2.5+5;
    trackbarsAux[1]= getTrackbarPos("preFilterCap",trackbarWindowName)*0.625+1;
    trackbarsAux[2]= getTrackbarPos("SADWindowSize",trackbarWindowName)*2.5+5;
    trackbarsAux[3]= getTrackbarPos("minDisparity",trackbarWindowName)*2.0-100;
    trackbarsAux[4]= getTrackbarPos("numberOfDisparities",trackbarWindowName)*16;
    trackbarsAux[5]= getTrackbarPos("textureThreshold",trackbarWindowName)*320;
    trackbarsAux[6]= getTrackbarPos("uniquenessRatio",trackbarWindowName)*2.555;
    trackbarsAux[7]= getTrackbarPos("speckleWindowSize",trackbarWindowName)*1.0;
    trackbarsAux[8]= getTrackbarPos("speckleRange",trackbarWindowName)*1.0;
    trackbarsAux[9]= getTrackbarPos("disp12MaxDiff",trackbarWindowName)*1.0;

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
