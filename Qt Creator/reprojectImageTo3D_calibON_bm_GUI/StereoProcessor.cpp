/*
 * StereoProcessor.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: nicolasrosa
 */

#include "StereoProcessor.h"

//Constructor
StereoProcessor::StereoProcessor(int number){
    inputNum=number;
}

//Constructor
StereoCalib::StereoCalib(){}

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

    cout << "Intrinsics Path: " << this->calib.intrinsicsFileName << endl;
    cout << "Extrinsics Path: " << this->calib.extrinsicsFileName << endl;
    cout << "Q Matrix Path: "   << this->calib.QmatrixFileName    << endl;
    cout << "Config.yml Read Successfully." << endl << endl ;
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
    if(this->calib.M1.cols==0 || this->calib.D1.cols==0 || this->calib.M2.cols==0 || this->calib.D2.cols==0 || this->calib.R.cols==0 || this->calib.T.cols==0){
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
        if(this->calib.Q.cols==0){
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
