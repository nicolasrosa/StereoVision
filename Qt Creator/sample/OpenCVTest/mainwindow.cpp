#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "cv.h"
#include "highgui.h"
#include "cvaux.h"
#include "stdio.h"
#include "opencv2/opencv.hpp"

using namespace cv;

#define VIDEO_INPUT
//#define IMAGE_INPUT

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);

#ifdef IMAGE_INPUT
    cv::Mat imageInput = imread("/home/nicolas/workspace/eclipse/data/teddy_l.png");

     cv::imshow("Display Image",imageInput);
#endif

#ifdef VIDEO_INPUT
     VideoCapture cap;
     Mat img;

     cap.open(0);

     while(1){
         cap >> img;
         cv::imshow("Video",img);

         waitKey(1);
     }
#endif


}

MainWindow::~MainWindow()
{
    delete ui;
}
