/*
 * StereoCalib.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: nicolasrosa
 */

#include "inc/StereoCalib.h"

/* Constructor */
StereoCalib::StereoCalib(){
    double focalLength=0;
    double baseline=0;

    isVideoFile=false;
    isImageFile=false;

    needCalibration=false;
    hasQMatrix=false;
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
void StereoCalib::readCalibrationFiles(){
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

void StereoCalib::readIntrinsicsFile(){
    FileStorage fs(this->intrinsicsFileName, FileStorage::READ);
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

void StereoCalib::readExtrinsicsFile(){
    FileStorage fs(this->extrinsicsFileName, FileStorage::READ);
    if(!fs.isOpened()){
        cerr << "Failed to open extrinsics.yml file!" << endl;
        return;
    }

    fs["R"] >> this->R;
    fs["T"] >> this->T;

    fs.release();
}

void StereoCalib::createKMatrix(){
    this->K=Mat::eye(3,3,CV_64F);
    this->K.at<double>(0,0)=this->focalLength;
    this->K.at<double>(1,1)=this->focalLength;
    this->K.at<double>(0,2)=(this->imageSize.width-1.0)/2.0;
    this->K.at<double>(1,2)=(this->imageSize.height-1.0)/2.0;
    //this->K.at<double>(0,2)=(0-1.0)/2.0;
    //this->K.at<double>(1,2)=(0-1.0)/2.0;

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

    if(this->imageSizeDesired.width == 640 && this->imageSizeDesired.height == 480){
        if(!fs.isOpened()){
            cerr << "Failed to open Q.yml file" << endl;
            return;
        }
        fs["Q"] >> this->Q;

        /* Checking if the Reading Process was Successful */
        if(!this->Q.data){
            cerr << "Check Q Matrix Content!" << endl;
            return;
        }

        /* Console Output */
        cout << "Q:" << endl << this->Q << endl;

        this->focalLength = this->Q.at<double>(2,3);  cout << "f:" << this->focalLength << endl;
        this->baseline = -1.0/this->Q.at<double>(3,2); cout << "baseline: " << this->baseline << endl;
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
    this->Q = Mat::eye(4,4,CV_64F);
    this->Q.at<double>(0,3)=-this->imageCenter.x;
    this->Q.at<double>(1,3)=-this->imageCenter.y;
    this->Q.at<double>(2,2)=0.0;

    this->Q.at<double>(2,3)=this->focalLength;
    this->Q.at<double>(3,2)=1.0/this->baseline;
    this->Q.at<double>(3,3)=0.0;

    cout << "Q:" << endl << this->Q << endl;

    cout << "f: " << this->focalLength << endl;
}
