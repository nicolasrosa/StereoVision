/*
 * StereoProcessor.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: nicolasrosa
 */

#include "StereoProcessor.h"

StereoProcessor::StereoProcessor(int number){
    inputNum=number;
}

int StereoProcessor::getInputNum(){
    return inputNum;
}

void StereoProcessor::readConfigFile(){
    FileStorage fs("../reprojectImageTo3D_calibON_bm_GUI/config.yml", FileStorage::READ);
    if(!fs.isOpened()){
        printf("Failed to open config.yml file.\n");
        return;
    }
    fs["Intrinsics Path"] >> this->intrinsicsFileName;
    fs["Extrinsics Path"] >> this->extrinsicsFileName;
    fs["Q Matrix Path"]   >> this->QmatrixFileName;

    fs.release();

    cout << "Intrinsics Path: " << this->intrinsicsFileName << endl;
    cout << "Extrinsics Path: " << this->extrinsicsFileName << endl;
    cout << "Q Matrix Path: "   << this->QmatrixFileName    << endl;
    cout << "Config.yml Read Successfully." << endl << endl ;
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

void StereoProcessor::readQMatrix(Mat &Q,double* focalLength,double* baseline,bool is640x480){
    FileStorage fs(this->QmatrixFileName, FileStorage::READ);

    if(is640x480){
        if(!fs.isOpened()){
            printf("Failed to open Q.yml file\n");
            return;
        }

        fs["Q"] >> Q;
        cout << "Q:" << endl << Q << endl;

        *focalLength = Q.at<double>(2,3);  cout << "f:" << *focalLength << endl;
        *baseline = -1.0/Q.at<double>(3,2); cout << "baseline: " << *baseline << endl;
    }else{
        printf("Check Correct Q.yml file\n");
        return;
    }

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
