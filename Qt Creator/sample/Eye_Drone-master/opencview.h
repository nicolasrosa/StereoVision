#ifndef OPENCVIEW_H
#define OPENCVIEW_H
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include "targetfinder.h"
#include <QWidget>

using namespace cv;


/**
 * @brief The OpenCview class is going to be used to display the OpenCV data into our QWidget called MainWindow.
 * It receives from the Main Engine (written using OpenCV) and display it as a QImage into the MainWindow QWidget.
 */

class OpenCview : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief OpenCview is the constructor for the class
     * @param parent is the base class
     */
    explicit OpenCview(QWidget *parent = 0);
    /**
     * @brief FindTarget is a function that will initialize the Object from our MainEngine, the one that will proecss the image.
     * @param M is a matrix declared as Mat type, and OpenCV type
     */
    void FindTarget(Mat &M);
signals:
    
public slots:
    /**
     * @brief paintEvent will "paint" the Qt events into the MainWindow QWidget
     * @param e
     */
    void paintEvent(QPaintEvent *e);
    /**
     * @brief timerEvent is a ininterruptive loop that will be runned after a pre assigned delay
     * @param e
     */
    void timerEvent(QTimerEvent *e);
    /**
     * @brief mousePressEvent is a function called when the left button of the mouse is clicked
     * @param e
     */
    void mousePressEvent(QMouseEvent *e);
    /**
     * @brief mouseReleaseEvent is the function called when the left button fo the mouse is released
     * @param e
     */
    void mouseReleaseEvent(QMouseEvent *e);
    /**
     * @brief mouseMoveEvent is the function called when the mouse moves
     * @param e
     */
    void mouseMoveEvent(QMouseEvent *e);
    /**
     * @brief enable_ROI_selection this function will set the flag "enable_ROI", which will let
     * the user select the ROI (Region of Interest) on the screen using the mouse
     */
    void enable_ROI_selection();

private:
    /**
     * @brief Raw_Image  is the Raw_Image captured from the camera
     * @brief Display_Image is the resulting post-process image that will be displayer on the QWidget MainWindow
     */

    Mat Raw_Image, Display_Image; //Display Image is the image that is going to be displayed
    /**
     * @brief cap is an object from the OpenCV type VideoCapture. This will be used to store the frames from the camera
     */
    VideoCapture cap;
    /**
     * @brief Target is the class from the main engine called TargetFinder. This class [target finder] is where the image processing happens
     */
    TargetFinder Target;
    /**
     * @brief corner1 and corner 2 are the corners of the Region of Interest seleceted by the user
     */
    Point corner1, corner2; //coordinates for two opposite corners of an rectangle of interest. Here we have two points: (x0, y0) and (x1, y1)
    /**
     * @brief selectMode
     * @brief flag_Process is the flag that will make the code use the main engine or just pass it and display the Raw_Image
     * @brief enable_ROI is the flag that will or not enable the ROI to be selecetd. It will be toggled when the button is clicked
     */
    bool selectMode, flag_Process, enable_ROI;
};

#endif // OPENCVIEW_H
