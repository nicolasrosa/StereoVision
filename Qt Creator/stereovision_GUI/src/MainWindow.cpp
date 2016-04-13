/* Project: reprojectImageTo3D - BlockMatching Algorithm
 * mainwindow.cpp
 *
 *  Created on: June, 2015
 *      Author: nicolasrosa
 *  Credits to:
 * Anonymous Author: Credits: http://opencv.jp/opencv2-x-samples/point-cloud-rendering
 * Kyle Hounslow - https://www.youtube.com/watch?v=bSeFrPrqZ2A
 * helicopters*.png - Icons made by Freepik from www.flaticon.com - Link: http://www.flaticon.com/free-icon/drone_90894#term=drone&page=1&position=11
 * spydrone*.png - Icon made by Aha-Soft from Noun Project
 */

/* Libraries */
#include "inc/MainWindow.h"
#include "ui_mainwindow.h"

/* Global Variables */

/* Constructor and Destructor */
MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow){
    uiConfiguration();
    timerConfiguration();

    stereoVisionProcessInit();

    printHelp();

    closeEventOccured = false;
    isTrackingObjects=true;
}


MainWindow::~MainWindow(){
    delete stereo;
    delete ui;
    delete stereoParamsSetupWindow;

    tmrTimer->stop();
    closeEventOccured = true;
}


/* Instance Methods */
void MainWindow::uiConfiguration(){
    ui->setupUi(this);
    setupUi_Custom();
}


void MainWindow::timerConfiguration(){
    tmrTimer = new QTimer(this);
    connect(tmrTimer,SIGNAL(timeout()),this,SLOT(stereoVisionProcess_UpdateGUI()));
    tmrTimer->start(20);
}


void MainWindow::setupUi_Custom(){
    setWindowIcon(QIcon(":/icons/icon/spydrone_black.png"));

    /* Allows the following pushButtons to toggle - A non-checkable button never emits the toggled(bool) signal. */
    ui->toggleBtnShowHist->setCheckable(true);
    ui->toggleBtnShowXYZ->setCheckable(true);
    ui->toggleBtnShowDispDepth->setCheckable(true);
    ui->toggleBtnShowLeftImage->setCheckable(true);

    ui->toggleBtnShowHist->hide();
    ui->toggleBtnShowXYZ->hide();
    ui->toggleBtnShowDispDepth->hide();
    ui->toggleBtnShowLeftImage->hide();

    ui->statusBar->showMessage("Running...");
}


void MainWindow::stereoVisionProcessInit(){
    stereo = new StereoProcessor(0);

    /* (1) Open Image Source */
    openStereoSource(stereo->getInputNum());
    stereo->readConfigFile();
    stereo->readStereoBMConfigFile();
    stereo->readStereoSGBMConfigFile();
    stereo->readStereoBM_GPUConfigFile();

    /* (2) Camera Setting */
    /* Getting Input Resolution */
    switch(stereo->calib.inputType){
    case StereoCalib::VideoFile:
        stereo->calib.setResolution(stereo->capL.get(CV_CAP_PROP_FRAME_WIDTH),stereo->capL.get(CV_CAP_PROP_FRAME_HEIGHT));
        break;
    case StereoCalib::ImageFile:
        stereo->calib.setResolution(stereo->imageL[0].cols,stereo->imageL[0].rows);
        break;
    }

    /* Setting Desired Resolution */
    stereo->calib.setResolutionDesired(640,480);

    /* Checking Invalid Input Size */
    if(stereo->calib.getResolution_width()==0 && stereo->calib.getResolution_height()==0){
        cerr << "Number of Cols and Number of Rows equal to ZERO!" << endl;
    }else{
        /* Console Output */
        cout << "Input Resolution(Width,Height): (" << stereo->calib.getResolution_width() << "," << stereo->calib.getResolution_height() << ")" << endl;
        cout << "Desired Resolution(Width,Height): (" << stereo->calib.getResolutionDesired_width() << "," << stereo->calib.getResolutionDesired_height() << ")" << endl << endl;

        /* GUI */
        ui->textBoxOutput->appendPlainText(QString("Input Resolution(Width,Height): (")+
                                           QString::number(stereo->calib.getResolution_width())+
                                           QString(",")+QString::number(stereo->calib.getResolution_height())+
                                           QString(")"));
        ui->textBoxOutput->appendPlainText(QString("Desired Resolution(Width,Height): (")+
                                           QString::number(stereo->calib.getResolutionDesired_width())+
                                           QString(",")+
                                           QString::number(stereo->calib.getResolutionDesired_height())+
                                           QString(")"));
    }

    /* Resizing the Input Resolution to the Desired Resolution*/
    if(stereo->calib.inputType == StereoCalib::ImageFile && (stereo->calib.getResolution_width() != stereo->calib.getResolutionDesired_width())){
        stereo->utils.resizeFrames(&stereo->imageL[0],&stereo->imageR[0],stereo->calib.getResolutionDesired());
    }

    /* (3) Stereo Initialization */
    /* Initializing Stereo Matching Methods */
    stereo->stereoBM_Init();
    stereo->stereoSGBM_Init();
    stereo->stereoBM_GPU_Init();

    /* Setting Stereo Parameters */
    stereo->setNumRows(stereo->calib.getResolutionDesired_height());
    stereo->setNumChannels(stereo->imageL[0].channels());

    stereo->setStereoBM_Params();
    stereo->setStereoSGBM_Params();
    stereo->setStereoBM_GPU_Params();

    /* (4) Stereo Calibration */
    if(stereo->calib.needCalibration){
        /* Read Calibration Files */
        stereo->calib.readCalibrationFiles();

        /* Read/Calculate the Q Matrix */
        if(stereo->calib.hasQMatrix){
            stereo->calib.readQMatrix(); //true=640x480 false=others
        }else{
            stereo->calib.imageCenter = Point2d((stereo->imageL[0].cols-1.0)/2.0,(stereo->imageL[0].rows-1.0)/2.0);
            stereo->calib.calculateQMatrix();
        }

        /* Calculate the K Matrix */
        ////        // Checking Intrinsic Matrix
        ////        if(stereo->calib.isKcreated){
        ////           cout << "The Intrinsic Matrix is already Created." << endl << endl;
        ////        }else{
        //            //createKMatrix();
        // //       }
        stereo->calib.createKMatrix();
    }else{
        cout << "Calibration: OFF" << endl << endl;
        cerr << "Warning: Can't generate 3D Reconstruction. Please, check Q,K Matrix." << endl;

        //stereo->readQMatrix(); //true=640x480 false=others
        //stereo->createKMatrix();
    }

    /* (5) Point Cloud Initialization */
    stereo->view3D.PointCloudInit(stereo->calib.baseline,true);
}


void MainWindow::stereoVisionProcess_UpdateGUI(){
    /* (6) Rendering Loop */
    stereo->utils.startClock();

    if(stereo->calib.inputType == StereoCalib::VideoFile){
        /* (7) Frames Capture */
        stereo->captureFrames();

        /* (8) Camera Retification */
        if(stereo->calib.needCalibration){
            stereo->applyRectification();
        }
    }

    /* (9) Disparities Calculation */
    if(stereo->flags.showDisparityMap || stereo->flags.show3Dreconstruction || stereo->flags.showTrackingObjectView || stereo->flags.showDiffImage || stereo->flags.showWarningLines){
        stereo->calculateDisparities();
    }

    /* (10) Projecting 3D point cloud to image */
    if(stereo->flags.show3Dreconstruction && stereo->calib.needCalibration){
        stereo->calculate3DReconstruction();
    }

    /* (11) Image Processing */
    if(stereo->flags.showTrackingObjectView || stereo->flags.showDiffImage || stereo->flags.showWarningLines){
        switch(stereo->calib.inputType){
        case StereoCalib::VideoFile:
            stereo->morph.applyMorphology(stereo->disp.disp_8U,stereo->imageL[0],true,stereo->calib.inputType,false);
            break;
        case StereoCalib::ImageFile:
            ui->textBoxOutput->appendPlainText(QString("This feature is not supported, because of the Input file type (Image File).1"));
            break;
        }
    }

    if(stereo->flags.showHistograms){
        stereo->utils.calculateHist(stereo->disp.disp_8U,"Disparity Map Histogram");
        stereo->utils.calculateHist(stereo->imageL[0],"Left Image Histogram");
    }

    /* (12) Movement Difference between Frames */
    if(stereo->flags.showDiffImage || stereo->flags.showWarningLines){
        switch(stereo->calib.inputType){
        case StereoCalib::VideoFile:
            if(stereo->diff.StartDiff){
                stereo->diff.createDiffImage(stereo->imageL_grey[0],stereo->imageL_grey[1]);

                //TODO: Terminar a funcao Disp_diff
                //stereo->morph.Disp_diff(stereo->disp.disp_8U,stereo->disp.disp_8U_last,stereo->disp.disp_8U_diff);

                if(stereo->diff.diffImage.data){
                    stereo->diff.createResAND(stereo->diff.diffImage,stereo->morph.imgThreshold);
                    stereo->diff.convertToBGR();
                    stereo->imageL[0].copyTo(stereo->diff.imageL);
                    stereo->diff.addRedLines();
                }

                stereo->saveLastFrames();
            }else{
                stereo->saveLastFrames();
                stereo->diff.StartDiff=true;
            }
            break;
        case StereoCalib::ImageFile:
            ui->textBoxOutput->appendPlainText(QString("This feature is not supported, because of the Input file type (Image File).2"));
            break;
        }
    }

    /* (13) GUI Output */
    MainWindow::updateDisplayWindows();

    /* (14) Video Loop - If the last frame is reached, reset the capture and the frameCounter */
    stereo->videoLooper();

    /* (15) Performance Measurement - FPS */
    stereo->utils.stopClock();
    //stereo->utils.showFPS();
    ui->lcdNumber->display((int)stereo->utils.fps);

    if(closeEventOccured){
        cout << "----------------------------- END ------------------------------------" << endl;
        return;
    }

    /* (16) Loop End - It will display the window infinitely until any keypress (it is suitable for image display) */
    waitKey(1);
}

void MainWindow::deleteStereoObj(){
    delete stereo;
}

void MainWindow::printHelp(){
    /* Console Output */
    cout << "-----------------Help Menu-----------------\n"
         << "Run command line: ./reprojectImageTo3D\n"
         << "Keys:\n"
         << "'`' -\tShow Help\n"
         << "'1' -\tShow L/R Windows\t\t'4' -\tShow XYZ\n"
         << "'2' -\tShow Disparity Map\t\t'5' -\tShow FPS\n"
         << "'3' -\tShow 3D Reconstruction\t'6' -\tShow Stereo Parameters\n"
         << "\n3D Viewer Navigation:\n"
         << "x-axis:\t'Key_Right'/'Key_Left' -> +x,-x\n"
         << "y-axis:\t'Key_Up'/'Key_Down' -> +y,-y\n"
         << "z-axis:\t'Key_Plus'/'Key_Minus' -> +z,-z\n"
         << "-------------------------------------------\n"
         << "\n\n";

    /* GUI */
    ui->textBoxOutput->appendPlainText
            (QString("-----------------Help Menu-----------------\n")+
             QString("Run command line: ./reprojectImageTo3D\n")+
             QString("Keys:\n")+
             QString("'`' -\tShow Help\n")+
             QString("'1' -\tShow L/R Windows\t\t'4' -\tShow XYZ\n")+
             QString("'2' -\tShow Disparity Map\t\t'5' -\tShow FPS\n")+
             QString("'3' -\tShow 3D Reconstruction\t'6' -\tShow Stereo Parameters\n")+
             QString("\n3D Viewer Navigation:\n")+
             QString("x-axis:\t'Key_Right'/'Key_Left' -> +x,-x\n")+
             QString("y-axis:\t'Key_Up'/'Key_Down' -> +y,-y\n")+
             QString("z-axis:\t'Key_Plus'/'Key_Minus' -> +z,-z\n")+
             QString("-------------------------------------------\n")+
             QString("\n\n"));
}


void MainWindow::openStereoSource(int inputNum){
    /* Create an object that decodes the input Video stream. */
    cout << "Enter Video Number(1,2,3,4,5,6,7,8,9): " << endl;
    cout << "Input File: " << inputNum << endl;

    //	scanf("%d",&inputNum);
    ui->textBoxOutput->appendPlainText(QString("Enter Video Number(1,2,3,4,5,6,7,8,9): "));
    ui->textBoxOutput->appendPlainText(QString("Input File: ")+QString::number(inputNum));

    switch(inputNum){
    case 0:
        stereo->imageL_filename = "/home/nicolas/workspace/data/video10_l.avi";
        stereo->imageR_filename = "/home/nicolas/workspace/data/video10_r.avi";
        stereo->calib.intrinsicsFileName = "/home/nicolas/repository/StereoVision/Qt Creator/stereovision_GUI/config/calib/cam1/intrinsics.yml";
        stereo->calib.extrinsicsFileName = "/home/nicolas/repository/StereoVision/Qt Creator/stereovision_GUI/config/calib/cam1/extrinsics.yml";
        stereo->calib.needCalibration=true;
        stereo->calib.hasQMatrix=true;
        break;
    case 1:
        stereo->imageL_filename = "/home/nicolas/workspace/data/video12_l.avi";
        stereo->imageR_filename = "/home/nicolas/workspace/data/video12_r.avi";
        stereo->calib.intrinsicsFileName = "/home/nicolas/repository/StereoVision/Qt Creator/stereovision_GUI/config/calib/cam1/intrinsics.yml";
        stereo->calib.extrinsicsFileName = "/home/nicolas/repository/StereoVision/Qt Creator/stereovision_GUI/config/calib/cam1/extrinsics.yml";
        stereo->calib.needCalibration=true;
        stereo->calib.hasQMatrix=true;
        break;
    case 2:
        stereo->imageL_filename = "/home/nicolas/workspace/data/dataset/Piano-perfect/im0.png";
        stereo->imageR_filename = "/home/nicolas/workspace/data/dataset/Piano-perfect/im1.png";
        stereo->calib.needCalibration=false;
        stereo->calib.hasQMatrix=false;
        break;
    case 3:
        stereo->imageL_filename = "/home/nicolas/workspace/data/20004.avi";
        stereo->imageR_filename = "/home/nicolas/workspace/data/30004.avi";
        stereo->calib.intrinsicsFileName = "/home/nicolas/repository/StereoVision/Qt Creator/stereovision_GUI/config/calib/cam2/intrinsics.yml";
        stereo->calib.extrinsicsFileName = "/home/nicolas/repository/StereoVision/Qt Creator/stereovision_GUI/config/calib/cam2/extrinsics.yml";
        stereo->calib.needCalibration=true;
        stereo->calib.hasQMatrix=false;
        break;
    case 4:
        stereo->imageL_filename = "/home/nicolas/workspace/data/teddy_l.png";
        stereo->imageR_filename = "/home/nicolas/workspace/data/teddy_r.png";
        stereo->calib.needCalibration=false;
        stereo->calib.hasQMatrix=false;
        break;
    case 5:
        stereo->imageL_filename = "/home/nicolas/workspace/data/left/video15.avi";
        stereo->imageR_filename = "/home/nicolas/workspace/data/right/video15.avi";
        stereo->calib.intrinsicsFileName = "/home/nicolas/repository/StereoVision/Qt Creator/stereovision_GUI/config/calib/cam1/intrinsics.yml";
        stereo->calib.extrinsicsFileName = "/home/nicolas/repository/StereoVision/Qt Creator/stereovision_GUI/config/calib/cam1/extrinsics.yml";
        stereo->calib.needCalibration=true;
        stereo->calib.hasQMatrix=true;
        break;
    case 6:
        stereo->imageL_filename = "/home/nicolas/workspace/data/20011.avi";
        stereo->imageR_filename = "/home/nicolas/workspace/data/30011.avi";
        stereo->calib.intrinsicsFileName = "/home/nicolas/repository/StereoVision/Qt Creator/stereovision_GUI/config/calib/cam2/intrinsics.yml";
        stereo->calib.extrinsicsFileName = "/home/nicolas/repository/StereoVision/Qt Creator/stereovision_GUI/config/calib/cam2/extrinsics.yml";
        stereo->calib.needCalibration=true;
        stereo->calib.hasQMatrix=false;
        break;
    case 7:
        stereo->imageL_filename = "../data/left/left2.png";
        stereo->imageR_filename = "../data/right/right2.png";
        stereo->calib.needCalibration=false;
        stereo->calib.hasQMatrix=false;
        break;
    case 8:
        stereo->imageL_filename = "../data/left/left3.png";
        stereo->imageR_filename = "../data/right/right3.png";
        stereo->calib.needCalibration=false;
        stereo->calib.hasQMatrix=false;
        break;
    }

    /* Identify the type of the input file. */
    if(stereo->imageL_filename.substr(stereo->imageL_filename.find_last_of(".") + 1) == "avi"){
        stereo->calib.inputType = StereoCalib::VideoFile;

        stereo->capL.open(stereo->imageL_filename);
        stereo->capR.open(stereo->imageR_filename);

        if(!stereo->capL.isOpened() || !stereo->capR.isOpened()){		// Check if it succeeded
            cerr <<  "Could not open or find the input videos!" << endl ;
            ui->textBoxOutput->appendPlainText(QString( "Could not open or find the input videos!"));
            //return -1;
        }

        /* Console Output */
        cout << "It's a Video file" << endl;
        cout << "Input 1 Resolution: " << stereo->capR.get(CV_CAP_PROP_FRAME_WIDTH) << "x" << stereo->capR.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
        cout << "Input 2 Resolution: " << stereo->capL.get(CV_CAP_PROP_FRAME_WIDTH) << "x" << stereo->capL.get(CV_CAP_PROP_FRAME_HEIGHT) << endl << endl;

        /* GUI */
        ui->textBoxOutput->appendPlainText(QString("It's a Video file"));
        ui->textBoxOutput->appendPlainText(QString("Input 1 Resolution: ") + QString::number(stereo->capL.get(CV_CAP_PROP_FRAME_WIDTH)) + QString("x") + QString::number(stereo->capL.get(CV_CAP_PROP_FRAME_HEIGHT)));
        ui->textBoxOutput->appendPlainText(QString("Input 2 Resolution: ") + QString::number(stereo->capR.get(CV_CAP_PROP_FRAME_WIDTH)) + QString("x") + QString::number(stereo->capR.get(CV_CAP_PROP_FRAME_HEIGHT)));
    }else{
        cout << "It is not a Video file" << endl;
        ui->textBoxOutput->appendPlainText(QString( "It is not a Video file"));
        if(stereo->imageL_filename.substr(stereo->imageL_filename.find_last_of(".") + 1) == "jpg" || stereo->imageL_filename.substr(stereo->imageL_filename.find_last_of(".") + 1) == "png"){
            cout << "It's a Image file" << endl;
            ui->textBoxOutput->appendPlainText(QString( "It's a Image file"));
            stereo->calib.inputType = StereoCalib::ImageFile;

            stereo->imageL[0] = imread(stereo->imageL_filename, CV_LOAD_IMAGE_COLOR);
            stereo->imageR[0] = imread(stereo->imageR_filename, CV_LOAD_IMAGE_COLOR);

            if(!stereo->imageL[0].data || !stereo->imageR[0].data){      // Check if it succeeded
                ui->textBoxOutput->appendPlainText(QString("Could not open or find the input images!"));
                return;
            }
        }else{
            cout << "It is not a Image file" << endl;
            ui->textBoxOutput->appendPlainText(QString( "It is not a Image file"));
        }
    }
}


void MainWindow::on_btnShowInputImages_clicked(){
    stereo->flags.showInputImages = true;
    stereo->flags.showDisparityMap = false;
    stereo->flags.show3Dreconstruction = false;
    stereo->flags.showTrackingObjectView = false;
    stereo->flags.showDiffImage = false;
    stereo->flags.showWarningLines = false;
}


void MainWindow::on_btnShowDisparityMap_clicked(){
    stereo->flags.showInputImages = false;
    stereo->flags.showDisparityMap = true;
    stereo->flags.show3Dreconstruction = false;
    stereo->flags.showTrackingObjectView = false;
    stereo->flags.showDiffImage = false;
    stereo->flags.showWarningLines = false;
}


void MainWindow::on_btnShow3DReconstruction_clicked(){
    stereo->flags.showInputImages = false;
    stereo->flags.showDisparityMap = false;
    stereo->flags.show3Dreconstruction = true;
    stereo->flags.showTrackingObjectView = false;
    stereo->flags.showDiffImage = false;
    stereo->flags.showWarningLines = false;
}


void MainWindow::on_btnShowTrackingObjectView_clicked(){
    stereo->flags.showInputImages = false;
    stereo->flags.showDisparityMap = false;
    stereo->flags.show3Dreconstruction = false;
    stereo->flags.showTrackingObjectView = true;
    stereo->flags.showDiffImage = false;
    stereo->flags.showWarningLines = false;
}


void MainWindow::on_btnShowDiffImage_clicked(){
    stereo->flags.showInputImages = false;
    stereo->flags.showDisparityMap = false;
    stereo->flags.show3Dreconstruction = false;
    stereo->flags.showTrackingObjectView = false;
    stereo->flags.showDiffImage = true;
    stereo->flags.showWarningLines = false;
}


void MainWindow::on_btnShowWarningLines_clicked(){
    stereo->flags.showInputImages = false;
    stereo->flags.showDisparityMap = false;
    stereo->flags.show3Dreconstruction = false;
    stereo->flags.showTrackingObjectView = false;
    stereo->flags.showDiffImage = false;
    stereo->flags.showWarningLines = true;
}


void MainWindow::on_btnPauseOrResume_clicked(){
    if(tmrTimer->isActive() == true){
        tmrTimer->stop();

        /* Console Output */
        //cout << "Paused!" << endl;

        /* GUI */
        ui->btnPauseOrResume->setText("Resume");
        ui->statusBar->showMessage("Halted!");

    }else{
        tmrTimer->start(20);

        /* Console Output */
        //cout << "Resumed!" << endl;

        /* GUI */
        ui->btnPauseOrResume->setText("Pause");
        ui->statusBar->showMessage("Running...");
    }
}


void MainWindow::on_btnShowStereoParamSetup_clicked(){
    /* Creates  stereoParamsSetupWindow Object */
    stereoParamsSetupWindow = new SetStereoParams(this,stereo);

    cout << "[Stereo Param Setup] Stereo Parameters Configuration Loaded!" << endl;

    switch(stereo->method){
    case StereoProcessor::BM:
        stereoParamsSetupWindow->loadStereoParamsUi(stereo->bm->getPreFilterSize(),
                                                    stereo->bm->getPreFilterCap(),
                                                    stereo->bm->getBlockSize(),
                                                    stereo->bm->getMinDisparity(),
                                                    stereo->bm->getNumDisparities(),
                                                    stereo->bm->getTextureThreshold(),
                                                    stereo->bm->getUniquenessRatio(),
                                                    stereo->bm->getSpeckleWindowSize(),
                                                    stereo->bm->getSpeckleRange(),
                                                    stereo->bm->getDisp12MaxDiff());
        // Debug
        //stereo->BMcfg.showConfigValues();
        break;

    case StereoProcessor::SGBM:
        //TODO: Remover cfgSGBM variables quando for criado uma janela de configuração para cada método.
        //Relembrando: SGBM não tem alguns parametros que o BM tem.
        stereoParamsSetupWindow->loadStereoParamsUi(stereo->cfgSGBM.preFilterSize,
                                                    stereo->sgbm->getPreFilterCap(),
                                                    stereo->sgbm->getBlockSize(),
                                                    stereo->sgbm->getMinDisparity(),
                                                    stereo->sgbm->getNumDisparities(),
                                                    stereo->cfgSGBM.textureThreshold,
                                                    stereo->sgbm->getUniquenessRatio(),
                                                    stereo->sgbm->getSpeckleWindowSize(),
                                                    stereo->sgbm->getSpeckleRange(),
                                                    stereo->sgbm->getDisp12MaxDiff());
        // Debug
        //stereo->SGBMcfg.showConfigValues();

        break;

    case StereoProcessor::BM_GPU:
        //        stereoParamsSetupWindow->loadStereoParamsUi(stereo->cfgBM_GPU.preFilterSize,
        //                                                    stereo->cfgBM_GPU.preFilterCap,
        //                                                    stereo->cfgBM_GPU.SADWindowSize,
        //                                                    stereo->cfgBM_GPU.minDisparity,
        //                                                    stereo->cfgBM_GPU.numberOfDisparities,
        //                                                    stereo->cfgBM_GPU.textureThreshold,
        //                                                    stereo->cfgBM_GPU.uniquenessRatio,
        //                                                    stereo->cfgBM_GPU.speckleWindowSize,
        //                                                    stereo->cfgBM_GPU.speckleRange,
        //                                                    stereo->cfgBM_GPU.disp12MaxDiff);

        stereoParamsSetupWindow->loadStereoParamsUi(stereo->bm_gpu->getPreFilterSize(),
                                                    stereo->bm_gpu->getPreFilterCap(),
                                                    stereo->bm_gpu->getBlockSize(),
                                                    stereo->bm_gpu->getMinDisparity(),
                                                    stereo->bm_gpu->getNumDisparities(),
                                                    stereo->bm_gpu->getTextureThreshold(),
                                                    stereo->bm_gpu->getUniquenessRatio(),
                                                    stereo->bm_gpu->getSpeckleWindowSize(),
                                                    stereo->bm_gpu->getSpeckleRange(),
                                                    stereo->bm_gpu->getDisp12MaxDiff());

        // Debug
        //stereo->cfgBM_GPU.showConfigValues();

        break;

    }

    stereoParamsSetupWindow->show();
}


void MainWindow::on_inputSelector_activated(int index){
    cout << "Chose Input: " << index << endl;

    /* Destroys last stereo object */
    MainWindow::deleteStereoObj();

    /* Create the new stereo object */
    stereo = new StereoProcessor(index);

    /* Forced Restart */
    this->stereoVisionProcessInit();
}


void MainWindow::on_methodSelector_activated(int index){
    switch(index){
    case 0:
        cout << "Chose Method: BM" <<endl;
        stereo->method = StereoProcessor::BM;
        break;
    case 1:
        cout << "Chose Method: SGBM" <<endl;
        stereo->method = StereoProcessor::SGBM;
        break;
    case 2:
        cout << "Chose Method: BM_GPU" <<endl;
        stereo->method = StereoProcessor::BM_GPU;
        break;

    }
}


void MainWindow::closeEvent(QCloseEvent *event){
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Exit",
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes){
        event->ignore();
    } else {
        event->accept();
        ui->~MainWindow();
    }
}


QImage MainWindow::Mat2QImage(const Mat& mat){
    // 8-bits unsigned, NO. OF CHANNELS=1
    if(mat.type()==CV_8UC1){
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }

    // 8-bits unsigned, NO. OF CHANNELS=3
    if(mat.type()==CV_8UC3){
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else{
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}


void MainWindow::putImage(const Mat& src,int windowID){
    switch(windowID){
        case LEFT_WINDOW:
            qimageL = Mat2QImage(src);
            ui->lblOriginalLeft->setPixmap(QPixmap::fromImage(qimageL));
            break;
        case RIGHT_WINDOW:
            qimageR = Mat2QImage(src);
            ui->lblOriginalRight->setPixmap(QPixmap::fromImage(qimageR));
            break;
    }
}


void MainWindow::on_toggleBtnShowHist_clicked(bool checked){
    if(checked){
        cout << "Show Hist 1: On" << endl;
        stereo->flags.showHistograms = true;
    }else{
        cout << "Show Hist 2: Off" << endl;
        stereo->flags.showHistograms = false;
        destroyWindow("Disparity Map Histogram");
        destroyWindow("Left Image Histogram");
    }
}


void MainWindow::on_toggleBtnShowXYZ_toggled(bool checked){
    if(checked){
        cout << "ShowXYZ: On" << endl;
        stereo->flags.showXYZ = true;
    }else{
        cout << "ShowXYZ: Off" << endl;
        stereo->flags.showXYZ = false;
    }
}


void MainWindow::on_toggleBtnShowDispDepth_toggled(bool checked){
    if(checked){
        cout << "Show Disparity/Depth: On" << endl;
        stereo->flags.showDispDepth = true;
    }else{
        cout << "Show Disparity/Depth: Off" << endl;
        stereo->flags.showDispDepth = false;
    }
}


void MainWindow::on_toggleBtnShowLeftImage_toggled(bool checked){
    if(checked){
        cout << "Show Disp BGR Image on the Right Window: On" << endl;
        stereo->flags.showLeftOnRightWindow = true;
        ui->toggleBtnShowLeftImage->setText("ShowDisp");
    }else{
        cout << "Show Disp BGR Image on the Right Window: Off" << endl;
        stereo->flags.showLeftOnRightWindow = false;
        ui->toggleBtnShowLeftImage->setText("ShowLeft");
    }
}


void MainWindow::mousePressEvent(QMouseEvent *e){
    int x_clickedPos;
    int y_clickedPos;

    int x_min = ui->lblOriginalLeft->geometry().x();
    int x_max = x_min + ui->lblOriginalLeft->geometry().width();

    int y_min = ui->lblOriginalLeft->geometry().y();
    int y_max = y_min + ui->lblOriginalLeft->geometry().height();

    x_clickedPos = e->pos().x();
    y_clickedPos = e->pos().y();

    if(x_clickedPos >= x_min && x_clickedPos <= x_max){
        if(y_clickedPos >= y_min && y_clickedPos <= y_max){

            stereo->x = e->pos().x()-x_min;
            stereo->y = e->pos().y()-y_min;

            /* Debug - Mouse Clicked Coordinates (x,y) */
            //cout << "x: " << stereo->x << endl;
            //cout << "y: " << stereo->y << endl;
        }

    }
}


void MainWindow::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
    case Qt::Key_Left:
        stereo->view3D.viewpoint.x-= stereo->view3D.step;
        break;
    case Qt::Key_Right:
        stereo->view3D.viewpoint.x+= stereo->view3D.step;
        break;
    case Qt::Key_Up:
        stereo->view3D.viewpoint.y+= stereo->view3D.step;
        break;
    case Qt::Key_Down:
        stereo->view3D.viewpoint.y-= stereo->view3D.step;
        break;
    case Qt::Key_Plus:
        stereo->view3D.viewpoint.z+= stereo->view3D.step;
        break;
    case Qt::Key_Minus:
        stereo->view3D.viewpoint.z-= stereo->view3D.step;
        break;
    }
}

void MainWindow::updateDisplayWindows(){
    if(stereo->flags.showInputImages){
        putImage(stereo->imageL[0],LEFT_WINDOW);
        putImage(stereo->imageR[0],RIGHT_WINDOW);


        ui->toggleBtnShowHist->hide();
        ui->toggleBtnShowXYZ->hide();
        ui->toggleBtnShowDispDepth->hide();
        ui->toggleBtnShowLeftImage->hide();
    }

    if(stereo->flags.showDisparityMap){
        if(stereo->flags.showLeftOnRightWindow){
            putImage(stereo->imageL[0],LEFT_WINDOW);
        }else{
            putImage(stereo->disp.disp_8U,LEFT_WINDOW);
        }
        putImage(stereo->disp.disp_BGR,RIGHT_WINDOW);

        ui->toggleBtnShowHist->show();
        ui->toggleBtnShowXYZ->hide();
        ui->toggleBtnShowDispDepth->show();
        ui->toggleBtnShowLeftImage->show();
    }

    if(stereo->flags.show3Dreconstruction){
        putImage(stereo->view3D.disp3D_8U,LEFT_WINDOW);
        putImage(stereo->view3D.disp3D_BGR,RIGHT_WINDOW);

        ui->toggleBtnShowHist->hide();
        ui->toggleBtnShowXYZ->show();
        ui->toggleBtnShowDispDepth->hide();
        ui->toggleBtnShowLeftImage->hide();
    }

    if(stereo->flags.showTrackingObjectView){
        putImage(stereo->morph.trackingView,LEFT_WINDOW);
        putImage(stereo->morph.imgThresholdDraw,RIGHT_WINDOW);

        ui->toggleBtnShowHist->hide();
        ui->toggleBtnShowXYZ->hide();
        ui->toggleBtnShowDispDepth->hide();
        ui->toggleBtnShowLeftImage->hide();
    }

    if(stereo->flags.showDiffImage && stereo->diff.StartDiff){
        putImage(stereo->diff.diffImage,LEFT_WINDOW);
        putImage(stereo->diff.res_AND,RIGHT_WINDOW);

        ui->toggleBtnShowHist->hide();
        ui->toggleBtnShowXYZ->hide();
        ui->toggleBtnShowDispDepth->hide();
        ui->toggleBtnShowLeftImage->hide();
    }

    if(stereo->flags.showWarningLines && stereo->diff.StartDiff){
        putImage(stereo->diff.res_ADD,LEFT_WINDOW);
        putImage(stereo->diff.res_AND_BGR,RIGHT_WINDOW);

        ui->toggleBtnShowHist->hide();
        ui->toggleBtnShowXYZ->hide();
        ui->toggleBtnShowDispDepth->hide();
        ui->toggleBtnShowLeftImage->hide();
    }
}
