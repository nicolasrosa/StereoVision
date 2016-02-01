#include "opencview.h"
#include <QPainter>
#include <QDebug>

OpenCview::OpenCview(QWidget *parent) :
    QWidget(parent)
{
    cap.open(0);
    cap >> m;
    startTimer(10);
    flag = true;
}

void OpenCview::click(){
  flag = !flag;
  qDebug() << flag;
}

void OpenCview::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    cvtColor(m, disp, CV_GRAY2RGB);
//    cvtColor(m, disp, CV_BGR2RGB);
    QImage image((uchar*)disp.data, disp.cols, disp.rows,QImage::Format_RGB888);
    p.drawImage(rect(),image,image.rect());
}

void OpenCview::capture()
{
//    Mat opencv_image = imread("fruits.jpg", CV_LOAD_IMAGE_COLOR);
 //   Mat dest;
  //  cvtColor(opencv_image, dest,CV_BGR2RGB);
    // QImage image((uchar*)dest.data, dest.cols, dest.rows,QImage::Format_RGB888);
}

void OpenCview::timerEvent(QTimerEvent *e)
{
    float horizontal[]={-1,0,1,
                          -2,0,2,
                          -1,0,1};
    Mat frame, frame32f, frameFiltered;
    Mat mask(3,3,CV_32F), mask1(3,3,CV_32F);
    double min, max;
    mask = Mat(3,3,CV_32F,horizontal);
    cap >> m;
    frame = m;
    cvtColor(m, frame, CV_BGR2GRAY);
    if(flag){
      flip(frame,frame,1);
    }
    frame.convertTo(frame32f, CV_32F);
    filter2D(frame32f,frameFiltered,frame32f.depth(),mask,Point(1,1),0);
    //std::cout << frame32f.at<float>(100,100)<< "\n";
    //filter2D(edges,frame1,edges.depth(),mask1,Point(1,1),128);
     //res=frame+frame1;
     //filter2D(edges,frame,edges.depth(),mask,Point(1,1),128);
     //flip(frame,edges,1);
    frameFiltered=abs(frameFiltered);
  //   minMaxLoc(frameFiltered, &min, &max, 0, 0, Mat());
     //std::cout << max << "\n";
    frameFiltered.convertTo(m,CV_8U);

    repaint();
}
