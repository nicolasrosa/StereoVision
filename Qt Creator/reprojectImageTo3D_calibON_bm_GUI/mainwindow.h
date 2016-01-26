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
#include <QCloseEvent>
#include <QMessageBox>

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
    void setupUi_Custom();
    void StereoVisionProcessInit();
    void printHelp();
    void openStereoSource(int inputNum);

    QImage Mat2QImage(const Mat& mat);
    void putImageL(const Mat& src);
    void putImageR(const Mat& src);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    StereoProcessor *stereo;
    SetStereoParams *stereoParamsSetupWindow;

    QImage qimageL,qimageR;
    QTimer* tmrTimer;

    bool closeEventOccured;

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
    void on_methodSelector_activated(int index);
    void closeEvent(QCloseEvent *event);
    void on_toggleBtnShowHist_clicked(bool checked);
    void mousePressEvent(QMouseEvent *e);
    void on_toggleBtnShowXYZ_toggled(bool checked);
    void on_toggleBtnShowDispDepth_toggled(bool checked);

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H


