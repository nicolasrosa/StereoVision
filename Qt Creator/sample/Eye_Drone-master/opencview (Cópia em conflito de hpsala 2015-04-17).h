#ifndef OPENCVIEW_H
#define OPENCVIEW_H
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <QWidget>

using namespace cv;

class OpenCview : public QWidget
{
    Q_OBJECT
    Mat m, disp;
    bool flag;
public:
    explicit OpenCview(QWidget *parent = 0);
    VideoCapture cap;
signals:
    
public slots:
    void paintEvent(QPaintEvent *e);
    void capture();
    void click();
    void timerEvent(QTimerEvent *e);
};

#endif // OPENCVIEW_H
