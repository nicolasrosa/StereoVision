#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/* Libraries */
#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/* Custom Libraries */
#include "StereoProcessor.h"

namespace Ui{
    class MainWindow;
}

class MainWindow : public QMainWindow,public StereoProcessor{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void StereoVisionProcessInit();

    void printHelp();
    void openStereoSource(int inputNum,cv::VideoCapture* capL,cv::VideoCapture* capR,cv::Mat* imageL,cv::Mat* imageR);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MainWindow::StereoProcessor *stereo;

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
