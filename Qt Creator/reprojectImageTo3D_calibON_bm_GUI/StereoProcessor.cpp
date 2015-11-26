/*
 * StereoProcessor.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: nicolasrosa
 */

#include "StereoProcessor.h"
#include "trackObject.h"

/* Begin: Constructors */


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
