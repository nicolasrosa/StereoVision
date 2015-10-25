#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <StereoProcessor.h>

namespace Ui{
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void printHelp();
    void openStereoSource(int inputNum,cv::VideoCapture* capL,cv::VideoCapture* capR,cv::Mat* imageL,cv::Mat* imageR);
    //StereoProcessor StereoVisionProcessInit();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Mat imageL[2],imageR[2];
    Mat	imageL_grey[2],imageR_grey[2];
    VideoCapture capL,capR;

    QImage qimageL,qimageR;

    QTimer* tmrTimer;

public slots:
    void StereoVisionProcessAndUpdateGUI();

private slots:
    void on_btnPauseOrResume_clicked();

    void on_btnShowDisparityMap_clicked();

    void on_btnShowStereoParamSetup_clicked();

    void on_btnShow3DReconstruction_clicked();
    void on_btnShowInputImages_clicked();
    void on_btnShowTrackingObjectView_clicked();
};

#endif // MAINWINDOW_H
