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

void StereoMorphology::imageProcessing(Mat src, Mat cameraFeedL,bool isTrackingObjects,bool isVideoFile){
    //FIXME: Mudar colocar a declacao dos elementos abaixo dentro da classe StereoMorphology
    Mat erosionElement  = getStructuringElement(MORPH_RECT,Size(2*EROSION_SIZE +1, 2*EROSION_SIZE+1 ),Point(EROSION_SIZE,  EROSION_SIZE ));
    Mat dilationElement = getStructuringElement(MORPH_RECT,Size(2*DILATION_SIZE+1, 2*DILATION_SIZE+1),Point(DILATION_SIZE, DILATION_SIZE));
    Mat imgE,imgED;
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

void StereoMorphology::Disp_diff(Mat disp8U,Mat disp8U_last,Mat disp8U_diff){
    absdiff(disp8U,disp8U_last,disp8U_diff);
    Mat disp_diff_th;
    threshold(disp8U_diff,disp_diff_th, 10, 255, THRESH_BINARY);

    Mat disp_sum;
    addWeighted(disp8U, 1, disp_diff_th, 1, 0.0, disp_sum);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat disp_countours = Mat::zeros(disp_diff_th.rows, disp_diff_th.cols, CV_8UC3);
    Scalar white = CV_RGB( 255, 255, 255 );
    //                Scalar color( rand()&255, rand()&255, rand()&255 );

    findContours( disp_diff_th, contours, hierarchy,
                  CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

    // iterate through all the top-level contours,
    // draw each connected component with its own random color
    int idx = 0;
    for( ; idx >= 0; idx = hierarchy[idx][0] )
    {
        //drawContours( disp_countours, contours, idx, color, CV_FILLED, 8, hierarchy );
        drawContours( disp_countours, contours, idx, white, CV_FILLED);
    }

    namedWindow( "Components", 1 );


    cv::Mat holes=disp_diff_th.clone();
    cv::floodFill(holes,cv::Point2i(0,0),cv::Scalar(1));
    for(int i=0;i<disp_diff_th.rows*disp_diff_th.cols;i++)                {
        if(holes.data[i]==0)
            disp_diff_th.data[i]=1;
    }

    imshow( "holes", holes );
    imshow( "Components", disp_countours );


    //imshow("Disp",stereo->disp.disp_8U);
    //imshow("Disp_last",stereo->disp.disp_8U_last);
    imshow("Disp_diff",disp8U_diff);
    imshow("Disp_diff_th",disp_diff_th);
    imshow("Disp_sum",disp_sum);
}
