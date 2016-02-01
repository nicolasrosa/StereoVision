#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <StereoProcessor.h>
#include <ui_mainwindow.h>
#include <ui_setstereoparams.h>

namespace Ui{
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void printHelp();
    void StereoVisionProcessInit();
    void StereoVisionProcess_UpdateGUI();
    void setupUi_Custom();
    void openStereoSource(int inputNum);
    void putImageL(const Mat& src);
    void putImageR(const Mat& src);

    QTimer *tmrTimer;

    StereoProcessor *stereo;
    bool closeEventOccured;

private:
    Ui::MainWindow *ui;
    Ui::SetStereoParams *stereoParamsSetupWindow;

private slots:
    void on_btnShowInputImages_clicked();
    void on_btnShowDisparityMap_clicked();
    void on_btnShow3DReconstruction_clicked();
    void on_btnShowTrackingObjectView_clicked();
    void on_btnShowDiffImage_clicked();
    void on_btnShowWarningLines_clicked();
    void on_btnPauseOrResume_clicked();
    void on_btnShowStereoParamSetup_clicked();


    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
