/*
 * stereoARM.cpp
 *
 *  Created on: Feb 14, 2016
 *      Author: nicolasrosa
 */

/* Libraries */
#include "../inc/stereoARM/StereoProcessor.h"

#define STEREO_BM
//#define STEREO_SGBM

#define RESOLUTION_320x240
//#define RESOLUTION_640x480
//#define RESOLUTION_1280x960

using namespace cv;
using namespace std;

int main(){
    //Matrix to store each left and right frame of the video_l.avi and video_r.avi
    StereoProcessor stereo;

    stereo.createBM();
    stereo.open();
    stereo.getImageSize();
    stereo.resize();
    stereo.readCalibrationFiles();
    stereo.setParametersBM();

    while(1){
        stereo.captureFrames();
        stereo.applyRectification();

        stereo.calculateDisparities();
        //stereo.applyMorphology();



        //DepthMap GreyScale to RGB
        //    cvtColor(imgDisparity8U,result_bgr,CV_GRAY2BGR);
        //applyColorMap(result,result_bgr, COLORMAP_JET);


        //Mat *cv_image_pseudocolor = imread(imgDisparity8U.size(),IPL_DEPTH_8U, 3);

        /*Mat image = imread("Black&White.jpg");
        imshow("Image",image);
        waitKey(0);
        That should fix your problem, if not, try using the C interface

        IplImage* image = cvLoadImage("Black&White.jpg");
        cvNamedWindow( "Image", CV_WINDOW_AUTOSIZE );
        cvShowImage("Image", image);
        cvWaitKey(0);*/


        cout << "Streaming!" << endl;
        #ifdef SHOW_VIDEO_STREAMING
            //imshow("Left" ,img2);
            //imshow("Right",img1);

            #if defined(RESOLUTION_320x240) || defined(RESOLUTION_640x480)
                //moveWindow("Left" ,0,0);
                //moveWindow("Right",capR.get(CV_CAP_PROP_FRAME_WIDTH)+100,0);
            #endif
        #endif

            //imshow("DepthImage",imgDisparity8U);
            //imshow("Eroded Image",result);
            //imshow("DepthImage RGB",result_bgr);


        #if defined(RESOLUTION_320x240) || defined(RESOLUTION_640x480)
            //moveWindow("DepthImage",0,capR.get(CV_CAP_PROP_FRAME_HEIGHT)+100);
            //moveWindow("Eroded Image",capR.get(CV_CAP_PROP_FRAME_WIDTH)+100,capR.get(CV_CAP_PROP_FRAME_HEIGHT)+100);
            //moveWindow("DepthImage RGB",capR.get(CV_CAP_PROP_FRAME_WIDTH)*2+150,capR.get(CV_CAP_PROP_FRAME_HEIGHT)+100);
            //moveWindow(trackbarWindowName,capR.get(CV_CAP_PROP_FRAME_WIDTH)*3+250,0);
        #endif

        cout << "DONE..." << endl;

        // Save the result
        imwrite("disp_8U.jpg", stereo.disp_8U);

        if(waitKey(30) >= 0) break;
    }
    // The camera will be closed automatically in VideoCapture Destructor
    cout << "END" << endl;
    return 0;
}
