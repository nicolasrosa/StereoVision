/*
 * mainwindow.h
 *
 *  Created on: Oct 1, 2015
 *      Author: nicolasrosa
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/* Libraries */
#include <QMainWindow>
#include <opencv2/opencv.hpp>

/* Custom Libraries */
#include "StereoProcessor.h"
#include "setstereoparams.h"

using namespace cv;

namespace Ui{
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void StereoVisionProcessInit();
    void printHelp();
    void openStereoSource(int inputNum);
    void createTrackbars();
    QImage putImage(const Mat& mat);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    StereoProcessor *stereo;
    SetStereoParams *stereoParamsSetupWindow;

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
    void on_btnShowDiffImage_clicked();
    void on_btnShowDiffImage_2_clicked();
    void on_comboBox_activated(int index);
};

#endif // MAINWINDOW_H


