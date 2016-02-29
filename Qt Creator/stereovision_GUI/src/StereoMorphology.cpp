/*
 * StereoMorphology.cpp
 *
 *  Created on: Feb 29, 2016
 *      Author: nicolasrosa
 */

#include "inc/StereoMorphology.h"
#include "inc/trackObject.h"

/* Constructor */
StereoMorphology::StereoMorphology(){

}

void StereoMorphology::imageProcessing(Mat src, Mat imgE, Mat imgED,Mat cameraFeedL,bool isTrackingObjects,bool isVideoFile){
    //FIXME: Mudar colocar a declacao dos elementos abaixo dentro da classe StereoMorphology
    Mat erosionElement  = getStructuringElement(MORPH_RECT,Size(2*EROSION_SIZE +1, 2*EROSION_SIZE+1 ),Point(EROSION_SIZE,  EROSION_SIZE ));
    Mat dilationElement = getStructuringElement(MORPH_RECT,Size(2*DILATION_SIZE+1, 2*DILATION_SIZE+1),Point(DILATION_SIZE, DILATION_SIZE));
    Mat imgEBGR,imgEDBGR;
    Mat imgEDMedian,imgEDMedianBGR;
    int x,y;

    //Mat imgThreshold;
    static Mat lastImgThreshold;
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

    //TODO: Solve Lighting Noise Problem
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
        imgThreshold = lastImgThreshold;
    }else{
        // Saves the last valid frame
        lastImgThreshold=imgThreshold;
        //lastThresholdSum = CurrentThresholdSum;
    }

         imshow("imgThreshold",imgThreshold);



    // Tracking Object
    if(isTrackingObjects){
        cameraFeedL.copyTo(trackingView);
        if(isVideoFile){
           trackFilteredObject(x,y,imgThreshold,trackingView);
        }
    }

    imgThreshold.copyTo(imgThresholdDraw);
    putText(imgThresholdDraw,"T: "+intToString(T_Otsu),Point(0,25),1,1,Scalar(255,255,255),2);

    // Output
    //imshow("Eroded Image",imgE);
    //imshow("Eroded Image BGR",imgEBGR);
    //imshow("Eroded+Dilated Image",imgED);
    //imshow("Eroded+Dilated Image BGR",imgEDBGR);
    //imshow("Eroded+Dilated+Median Image",imgEDMedian);
    //imshow("Eroded+Dilated+Median Image BGR",imgEDMedianBGR);
    //imshow("Tracking Object",trackingView);
    //imshow("Thresholded Image",imgThreshold);
}
