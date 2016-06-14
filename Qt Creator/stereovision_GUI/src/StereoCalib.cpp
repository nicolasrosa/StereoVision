/*
 * StereoCalib.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: nicolasrosa
 */

/* Libraries */
#include "inc/StereoCalib.h"

/* Constructor and Destructor */
StereoCalib::StereoCalib(){
    double focalLength=0;
    double baseline=0;

    needCalibration=false;
    hasQMatrix=false;
}


StereoCalib::~StereoCalib(){}


/* Instance Methods */
/*** Stereo Calibration function
  ** Description: Reads the Calibrations in *.yml files
  ** Receives:    Matrices Addresses for storage
  ** @param Mat M1,M2: Intrinsic Matrices from camera 1 and 2
  ** @param Mat D1,D2: Distortion Coefficients from camera 1 and 2
  ** @param Mat R: Rotation Matrix
  ** @param Mat t: Translation Vector
  ** Returns:     Nothing
  ***/
void StereoCalib::readCalibrationFiles(){
    cout << "Calibration: ON\n" << endl;

    readIntrinsicsFile();
    readExtrinsicsFile();

    /* Checking if the Reading Process was Successful */
    if(!M1.data || !D1.data || !M2.data || !D2.data || !R.data || !T.data){
        cerr << "Check instrinsics and extrinsics Matrixes content!" << endl;
        return;
    }

    /* Console Output */
    cout << "------------------------------ Intrinsics --------------------------------------" << endl;
    cout << "M1: " << endl << M1 << endl;
    cout << "D1: " << endl << D1 << endl;
    cout << "M2: " << endl << M2 << endl;
    cout << "D2: " << endl << D2 << endl << endl;
    cout << "intrinsics.yml Read Successfully."  << endl << endl;

    cout << "------------------------------ Extrinsics --------------------------------------" << endl;
    cout << "R: " << endl << R << endl;
    cout << "T: " << endl << T << endl << endl;
    cout << "extrinsics.yml Read Successfully.\n"  << endl;
    cout << "--------------------------------------------------------------------------------" << endl << endl;
}

void StereoCalib::readIntrinsicsFile(){
    FileStorage fs(intrinsicsFileName, FileStorage::READ);
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

void StereoCalib::readExtrinsicsFile(){
    FileStorage fs(extrinsicsFileName, FileStorage::READ);
    if(!fs.isOpened()){
        cerr << "Failed to open extrinsics.yml file!" << endl;
        return;
    }

    fs["R"] >> R;
    fs["T"] >> T;

    fs.release();
}

void StereoCalib::createKMatrix(){
    K=Mat::eye(3,3,CV_64F);
    K.at<double>(0,0)=focalLength;
    K.at<double>(1,1)=focalLength;
    K.at<double>(0,2)=(resolution.width-1.0)/2.0;
    K.at<double>(1,2)=(resolution.height-1.0)/2.0;
    //K.at<double>(0,2)=(0-1.0)/2.0;
    //K.at<double>(1,2)=(0-1.0)/2.0;

    cout << "K:" << endl << K << endl;
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
    FileStorage fs(QmatrixFileName, FileStorage::READ);

    if(resolutionDesired.width == 640 && resolutionDesired.height == 480){
        if(!fs.isOpened()){
            cerr << "Failed to open Q.yml file" << endl;
            return;
        }

        fs["Q"] >> Q;

        fs.release();

        /* Checking if the Reading Process was Successful */
        if(!Q.data){
            cerr << "Check Q Matrix Content!" << endl;
            return;
        }

        /* Console Output */
        cout << "Q:" << endl << Q << endl;

        focalLength = Q.at<double>(2,3);        cout << "f:" << focalLength << endl;
        baseline = -1.0/Q.at<double>(3,2);      cout << "baseline: " << baseline << endl;
    }else{
        cerr << "Check Q.yml file!\n" << endl;
        return;
    }
}

/*** Calculate Q Matrix function
  ** Description: Calculates the Q Matrix in the *.yml file
  ** Receives:    Matrices Addresses for storage
  ** Returns:     Nothing
  **
  ** Perspective transformation matrix(Q)
  ** [ 1  0    0	   -cx     ]
  ** [ 0  1    0 	   -cy     ]
  ** [ 0  0    0		f      ]
  ** [ 0  0  -1/Tx 	(cx-cx')/Tx]
  ***/
void StereoCalib::calculateQMatrix(){
    Q = Mat::eye(4,4,CV_64F);
    Q.at<double>(0,3)=-imageCenter.x;
    Q.at<double>(1,3)=-imageCenter.y;
    Q.at<double>(2,2)=0.0;

    Q.at<double>(2,3)=focalLength;
    Q.at<double>(3,2)=1.0/baseline;
    Q.at<double>(3,3)=0.0;

    cout << "Q:" << endl << Q << endl;

    cout << "f: " << focalLength << endl;
}

void StereoCalib::setResolution(int width, int height){
    resolution.width = width;
    resolution.height = height;
}

void StereoCalib::setResolution(Size matrixSize){
    resolution = matrixSize;
}

void StereoCalib::setResolutionDesired(int width, int height){
    resolutionDesired.width = width;
    resolutionDesired.height = height;
}

void StereoCalib::setResolutionDesired(Size matrixSize){
    resolutionDesired = matrixSize;
}

Size StereoCalib::getResolution(){
    return(resolution);
}

Size StereoCalib::getResolutionDesired(){
    return(resolutionDesired);
}

int StereoCalib::getResolution_width(){
    return(resolution.width);
}

int StereoCalib::getResolution_height(){
    return(resolution.height);
}

int StereoCalib::getResolutionDesired_width(){
    return(resolutionDesired.width);
}

int StereoCalib::getResolutionDesired_height(){
    return(resolutionDesired.height);
}
