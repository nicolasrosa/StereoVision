#ifndef TARGETFINDER_H
#define TARGETFINDER_H

#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <string>
#include <cstring>
#include <sstream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;
using namespace std;

/**
 * @brief The TargetFinder class is where all the image processing is being done. Creating an object will allow you
 * to have acces to this set of functions
 */
class TargetFinder
{
private:
    /**
     * @brief H_MEAN, V_MEAN are the Mean values for the HSV (Hue, Saturatoin and Value) space of the ROI
     */
    double H_MEAN, S_MEAN, V_MEAN;
    /**
     * @brief H_SD, S_SD and V_SD are the Standard Deviation values for the HSV space of the ROI;
     */
    double H_SD, S_SD, V_SD;
    /**
     * @brief samples is the number of pixels analized for the Mean and Standard Deviation;
     */
    double samples;
    /**
     * @brief corner1 and corner2 are the corners of the ROI. This code make sure that the corner1 has x and y values less than corner2 ones
     */
    Point corner1, corner2;
    /**
     * @brief H_MIN, H_MAX, S_MIN, S_MAX, V_MIN and V_MAX are the HSV minimum and maxium values to be filtered using the inRange OpenCV function
     */
    double H_MIN, H_MAX, S_MIN, S_MAX, V_MIN, V_MAX;


public:
    /**
     * @brief TargetFinder is the constructor
     */
    TargetFinder();
    /**
     * @brief Standarized_Deviation is the function that will give the standirez deviation HSV values of the ROI
     * @param ROI_Image is the image taken from the camera and converted to HSV color space
     */
    void Standarized_Deviation(Mat &ROI_Image);
    /**
     * @brief Mean is the function that will give the standirez deviation HSV values of the ROI
     * @param ROI_Image is the image taken from the camera and converted to HSV color space
     */
    void Mean(Mat &ROI_Image);
    /**
     * @brief DrawObject is the function that will draw the green circle that will follow the tracked object
     * @param x is the x value fo the tracked object on the image
     * @param y is the y value fo the tracked object on the image
     * @param frame is the image taken from the camera where the circle will be drawn
     */
    void DrawObject (int x, int y, Mat &frame);
    /**
     * @brief morphOps is the function where the morphological operations are going to be used into our image.
     * Such operations are erode and dilate.
     * @param thresh is our threshold binary image. This image will display in white the pixels which the color values
     * are inside of the range given by H_MIN, H_MAX, S_MIN, S_MAX, V_MIN, V_MAX
     */
    void morphOps(Mat &thresh);
    /**
     * @brief trackFilteredObject is the function where the filter and the image processing operatoins are going to be done
     * @param x is the x value fo the tracked object on the image
     * @param y is the y value fo the tracked object on the image
     * @param threshold is our threshold binary image. This image will display in white the pixels which the color values
     * are inside of the range given by H_MIN, H_MAX, S_MIN, S_MAX, V_MIN, V_MAX
     * @param cameraFeed is the image taken by the camera and converted to the HSV colorspace
     */
    void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed);
    /**
     * @brief setCorners is the function that will make sure that the first corner of the ROI has
     * x and y values less than the ones from the second corner.
     * @param c1 is the corner1 parameter
     * @param c2 is the corner2 parameter
     */
    void setCorners(Point c1, Point c2);
    /**
     * @brief setHSV_Range sets the values of the H_MIN, H_MAX, S_MIN, S_MAX, V_MIN, V_MAX
     * @param precision select the pecisoin of the min and maximum HSV values. For example, if precision = 1, the V_MAX
     * will be the Mean*2.
     */
    void setHSV_Range(double &precision);
    /**
     * @brief intToString is a function that converts a integer value into a string. This is used in our trackFilteredObject() function
     * @param number
     * @return
     */
    string intToString(int number);
    /**
     * @brief MainEngine is the Main Engine for the image processing. This function has the recursive iterations to
     * take the data such as Mean and Stadard Deviation and gives it the processed image.
     * @param GivenImage
     * @param c1
     * @param c2
     */
    void MainEngine(Mat &GivenImage, Point c1, Point c2);

};

#endif // TARGETFINDER_H
