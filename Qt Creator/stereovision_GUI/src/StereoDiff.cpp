/*
 * StereoDiff.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: nicolasrosa
 */

/* Libraries */
#include "inc/StereoDiff.h"

/* Constructor and Destructor */
StereoDiff::StereoDiff(){
    StartDiff=false;
    alpha = 0.5;
    beta = (1.0-alpha);
    gamma = 0.0;
}

StereoDiff::~StereoDiff(){}

void StereoDiff::createDiffImage(Mat input1, Mat input2){
    absdiff(input1,input2,diffImage);
}

void StereoDiff::createResAND(Mat input1,Mat input2){
    bitwise_and(input1,input2,res_AND);
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

    addWeighted(imageL,alpha,res_AND_BGR,beta, gamma,res_ADD);

    //imshow("Add",res_ADD);
}
