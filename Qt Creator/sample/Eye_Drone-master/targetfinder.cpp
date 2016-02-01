#include "targetfinder.h"
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <string>
#include <cstring>
#include <sstream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>

const int FRAME_HEIGHT = 480;
const int FRAME_WIDTH = 640;

const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;

const int MAX_NUM_OBJECTS=50;


//Update by Miguel Rocha
int ref_x = -1;
int ref_y = -1;
bool draw_reference = false;


using namespace cv;
using namespace std;

int bigger (int a, int b){
    if(a>b)
        return a;
    else
        return b;
}

int smaller (int a, int b){
    if(a>b)
        return b;
    else
        return a;
}

TargetFinder::TargetFinder()
{

}

void TargetFinder::Mean(Mat &ROI_Image){

    Vec3b intensity; // This vector is used to store the intensity values of the three channels
    int sum_H = 0, sum_S=0, sum_V = 0;

    for(int y=corner1.y; y<=corner2.y; y++){
        for(int x=corner1.x; x<=corner2.x; x++){

            intensity = ROI_Image.at<Vec3b>(x,y);

            sum_H = sum_H + intensity.val[0]; // H channel

            sum_S = sum_S + intensity.val[1];  // S channel

            sum_V = sum_V + intensity.val[2];  // V channel
         }
    }


    H_MEAN = (sum_H)/samples;
    S_MEAN = (sum_S)/samples;
    V_MEAN = (sum_V)/samples;

    cout << "H MEAN: " << H_MEAN << endl;
    cout << "S MEAN: " << S_MEAN << endl;
    cout << "V MEAN: " << V_MEAN << endl;
    cout << endl;
}

void TargetFinder::Standarized_Deviation(Mat &ROI_Image){

    Vec3b intensity; // This vector is used to store the intensity values of the three channels
    double sum_H = 0, sum_S=0, sum_V = 0; // Variables used to sum the (X - X_MEAN)^2, X equal the set { H, S, V }

    for(int y=corner1.y; y<=corner2.y; y++){
        for(int x=corner1.x; x<=corner2.x; x++){
            // the first two for's are used to acces the matrix using the form M[x,y]

            intensity = ROI_Image.at<Vec3b>(x,y);

            sum_H += (intensity.val[0] - H_MEAN)*(intensity.val[0] - H_MEAN);   // H channel

            sum_S += (intensity.val[1] - S_MEAN)*(intensity.val[1] - S_MEAN);  // S channel

            sum_V += (intensity.val[2] - V_MEAN)*(intensity.val[2] - V_MEAN);  // V channel
        }
    }

    H_SD = sqrt(sum_H/samples);
    S_SD = sqrt(sum_S/samples);
    V_SD = sqrt(sum_V/samples);

    cout << "H SD: " << H_SD << endl;
    cout << "S SD: " << S_SD << endl;
    cout << "V SD: " << V_SD << endl;

}

string TargetFinder::intToString(int number){


    std::stringstream ss;
    ss << number;
    return ss.str();
}

void TargetFinder::DrawObject(int x, int y, Mat &frame){

    //use some of the openCV drawing functions to draw crosshairs
    //on your tracked image!
    //UPDATE:JUNE 18TH, 2013 By Kyle
    //added 'if' and 'else' statements to prevent
    //memory errors from writing off the screen (ie. (-25,-25) is not within the window

    //UPDATE: OCTOBER 7TH, 2015 By Miguel Rocha Jr -> github.com/miguelrochajr

    circle(frame,Point(x,y),20,Scalar(0,255,0),2);
    if(y-25>0)
        line(frame,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,0),Scalar(0,255,0),2);
    if(y+25<FRAME_HEIGHT)
        line(frame,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,FRAME_HEIGHT),Scalar(0,255,0),2);
    if(x-25>0)
        line(frame,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(0,y),Scalar(0,255,0),2);
    if(x+25<FRAME_WIDTH)
        line(frame,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(FRAME_WIDTH,y),Scalar(0,255,0),2);

    //Draw a red line from the referece point until the x,y coordinate that we are tracking our object
    if(draw_reference == false){
        ref_y = y;
        ref_x = x;
    }
    else{
        line(frame, Point(ref_x, ref_y), Point(x,y), Scalar(0, 0, 255), 4);
    }
    putText(frame,intToString(x)+","+intToString(y),Point(x,y+30),1,1,Scalar(0,255,0),2);
}

void TargetFinder::morphOps(Mat &thresh){

    //create structuring element that will be used to "dilate" and "erode" image.
    //the element chosen here is a 3px by 3px rectangle

    Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
    //dilate with larger element so make sure object is nicely visible
    Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

    erode(thresh,thresh,erodeElement);
    erode(thresh,thresh,erodeElement);


    dilate(thresh,thresh,dilateElement);
    dilate(thresh,thresh,dilateElement);
}

void TargetFinder::trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed){
    Mat temp;
    threshold.copyTo(temp);
    //these two vectors needed for output of findContours
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //find contours of filtered image using openCV findContours function
    findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
    //use moments method to find our filtered object
    double refArea = 0;
    bool objectFound = false;
    if (hierarchy.size() > 0) {
            int numObjects = hierarchy.size();
            //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
            if(numObjects<MAX_NUM_OBJECTS){
                for (int index = 0; index >= 0; index = hierarchy[index][0]) {

                    Moments moment = moments((cv::Mat)contours[index]);
                    double area = moment.m00;

                    //if the area is less than 20 px by 20px then it is probably just noise
                    //if the area is the same as the 3/2 of the image size, probably just a bad filter
                    //we only want the object with the largest area so we safe a reference area each
                    //iteration and compare it to the area in the next iteration.
                    if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
                        x = moment.m10/area;
                        y = moment.m01/area;
                        objectFound = true;
                        refArea = area;
                    }else objectFound = false;
                }
                //let user know you found an object
                if(objectFound ==true){
                    putText(cameraFeed,"Tracking Object",Point(0,50),2,1,Scalar(0,255,0),2);
                    //draw object location on screen
                    DrawObject(x,y,cameraFeed);}

            }else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
     }
}



void TargetFinder::setCorners(Point c1, Point c2){

        corner1.x = smaller(c1.x, c2.x);
        corner1.y = smaller(c1.y, c2.y);
        corner2.x = bigger(c1.x, c2.x);
        corner2.y = bigger(c1.y, c2.y);

//    corner1.x = smaller(c1.x, c2.x);
//    corner1.y = smaller(c1.y, c2.y);
//    corner2.x = bigger(c1.x, c2.x);
//    corner2.y = bigger(c1.y, c2.y);

    int X, Y;
    X = corner2.x - corner1.x;
    Y = corner2.y - corner1.y;

    samples = (X)*(Y);

    cout << "Corner 1: " << corner1.x << " " << corner1.y << endl;
    cout << "Corner 2: " << corner2.x << " " << corner2.y << endl;
    cout << "Samples " << samples << endl;
}

void TargetFinder::setHSV_Range(double &precision){

    H_MIN = H_MEAN - (precision*H_SD);
    H_MAX = H_MEAN + (precision*H_SD);


    S_MIN = S_MEAN - (precision*S_SD);
    S_MAX = S_MEAN + (precision*S_SD);

    V_MIN = V_MEAN - (precision*V_SD);
    V_MAX = V_MEAN + (precision*V_SD);

    //Check if all value are in the range of 0 to 255
    if(H_MAX>255.0)
        H_MAX = 179.0;
    if(H_MIN<0.0)
        H_MIN = 0.0;

    if(S_MAX>255.0)
        S_MAX = 255.0;
    if(S_MIN<0.0)
        S_MIN=0.0;

    if(V_MAX>255.0)
        V_MAX=255.0;
    if(V_MIN<0.0)
        V_MIN=0.0;

    cout << "H range: <" << H_MAX << ", " << H_MIN << ">" << endl;
    cout << "S range: <" << S_MAX << ", " << S_MIN << ">" << endl;
    cout << "V range: <" << V_MAX << ", " << V_MIN << ">" << endl;


}

void TargetFinder::MainEngine(Mat &GivenImage, Point c1, Point c2){

    static bool flag=true;
    double p = 1;

    int x=0, y=0;
    Mat threshold, HSV;

    cvtColor(GivenImage, HSV, COLOR_BGR2HSV);


    if(flag){
        setCorners(c1, c2);
        Mean(HSV);
        Standarized_Deviation(HSV);
        setHSV_Range(p);

        flag = false;
    }


    inRange(HSV,Scalar(H_MIN, S_MIN, V_MIN),Scalar(H_MAX, S_MAX, V_MAX),threshold);
    morphOps(threshold);

    namedWindow("THRESHOLD", WINDOW_AUTOSIZE);
    imshow("THRESHOLD",threshold);

    namedWindow("HSV", WINDOW_AUTOSIZE);
    imshow("HSV", HSV);


    trackFilteredObject(x,y, threshold, GivenImage);


}
