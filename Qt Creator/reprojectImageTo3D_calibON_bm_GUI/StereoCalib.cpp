/*
 * StereoCalib.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: nicolasrosa
 */

#include "StereoCalib.h"

/* Constructor */
StereoCalib::StereoCalib(){}

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

    if(this->imageSizeDesired.width == 640 && this->imageSizeDesired.height == 480){
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
