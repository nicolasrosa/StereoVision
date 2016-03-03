/*
 * mainwindow.h
 *
 *  Created on: Oct 1, 2015
 *      Author: nicolasrosa
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/* Qt Libraries */
#include <QMainWindow>
#include <QtCore>
#include <QShortcut>
#include <QMessageBox>
#include <QCloseEvent>

/* OpenCV Libraries */
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/* Custom Libraries */
#include "StereoProcessor.h"
#include "StereoSetParamsWindow.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

using namespace cv;
using namespace std;

namespace Ui{
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setupUi_Custom();
    void StereoVisionProcessInit();
    void printHelp();
    void openStereoSource(int inputNum);

    QImage Mat2QImage(const Mat& mat);
    void putImageL(const Mat& src);
    void putImageR(const Mat& src);
    ~MainWindow();

private:
    Ui::MainWindow  *ui;
    StereoProcessor *stereo;
    SetStereoParams *stereoParamsSetupWindow;

    QImage qimageL,qimageR;
    QTimer* tmrTimer;

    bool closeEventOccured;
    bool isStereoParamSetupTrackbarsCreated;
    bool isTrackingObjects;

signals:

public slots:
    void StereoVisionProcess_UpdateGUI();

private slots:
    void on_btnPauseOrResume_clicked();
    void on_btnShowDisparityMap_clicked();
    void on_btnShowStereoParamSetup_clicked();
    void on_btnShow3DReconstruction_clicked();
    void on_btnShowInputImages_clicked();
    void on_btnShowTrackingObjectView_clicked();
    void on_btnShowDiffImage_clicked();
    void on_btnShowWarningLines_clicked();
    void on_toggleBtnShowHist_clicked(bool checked);
    void on_toggleBtnShowXYZ_toggled(bool checked);
    void on_toggleBtnShowDispDepth_toggled(bool checked);
    void on_methodSelector_activated(int index);

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *e);
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H

