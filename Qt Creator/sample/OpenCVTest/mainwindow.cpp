#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <iostream>
#include <opencv/cvaux.h>
#include <stdio.h>

using namespace std;
using namespace cv;

//#define CAMERA_INPUT
//#define VIDEO_INPUT
#define IMAGE_INPUT

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);

#ifdef IMAGE_INPUT
    cv::Mat imageInput = imread("/home/nicolas/workspace/data/teddy_l.png");
    cv::imshow("Display Image",imageInput);
#endif


#if defined(CAMERA_INPUT) || defined(VIDEO_INPUT)

#ifdef CAMERA_INPUT
    VideoCapture capture(0);
#endif

#ifdef VIDEO_INPUT
    VideoCapture capture("/home/nicolas/workspace/data/video10_l.avi");
#endif


    Mat frame;

    if(!capture.isOpened()){
        cerr << "Check Video Input!" << endl;
    }


    while(1){
        capture >> frame;

        if(frame.empty()){
            std::cerr << "Could not open file" << std::endl;
            break;
        }

        if(frame.empty()){
            cout << "oi" << endl;
        }

        cv::imshow("Video",frame);

        if(waitKey(30) >= 0) break;
    }
#endif

}

MainWindow::~MainWindow()
{
    delete ui;
}
