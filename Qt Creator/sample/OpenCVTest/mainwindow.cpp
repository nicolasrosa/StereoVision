#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "cv.h"
#include "highgui.h"
#include "cvaux.h"
#include "stdio.h"
#include "opencv2/opencv.hpp"

using namespace cv;

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);

    cv::Mat imageInput = imread("/home/nicolas/workspace/data/images/bear.jpg");
    cv::imshow("Display Image",imageInput);

//    QImage qimageInput((uchar*)imageInput.data,imageInput.cols,imageInput.step,QImage::Format_RGB888);

}

MainWindow::~MainWindow()
{
    delete ui;
}
