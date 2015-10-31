#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/* Libraries */
#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/* Custom Libraries */
#include "StereoProcessor.h"
#include "setstereoparams.h"

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
    SetStereoParams *StereoParamsSetupWindow;

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
