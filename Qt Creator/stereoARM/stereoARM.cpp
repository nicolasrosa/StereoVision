    // BBB Test OpenCV
// Nicolas Rosa, April 2015.

//#include <iostream> //for cout and cin
//#include <stdio.h> //for printf
// Include OpenCV
//#define CV_NO_BACKWARD_COMPATIBILITY
//#include <opencv.hpp>
//#include <highgui/highgui.hpp>
//#include <unistd.h>

#include "stdio.h"
#include "opencv2/opencv.hpp"

//#include "core.hpp"
//#include "highgui/highgui.hpp"

#define RESOLUTION_320x240
//#define RESOLUTION_640x480
//#define RESOLUTION_1280x960
#define SHOW_VIDEO_STREAMING
#define EROSION_SIZE 3  //SAR
#define DILATE_SIZE 5  //SAR

using namespace cv;
using namespace std;

//Initial value and MAX StereoBM Parameters.
//These will be changed using trackbars
int preFilterSize			 = 0;	const int preFilterSize_MAX		 	= 100;
int preFilterCap			 = 19;	const int preFilterCap_MAX		 	= 100;
int SADWindowSize			 = 6;	const int SADWindowSize_MAX		 	= 100;
int minDisparity			 = 50;	const int minDisparity_MAX		 	= 100;
int numberOfDisparities		 = 4;	const int numberOfDisparities_MAX 	= 16;
int textureThreshold		 = 0;	const int textureThreshold_MAX		= 100;
int uniquenessRatio			 = 12;	const int uniquenessRatio_MAX		= 100;
int speckleWindowSize		 = 0;	const int speckleWindowSize_MAX	 	= 100;
int speckleRange			 = 18;	const int speckleRange_MAX		 	= 100;
int disp12MaxDiff			 = 1;	const int disp12MaxDiff_MAX		 	= 1;

const string trackbarWindowName = "Trackbars";

//Scope
void on_trackbar( int, void* ){}//This function gets called whenever a trackbar position is changed
void createTrackbars();

int main(int, char**){
    //Matrix to store each left and right frame of the video_l.avi and video_r.avi
    Mat img1, img2,leftImage,rightImage,Q,result,result_bgr;
    Rect roi1, roi2;
    //int temp;

    int ndisparities = 64;
    int SADWindowSize = 31;
    Ptr<StereoBM> bm = StereoBM::create(ndisparities,SADWindowSize);

    //StereoBM bm;

    //create slider bars for HSV filtering
    //createTrackbars();

    //VideoCapture cap_r(0);
    //VideoCapture cap_l(1);

    VideoCapture cap_r("../data/20004.avi");
    VideoCapture cap_l("../data/30004.avi");

    if(!cap_r.isOpened()){ 					// Check if we succeeded
        cout << "Open Video Left Failed!" << endl;
        return -1;
    }

    if(!cap_l.isOpened()){ 					// Check if we succeeded
        cout << "Open Video Right Failed!" << endl;
        return -1;
    }

    #ifdef RESOLUTION_320x240
        cap_r.set(CV_CAP_PROP_FRAME_WIDTH, 320);
        cap_r.set(CV_CAP_PROP_FRAME_HEIGHT,240);
        cap_l.set(CV_CAP_PROP_FRAME_WIDTH, 320);
        cap_l.set(CV_CAP_PROP_FRAME_HEIGHT,240);
    #endif

    #ifdef RESOLUTION_640x480
        cap_r.set(CV_CAP_PROP_FRAME_WIDTH, 640);
        cap_r.set(CV_CAP_PROP_FRAME_HEIGHT,480);
        cap_l.set(CV_CAP_PROP_FRAME_WIDTH, 640);
        cap_l.set(CV_CAP_PROP_FRAME_HEIGHT,480);
    #endif

    #ifdef RESOLUTION_1280x960
        cap_r.set(CV_CAP_PROP_FRAME_WIDTH,1280);
        cap_r.set(CV_CAP_PROP_FRAME_HEIGHT,960);
        cap_l.set(CV_CAP_PROP_FRAME_WIDTH,1280);
        cap_l.set(CV_CAP_PROP_FRAME_HEIGHT,960);
    #endif

    #ifdef SHOW_VIDEO_STREAMING
        //namedWindow("Left",1);
        //namedWindow("Right",1);
    #endif
    //namedWindow("DepthImage", 1);

    cout << "Camera 1 Resolution: " << cap_r.get(CV_CAP_PROP_FRAME_WIDTH) << "x" << cap_r.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
    cout << "Camera 2 Resolution: " << cap_l.get(CV_CAP_PROP_FRAME_WIDTH) << "x" << cap_l.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;

    while(1){
        cout << "Taking New Frame..." << endl;
        cap_r >> img1;
        cap_l >> img2;

        cout << "Calculating Disparity..." << endl;
        Mat imgDisparity16S = Mat( img2.rows, img2.cols, CV_16S );
        Mat imgDisparity8U = Mat( img1.rows, img1.cols, CV_8UC1 );

        cout << "Color Space Conversion..." << endl;
        cvtColor(img2, leftImage,CV_BGR2GRAY);
        cvtColor(img1,rightImage,CV_BGR2GRAY);

        //Setting StereoBM Parameters
        /*temp= getTrackbarPos("preFilterSize",trackbarWindowName)*2.5+5;
        if(temp%2==1 && temp>=5 && temp<=255){
            bm.state->preFilterSize = temp;
            cout << getTrackbarPos("preFilterSize",trackbarWindowName) << "\t" <<temp << endl;
        }

        temp= getTrackbarPos("preFilterCap",trackbarWindowName)*0.625+1;
        if(temp>=1 && temp<=63){
            bm.state->preFilterCap = temp;
            cout << getTrackbarPos("preFilterCap",trackbarWindowName) << "\t" <<temp << endl;
        }

        temp= getTrackbarPos("SADWindowSize",trackbarWindowName)*2.5+5;
        if(temp%2==1 && temp>=5 && temp<=255 && temp<=cap_r.get(CV_CAP_PROP_FRAME_HEIGHT)){
            bm.state->SADWindowSize = temp;
            cout << getTrackbarPos("SADWindowSize",trackbarWindowName) << "\t" <<temp << endl;
        }

        temp= getTrackbarPos("minDisparity",trackbarWindowName)*2.0-100;
        if(temp>=-100 && temp<=100){
            bm.state->minDisparity = temp;
            cout << getTrackbarPos("minDisparity",trackbarWindowName) << "\t" <<temp << endl;
        }

        temp= getTrackbarPos("numberOfDisparities",trackbarWindowName)*16;
        if(temp%16==0 && temp>=16 && temp<=256){
            bm.state->numberOfDisparities = temp;
            cout << getTrackbarPos("numberOfDisparities",trackbarWindowName) << "\t" <<temp << endl;
        }

        temp= getTrackbarPos("textureThreshold",trackbarWindowName)*320;
        if(temp>=0 && temp<=32000){
            bm.state->textureThreshold = temp;
            cout << getTrackbarPos("textureThreshold",trackbarWindowName) << "\t" <<temp << endl;
        }

        temp= getTrackbarPos("uniquenessRatio",trackbarWindowName)*2.555;
        if(temp>=0 && temp<=255){
            bm.state->uniquenessRatio = temp;
            cout << getTrackbarPos("uniquenessRatio",trackbarWindowName) << "\t" <<temp << endl;
        }

        temp= getTrackbarPos("speckleWindowSize",trackbarWindowName)*1.0;
        if(temp>=0 && temp<=100){
            bm.state->speckleWindowSize = temp;
            cout << getTrackbarPos("speckleWindowSize",trackbarWindowName) << "\t" <<temp << endl;
        }

        temp= getTrackbarPos("speckleRange",trackbarWindowName)*1.0;
        if(temp>=0 && temp<=100){
            bm.state->speckleRange = temp;
            cout << getTrackbarPos("speckleRange",trackbarWindowName) << "\t" <<temp << endl;
        }

        temp= getTrackbarPos("disp12MaxDiff",trackbarWindowName)*1.0;
        if(temp>=0 && temp<=100){
            bm.state->disp12MaxDiff = temp;
            cout << getTrackbarPos("disp12MaxDiff",trackbarWindowName) << "\t" <<temp << endl;
        }*/

        cout << "Setting StereoBM Parameters..." << endl;
//        bm.state->preFilterSize = 5;
//        bm.state->preFilterCap = 18;
//        bm.state->SADWindowSize = 31;
//        bm.state->minDisparity = 0;
//        bm.state->numberOfDisparities = 64;
//        bm.state->textureThreshold = 0;
//        bm.state->uniquenessRatio = 0;
//        bm.state->speckleWindowSize = 0;
//        bm.state->speckleRange = 0;
//        bm.state->disp12MaxDiff = 1;

        bm->setPreFilterSize(5);
        bm->setPreFilterCap(18);
        bm->setBlockSize(31);
        bm->setMinDisparity(0);
        bm->setNumDisparities(64);
        bm->setTextureThreshold(0);
        bm->setUniquenessRatio(0);
        bm->setSpeckleWindowSize(0);
        bm->setSpeckleRange(0);
        bm->setDisp12MaxDiff(1);

        cout << "Computing Stereo Matching..." << endl;
        //bm(leftImage,rightImage,imgDisparity16S,CV_16S);
        bm->compute(leftImage,rightImage,imgDisparity16S);

        double minVal; double maxVal;

        minMaxLoc( imgDisparity16S, &minVal, &maxVal );
        imgDisparity16S.convertTo( imgDisparity8U, CV_8UC1, 255/(maxVal - minVal));

        //Erode the images
        /*Mat element(1,1,CV_8U,Scalar(1));
        erode(imgDisparity8U,result,element,Point(-1,-1),3);*/

        //SAR Erode and Dilate to take out spurious noise
        Mat element_erode = getStructuringElement( MORPH_RECT,Size( 2*EROSION_SIZE + 1, 2*EROSION_SIZE+1 ),
                                                       Point( EROSION_SIZE, EROSION_SIZE ));
        Mat element_dilate = getStructuringElement( MORPH_RECT,Size( 2*DILATE_SIZE + 1, 2*DILATE_SIZE+1 ),
                                                       Point( DILATE_SIZE, DILATE_SIZE ));
        cout << "Eroding and Dilating Result..." << endl;
        erode(imgDisparity8U,result,element_erode);
        dilate(result,result,element_dilate);

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
                //moveWindow("Right",cap_r.get(CV_CAP_PROP_FRAME_WIDTH)+100,0);
            #endif
        #endif

            //imshow("DepthImage",imgDisparity8U);
            //imshow("Eroded Image",result);
            //imshow("DepthImage RGB",result_bgr);


        #if defined(RESOLUTION_320x240) || defined(RESOLUTION_640x480)
            //moveWindow("DepthImage",0,cap_r.get(CV_CAP_PROP_FRAME_HEIGHT)+100);
            //moveWindow("Eroded Image",cap_r.get(CV_CAP_PROP_FRAME_WIDTH)+100,cap_r.get(CV_CAP_PROP_FRAME_HEIGHT)+100);
            //moveWindow("DepthImage RGB",cap_r.get(CV_CAP_PROP_FRAME_WIDTH)*2+150,cap_r.get(CV_CAP_PROP_FRAME_HEIGHT)+100);
            //moveWindow(trackbarWindowName,cap_r.get(CV_CAP_PROP_FRAME_WIDTH)*3+250,0);
        #endif

        cout << "DONE..." << endl;


        if(waitKey(30) >= 0) break;
    }
    // The camera will be closed automatically in VideoCapture Destructor
    cout << "END" << endl;
    return 0;
}

void createTrackbars(){ //Create window for trackbars
    char TrackbarName[50];

    // Create TrackBars Window
    namedWindow("Trackbars",0);

    // Create memory to store Trackbar name on window
    sprintf( TrackbarName, "preFilterSize");
    sprintf( TrackbarName, "preFilterCap");
    sprintf( TrackbarName, "SADWindowSize");
    sprintf( TrackbarName, "minDisparity");
    sprintf( TrackbarName, "numberOfDisparities");
    sprintf( TrackbarName, "textureThreshold");
    sprintf( TrackbarName, "uniquenessRatio");
    sprintf( TrackbarName, "speckleWindowSize");
    sprintf( TrackbarName, "speckleRange");
    sprintf( TrackbarName, "disp12MaxDiff");

    //Create Trackbars and insert them into window
    createTrackbar( "preFilterSize", trackbarWindowName, &preFilterSize, preFilterSize_MAX, on_trackbar );
    createTrackbar( "preFilterCap", trackbarWindowName, &preFilterCap, preFilterCap_MAX, on_trackbar );
    createTrackbar( "SADWindowSize", trackbarWindowName, &SADWindowSize, SADWindowSize_MAX, on_trackbar );
    createTrackbar( "minDisparity", trackbarWindowName, &minDisparity, minDisparity_MAX, on_trackbar );
    createTrackbar( "numberOfDisparities", trackbarWindowName, &numberOfDisparities, numberOfDisparities_MAX, on_trackbar );
    createTrackbar( "textureThreshold", trackbarWindowName, &textureThreshold, textureThreshold_MAX, on_trackbar );
    createTrackbar( "uniquenessRatio", trackbarWindowName, &uniquenessRatio, uniquenessRatio_MAX, on_trackbar );
    createTrackbar( "speckleWindowSize", trackbarWindowName, &speckleWindowSize, speckleWindowSize_MAX, on_trackbar );
    createTrackbar( "speckleRange", trackbarWindowName, &speckleRange, speckleRange_MAX, on_trackbar );
    createTrackbar( "disp12MaxDiff", trackbarWindowName, &disp12MaxDiff, disp12MaxDiff_MAX, on_trackbar );
}
