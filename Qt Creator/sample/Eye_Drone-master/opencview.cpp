#include <iostream>
#include "opencview.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QBrush>
#include <QPen>
#include <QColor>
//#include "targetfinder.h"


OpenCview::OpenCview(QWidget *parent) :
    QWidget(parent)
{
    cap.open(0); //objeto criado pelo OpenCV para capturar os frames do video
    //Set the height and width for the captured frame
    cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
    cap >> Raw_Image; // Object created using openCV to capture the frames and store the current frame into the Mat object called 'Raw_Image'
    startTimer(10);
    selectMode = false;
    flag_Process = false;
    enable_ROI = false;
}

void OpenCview::FindTarget(Mat &M){

    Target.MainEngine(M, corner1, corner2);//starts the Engine to process the Image 'M'

    if(enable_ROI){
        enable_ROI = false;
    }
}

void OpenCview::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    QBrush brush;
    QPen pen;
    pen.setColor(Qt::red);
    brush.setColor(qRgba(255,0,0,100));
    brush.setStyle(Qt::Dense7Pattern);
    p.setPen(pen);
    p.setBrush(brush);


    QImage image((uchar*)Display_Image.data, Display_Image.cols, Display_Image.rows,QImage::Format_RGB888);
    p.drawImage(rect(),image,image.rect());
    if(selectMode){
        p.drawRect(QRect(corner1.x,corner1.y, corner2.x - corner1.x, corner2.y - corner1.y));
    }
}

void OpenCview::timerEvent(QTimerEvent *e)
{
    cap >> Raw_Image;
    flip(Raw_Image, Raw_Image, 1);
    if(flag_Process){
        FindTarget(Raw_Image);
        Raw_Image.copyTo(Display_Image);
    }else{
        Raw_Image.copyTo(Display_Image);
    }

    cvtColor(Display_Image,Display_Image,CV_BGR2RGB);
    repaint(); //chama a funcao void OpenCview::paintevent(QPaintEvent *e)
}

void OpenCview::mousePressEvent(QMouseEvent *e)
{
    if(enable_ROI)
    {
        //qDebug() << e->x() << e->y();
        corner1.x = corner2.x = e->x();
        corner1.y = corner2.y = e->y();

        selectMode=true;
    }
}

void OpenCview::mouseReleaseEvent(QMouseEvent *e)
{
    if(enable_ROI)
    {
        //qDebug() << x1 << y1;
        selectMode=false;
        flag_Process = true;
    }
}

void OpenCview::mouseMoveEvent(QMouseEvent *e)
{
    if(enable_ROI)
    {
        corner2.x = e->x();
        corner2.y = e->y();
    }
}

void OpenCview::enable_ROI_selection(){
    enable_ROI = true;
}
