// reprojectImageTo3D - BlockMatching Algorithm
// Nicolas Rosa, June 2015.
// Credits: http://opencv.jp/opencv2-x-samples/point-cloud-rendering

// Libraries
#include "reprojectImageTo3D.h"

//Qt Creator Libs
#include <QtCore>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;
using namespace std;

//MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow),stereo(new StereoProcessor(6)){
MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);

    this->stereo = new StereoProcessor(6);

    StereoVisionProcessInit();

    tmrTimer = new QTimer(this);
    connect(tmrTimer,SIGNAL(timeout()),this,SLOT(StereoVisionProcessAndUpdateGUI()));
    tmrTimer->start(30);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_btnPauseOrResume_clicked(){
    if(tmrTimer->isActive() == true){
        tmrTimer->stop();
        cout << "Paused!" << endl;
        ui->btnPauseOrResume->setText("Resume");
    }else{
        tmrTimer->start(20);
        cout << "Resumed!" << endl;
        ui->btnPauseOrResume->setText("Pause");
    }
}

void MainWindow::on_btnShowStereoParamSetup_clicked(){
    static bool isAlreadyInitialized;

    setstereoparams = new SetStereoParams(this);
    setstereoparams->show();

    if(!isAlreadyInitialized){
        this->setstereoparams->setInitialStereoParamsUi(stereo->stereocfg.preFilterSize,
                                                       stereo->stereocfg.preFilterCap,
                                                       stereo->stereocfg.SADWindowSize,
                                                       stereo->stereocfg.minDisparity,
                                                       stereo->stereocfg.numberOfDisparities,
                                                       stereo->stereocfg.textureThreshold,
                                                       stereo->stereocfg.uniquenessRatio,
                                                       stereo->stereocfg.speckleWindowSize,
                                                       stereo->stereocfg.speckleRange,
                                                       stereo->stereocfg.disp12MaxDiff);
        isAlreadyInitialized = true;
        cout << "[Stereo Param Setup] Initialized!" << endl;
    }else{
        this->setstereoparams->getStereoParamsUi();
        cout << "[Stereo Param Setup] Last Stereo Params Values Loaded!" << endl;
    }
}

void MainWindow::on_btnShowInputImages_clicked(){
    showInputImages = true;
    showDisparityMap = false;
    show3Dreconstruction = false;
}

void MainWindow::on_btnShowDisparityMap_clicked(){
    showInputImages = false;
    showDisparityMap = true;
    show3Dreconstruction = false;
}

void MainWindow::on_btnShow3DReconstruction_clicked(){
    showInputImages = false;
    showDisparityMap = false;
    show3Dreconstruction = true;
}

void MainWindow::on_btnShowTrackingObjectView_clicked()
{

}

void MainWindow::StereoVisionProcessInit(){
    printHelp();

    cerr << "Arrumar a Matrix K, os valores das últimas colunas estão errados." << endl;
    cerr << "Arrumar a função StereoProcessor::calculateQMatrix()." << endl;
    cerr << "Arrumar o Constructor da classe StereoDisparityMap para Alocação de Memória das variáveis: disp_16S,disp_8U,disp_BGR" << endl;
    cerr << "Arrumar o tipo de execução da Stereo Param Setup, fazer com que a execução da main não pause." << endl;
    cerr << "Arrumar a funcionalidade do Botão Pause/Resume, não está funcionando." << endl;

    //(1) Open Image Source
    openStereoSource(stereo->getInputNum());
    stereo->readConfigFile();
    stereo->readStereoConfigFile();

    //(2) Camera Setting

    // Checking Resolution
    stereo->calib.is320x240  = false;
    stereo->calib.is640x480  = true;
    stereo->calib.is1280x720 = false;

    if(isVideoFile){
        stereo->imageSize.width = stereo->capL.get(CV_CAP_PROP_FRAME_WIDTH);
        stereo->imageSize.height = stereo->capL.get(CV_CAP_PROP_FRAME_HEIGHT);
    }else{
        stereo->imageSize.width = stereo->imageL[0].cols;
        stereo->imageSize.height = stereo->imageL[0].rows;
    }

    if(stereo->imageSize.width==0 && stereo->imageSize.height==0){
        cerr << "Number of Cols and Number of Rows equal to ZERO!" << endl;
    }else{
        cout << "Input Resolution(Width,Height): (" << stereo->imageSize.width << "," << stereo->imageSize.height << ")" << endl << endl;
    }

    //(3) Stereo Initialization
    stereo->bm = StereoBM::create(16,9);
    stereo->stereoInit();

    //(4) Stereo Calibration
    if(needCalibration){
        cout << "Calibration: ON" << endl;
        stereo->stereoCalib();

        // Compute the Q Matrix
        stereo->calib.readQMatrix(); //true=640x480 false=others

        //Point2d imageCenter = Point2d((imageL[0].cols-1.0)/2.0,(imageL[0].rows-1.0)/2.0);
        //calculateQMatrix();

        // Compute the K Matrix
        ////        // Checking Intrinsic Matrix
        ////        if(stereo->calib.isKcreated){
        ////           cout << "The Intrinsic Matrix is already Created." << endl << endl;
        ////        }else{
        //            //createKMatrix();
        // //       }
        stereo->calib.createKMatrix();

    }else{
        cout << "Calibration: OFF" << endl << endl;
        cerr << "Warning: Couldn't generate 3D Reconstruction. Please, check Q,K Matrix." << endl;

        //stereo->readQMatrix(); //true=640x480 false=others
        //stereo->createKMatrix();
    }

    //(5) Point Cloud Initialization
    stereo->view3D.PointCloudInit(stereo->calib.baseline/10,true);

    stereo->view3D.setViewPoint(20.0,20.0,-stereo->calib.baseline*10);
    stereo->view3D.setLookAtPoint(22.0,16.0,stereo->calib.baseline*10.0);

    //isStereoParamSetupTrackbarsCreated=createTrackbars();
    //createTrackbars();
}

void MainWindow::StereoVisionProcessAndUpdateGUI(){
    //Local Variables
    char key=0;

    //Diff
    bool StartDiff=false;
    Mat diffImage;

    //Timing
    int frameCounter=0;
    float fps,lastTime = clock();

    //(6) Rendering Loop
    while(key!='q'){
        if(isVideoFile){
            stereo->capL >> stereo->imageL[0];
            stereo->capR >> stereo->imageR[0];

            resizeFrames(&stereo->imageL[0],&stereo->imageR[0]);

            if(needCalibration){
                stereo->imageSize = stereo->imageL[0].size();
                stereoRectify(stereo->calib.M1,stereo->calib.D1,stereo->calib.M2,stereo->calib.D2,stereo->imageSize,stereo->calib.R,stereo->calib.T,stereo->calib.R1,stereo->calib.R2,stereo->calib.P1,stereo->calib.P2,stereo->calib.Q,CALIB_ZERO_DISPARITY,-1,stereo->imageSize,&stereo->calib.roi1,&stereo->calib.roi2);
                Mat rmap[2][2];

                initUndistortRectifyMap(stereo->calib.M1, stereo->calib.D1, stereo->calib.R1, stereo->calib.P1, stereo->imageSize, CV_16SC2, rmap[0][0], rmap[0][1]);
                initUndistortRectifyMap(stereo->calib.M2, stereo->calib.D2, stereo->calib.R2, stereo->calib.P2, stereo->imageSize, CV_16SC2, rmap[1][0], rmap[1][1]);
                Mat imageLr, imageRr;
                remap(stereo->imageL[0], imageLr, rmap[0][0], rmap[0][1], INTER_LINEAR);
                remap(stereo->imageR[0], imageRr, rmap[1][0], rmap[1][1], INTER_LINEAR);

                stereo->imageL[0] = imageLr;
                stereo->imageR[0] = imageRr;
            }
        }

        //Setting StereoBM Parameters
        stereo->stereoSetParams();

        // Convert BGR to Grey Scale
        cvtColor(stereo->imageL[0],stereo->imageL_grey[0],CV_BGR2GRAY);
        cvtColor(stereo->imageR[0],stereo->imageR_grey[0],CV_BGR2GRAY);

        stereo->bm->compute(stereo->imageL_grey[0],stereo->imageR_grey[0],stereo->disp.disp_16S);
        //fillOcclusion(disp,16,false);

        normalize(stereo->disp.disp_16S, stereo->disp.disp_8U, 0, 255, CV_MINMAX, CV_8U);
        applyColorMap(stereo->disp.disp_8U,stereo->disp.disp_BGR, COLORMAP_JET);

        /* Image Processing */
        Mat disp_8Median,disp_8MedianBGR;
        Mat disp_8eroded;Mat disp_8_eroded_dilated;

        //imageProcessing1(disp8,disp8Median,disp8Median);
        //imageProcessing2(disp8,disp8eroded,disp8_eroded_dilated,imageL[0],true);

        //(7) Projecting 3D point cloud to image
        if(show3Dreconstruction){
            cv::reprojectImageTo3D(stereo->disp.disp_16S,stereo->view3D.depth,stereo->calib.Q);
            Mat xyz= stereo->view3D.depth.reshape(3,stereo->view3D.depth.size().area());

            stereo->view3D.lookat(stereo->view3D.viewpoint, stereo->view3D.lookatpoint , stereo->view3D.Rotation);
            stereo->view3D.t.at<double>(0,0)=stereo->view3D.viewpoint.x;
            stereo->view3D.t.at<double>(1,0)=stereo->view3D.viewpoint.y;
            stereo->view3D.t.at<double>(2,0)=stereo->view3D.viewpoint.z;

            if(showXYZ){
                //cout<<t<<endl;
                cout << "x: " << stereo->view3D.t.at<double>(0,0) << endl;
                cout << "y: " << stereo->view3D.t.at<double>(1,0) << endl;
                cout << "z: " << stereo->view3D.t.at<double>(2,0) << endl;
            }

            stereo->view3D.t=stereo->view3D.Rotation*stereo->view3D.t;

            //projectImagefromXYZ(imageL[0],disp3Dviewer,disp,disp3D,xyz,Rotation,t,K,dist,isSub);
            stereo->view3D.projectImagefromXYZ(stereo->disp.disp_BGR,stereo->view3D.disp3D_BGR,stereo->disp.disp_16S,stereo->view3D.disp3D,xyz,stereo->view3D.Rotation,stereo->view3D.t,stereo->calib.K,stereo->view3D.dist,stereo->view3D.isSub);

            // GUI Output
            stereo->view3D.disp3D.convertTo(stereo->view3D.disp3D_8U,CV_8U,0.5);
            //imshow("3D Depth",disp3D);
            //imshow("3D Viewer",disp3Dviewer);
            //imshow("3D Depth RGB",disp3DBGR);

            QImage qimageL = putImage(stereo->view3D.disp3D_8U);
            QImage qimageR = putImage(stereo->view3D.disp3D_BGR);

            ui->lblOriginalLeft->setPixmap(QPixmap::fromImage(qimageL));
            ui->lblOriginalRight->setPixmap(QPixmap::fromImage(qimageR));
        }
        else{
            destroyWindow("3D Depth");
            destroyWindow("3D Viewer");
            destroyWindow("3D Depth RGB");
        }

        //(8)Movement Difference between Frames
        Mat thresholdImage;
        if(StartDiff && showDiffImage){
            absdiff(stereo->imageL_grey[0],stereo->imageL_grey[1],diffImage);
            threshold(diffImage, thresholdImage, THRESH_VALUE, 255,THRESH_BINARY);
        }

        //Saving Previous Frame
        stereo->imageL[0].copyTo(stereo->imageL[1]);
        stereo->imageR[0].copyTo(stereo->imageR[1]);
        stereo->imageL_grey[0].copyTo(stereo->imageL_grey[1]);
        stereo->imageR_grey[0].copyTo(stereo->imageR_grey[1]);

        //imshow("Previous Left",imageL[1]);
        //imshow("Previous Right",imageR[1]);

        StartDiff=1;

        //(9)OpenCV and GUI Output
        if(showInputImages){
            //  imshow("Left",imageL[0]);
            //  imshow("Right",imageR[0]);

            QImage qimageL = putImage(stereo->imageL[0]);
            QImage qimageR = putImage(stereo->imageR[0]);

            ui->lblOriginalLeft->setPixmap(QPixmap::fromImage(qimageL));
            ui->lblOriginalRight->setPixmap(QPixmap::fromImage(qimageR));
        }
        else{
            destroyWindow("Left");
            destroyWindow("Right");
        }

        if(showDisparityMap){
            //imshow("Disparity Map",disp_8U);
            //imshow("Disparity Map BGR",disp_BGR);

            QImage qimageL = putImage(stereo->disp.disp_8U);
            QImage qimageR = putImage(stereo->disp.disp_BGR);

            ui->lblOriginalLeft->setPixmap(QPixmap::fromImage(qimageL));
            ui->lblOriginalRight->setPixmap(QPixmap::fromImage(qimageR));
        }
        else{
            destroyWindow("Disparity Map");
            destroyWindow("Disparity Map BGR");
        }

        //       // if(showStereoParam && !isStereoParamSetupTrackbarsCreated){
        //       if(showStereoParam && !isStereoParamSetupTrackbarsCreated){
        //           isStereoParamSetupTrackbarsCreated=true;
        //           createTrackbars();
        //            cout << "oi" << endl;
        //        }else{
        //            destroyWindow(trackbarWindowName);
        //            isStereoParamSetupTrackbarsCreated=false;
        //        }

        if(showDiffImage){
            imshow("DiffImage",diffImage);
            imshow("thresoldImage",thresholdImage);
        }
        else{
            destroyWindow("DiffImage");
        }

        //(10)Shortcuts
        key = waitKey(1);
        if(key=='`')
            printHelp();
        //        if(key=='1')
        //            showInputImages = !showInputImages;
        //        if(key=='2')
        //            showDisparityMap = !showDisparityMap;
        //        if(key=='3')
        //            show3Dreconstruction = !show3Dreconstruction;
        if(key=='4')
            showXYZ = !showXYZ;
        if(key=='5')
            showFPS = !showFPS;
        if(key=='6')
            showStereoParamValues = !showStereoParamValues;
        if(key=='7')
            showDiffImage = !showDiffImage;

        if(key=='f')
            stereo->view3D.isSub=stereo->view3D.isSub?false:true;
        if(key=='h')
            stereo->view3D.viewpoint.x+=stereo->view3D.step;
        if(key=='g')
            stereo->view3D.viewpoint.x-=stereo->view3D.step;
        if(key=='l')
            stereo->view3D.viewpoint.y+=stereo->view3D.step;
        if(key=='k')
            stereo->view3D.viewpoint.y-=stereo->view3D.step;
        if(key=='n')
            stereo->view3D.viewpoint.z+=stereo->view3D.step;
        if(key=='m')
            stereo->view3D.viewpoint.z-=stereo->view3D.step;

        if(key=='q')
            break;

        //(11)Video Loop - If the last frame is reached, reset the capture and the frameCounter
        frameCounter += 1;

        if(frameCounter == stereo->capR.get(CV_CAP_PROP_FRAME_COUNT)){
            frameCounter = 0;
            stereo->capL.set(CV_CAP_PROP_POS_FRAMES,0);
            stereo->capR.set(CV_CAP_PROP_POS_FRAMES,0);
        }

        if(showFPS){
            //cout << "Frames: " << frameCounter << "/" << capR.get(CV_CAP_PROP_FRAME_COUNT) << endl;
            //cout << "Current time(s): " << current_time << endl;
            //cout << "FPS: " << (frameCounter/current_time) << endl;
            fps = (int) (1000/((clock()/1000) - lastTime)); // time stuff
            lastTime = clock()/1000;
            //cout << clock() << endl;
            cout << "FPS: " << fps << endl; // faster than draw??
        }
    }
    cout << "END" << endl;

    //return 0;
}

void MainWindow::printHelp(){
    //Console Output
    cout << "-----------------Help Menu-----------------\n"
         << "Run command line: ./reprojectImageTo3D\n"
         << "Keys:\n"
         << "'`' -\tShow Help\n"
         << "'1' -\tShow L/R Windows\t\t'4' -\tShow XYZ\n"
         << "'2' -\tShow Disparity Map\t\t'5' -\tShow FPS\n"
         << "'3' -\tShow 3D Reconstruction\t'6' -\tShow Stereo Parameters\n"
         << "\n3D Viewer Navigation:\n"
         << "x-axis:\t'g'/'h' -> +x,-x\n"
         << "y-axis:\t'l'/'k' -> +y,-y\n"
         << "z-axis:\t'n'/'m' -> +z,-z\n"
         << "-------------------------------------------\n"
         << "\n\n";

    //GUI
    ui->txtOutputBox->appendPlainText
            (QString("-----------------Help Menu-----------------\n")+
             QString("Run command line: ./reprojectImageTo3D\n")+
             QString("Keys:\n")+
             QString("'`' -\tShow Help\n")+
             QString("'1' -\tShow L/R Windows\t\t'4' -\tShow XYZ\n")+
             QString("'2' -\tShow Disparity Map\t\t'5' -\tShow FPS\n")+
             QString("'3' -\tShow 3D Reconstruction\t'6' -\tShow Stereo Parameters\n")+
             QString("\n3D Viewer Navigation:\n")+
             QString("x-axis:\t'g'/'h' -> +x,-x\n")+
             QString("y-axis:\t'l'/'k' -> +y,-y\n")+
             QString("z-axis:\t'n'/'m' -> +z,-z\n")+
             QString("-------------------------------------------\n")+
             QString("\n\n"));
}

void MainWindow::openStereoSource(int inputNum){
    std::string imageL_filename;
    std::string imageR_filename;

    // Create an object that decodes the input Video stream.
    ui->txtOutputBox->appendPlainText(QString("Enter Video Number(1,2,3,4,5,6,7,8,9): "));
    //	scanf("%d",&inputNum);
    ui->txtOutputBox->appendPlainText(QString("Input File: ")+QString::number(inputNum));
    switch(inputNum){
    case 1:
        imageL_filename = "../data/left/video2_denoised_long.avi";
        imageR_filename = "../data/right/video2_denoised_long.avi";
        needCalibration=true;
        ui->txtOutputBox->appendPlainText(QString("video2_denoised_long.avi"));
        break;
    case 2:
        imageL_filename = "../data/left/video0.avi";
        imageR_filename = "../data/right/video0.avi";
        needCalibration=false;
        ui->txtOutputBox->appendPlainText(QString( "video0.avi"));
        break;
    case 3:
        imageL_filename = "../data/left/video1.avi";
        imageR_filename = "../data/right/video1.avi";
        needCalibration=true;
        ui->txtOutputBox->appendPlainText(QString( "video1.avi"));
        break;
    case 4:
        imageL_filename = "../data/left/video2_noised.avi";
        imageR_filename = "../data/right/video2_noised.avi";
        needCalibration=true;
        ui->txtOutputBox->appendPlainText(QString( "video2_noised.avi"));
        break;
    case 5:
        imageL_filename = "../data/left/20004.avi";
        imageR_filename = "../data/right/30004.avi";
        needCalibration=false;
        break;
    case 6:
        imageL_filename = "../../workspace/data/left/video15.avi";
        imageR_filename = "../../workspace/data/right/video15.avi";
        needCalibration=true;
        break;
    case 7:
        imageL_filename = "../../workspace/data/left/left1.png";
        imageR_filename = "../../workspace/data/right/right1.png";
        needCalibration=false;
        break;
    case 8:
        imageL_filename = "../data/left/left2.png";
        imageR_filename = "../data/right/right2.png";
        needCalibration=false;
        break;
    case 9:
        imageL_filename = "../data/left/left3.png";
        imageR_filename = "../data/right/right3.png";
        needCalibration=false;
        break;
    }

    if(imageL_filename.substr(imageL_filename.find_last_of(".") + 1) == "avi"){
        ui->txtOutputBox->appendPlainText(QString("It's a Video file"));
        isVideoFile=true;

        stereo->capL.open(imageL_filename);
        stereo->capR.open(imageR_filename);

        if(!stereo->capL.isOpened() || !stereo->capR.isOpened()){		// Check if we succeeded
            ui->txtOutputBox->appendPlainText(QString( "Could not open or find the input videos!"));
            //return -1;
        }

        ui->txtOutputBox->appendPlainText(QString("Input 1 Resolution: ") + QString::number(stereo->capL.get(CV_CAP_PROP_FRAME_WIDTH)) + QString("x") + QString::number(stereo->capL.get(CV_CAP_PROP_FRAME_HEIGHT)));
        ui->txtOutputBox->appendPlainText(QString("Input 2 Resolution: ") + QString::number(stereo->capR.get(CV_CAP_PROP_FRAME_WIDTH)) + QString("x") + QString::number(stereo->capR.get(CV_CAP_PROP_FRAME_HEIGHT)));
    }else{
        ui->txtOutputBox->appendPlainText(QString( "It is not a Video file"));
        if(imageL_filename.substr(imageL_filename.find_last_of(".") + 1) == "jpg" || imageL_filename.substr(imageL_filename.find_last_of(".") + 1) == "png"){
            ui->txtOutputBox->appendPlainText(QString( "It's a Image file"));
            isImageFile=true;

            stereo->imageL[0] = imread(imageL_filename, CV_LOAD_IMAGE_COLOR);	// Read the file
            stereo->imageR[0] = imread(imageR_filename, CV_LOAD_IMAGE_COLOR);	// Read the file

            if(!stereo->imageL[0].data || !stereo->imageR[0].data){                     // Check for invalid input
                ui->txtOutputBox->appendPlainText(QString("Could not open or find the input images!"));
                return;
            }
        }else{
            ui->txtOutputBox->appendPlainText(QString( "It is not a Image file"));
        }
    }
}

//void createTrackbars(){ //Create Window for trackbars
//    char TrackbarName[50];

//    // Create TrackBars Window
//    namedWindow(trackbarWindowName,0);

//    // Create memory to store Trackbar name on window
//    sprintf( TrackbarName, "preFilterSize");
//    sprintf( TrackbarName, "preFilterCap");
//    sprintf( TrackbarName, "SADWindowSize");
//    sprintf( TrackbarName, "minDisparity");
//    sprintf( TrackbarName, "numberOfDisparities");
//    sprintf( TrackbarName, "textureThreshold");
//    sprintf( TrackbarName, "uniquenessRatio");
//    sprintf( TrackbarName, "speckleWindowSize");
//    sprintf( TrackbarName, "speckleRange");
//    sprintf( TrackbarName, "disp12MaxDiff");

//    //Create Trackbars and insert them into window
//    stereo.

//    createTrackbar( "preFilterSize", trackbarWindowName, &preFilterSize, preFilterSize_MAX, on_trackbar );
//    createTrackbar( "preFilterCap", trackbarWindowName, &preFilterCap, preFilterCap_MAX, on_trackbar );
//    createTrackbar( "SADWindowSize", trackbarWindowName, &SADWindowSize, SADWindowSize_MAX, on_trackbar );
//    createTrackbar( "minDisparity", trackbarWindowName, &minDisparity, minDisparity_MAX, on_trackbar );
//    createTrackbar( "numberOfDisparities", trackbarWindowName, &numberOfDisparities, numberOfDisparities_MAX, on_trackbar );
//    createTrackbar( "textureThreshold", trackbarWindowName, &textureThreshold, textureThreshold_MAX, on_trackbar );
//    createTrackbar( "uniquenessRatio", trackbarWindowName, &uniquenessRatio, uniquenessRatio_MAX, on_trackbar );
//    createTrackbar( "speckleWindowSize", trackbarWindowName, &speckleWindowSize, speckleWindowSize_MAX, on_trackbar );
//    createTrackbar( "speckleRange", trackbarWindowName, &speckleRange, speckleRange_MAX, on_trackbar );
//    createTrackbar( "disp12MaxDiff", trackbarWindowName, &disp12MaxDiff, disp12MaxDiff_MAX, on_trackbar );
//}

void on_trackbar(int,void*){}; //This function gets called whenever a trackbar position is changed

void resizeFrames(Mat* frame1,Mat* frame2){
    if(frame1->cols != 0 || !frame2->cols != 0){
#ifdef RESOLUTION_320x240
        resize(*frame1, *frame1, Size(320,240), 0, 0, INTER_CUBIC);
        resize(*frame2, *frame2, Size(320,240), 0, 0, INTER_CUBIC);
#endif

#ifdef RESOLUTION_640x480
        resize(*frame1, *frame1, Size(640,480), 0, 0, INTER_CUBIC);
        resize(*frame2, *frame2, Size(640,480), 0, 0, INTER_CUBIC);
#endif

#ifdef RESOLUTION_1280x720
        resize(*frame1, *frame1, Size(1280,720), 0, 0, INTER_CUBIC);
        resize(*frame2, *frame2, Size(1280,720), 0, 0, INTER_CUBIC);
#endif
    }
}

void change_resolution(VideoCapture* capL,VideoCapture* capR){
#ifdef RESOLUTION_320x240
    capL->set(CV_CAP_PROP_FRAME_WIDTH, 320);
    capL->set(CV_CAP_PROP_FRAME_HEIGHT,240);
    capR->set(CV_CAP_PROP_FRAME_WIDTH, 320);
    capR->set(CV_CAP_PROP_FRAME_HEIGHT,240);
#endif

#ifdef RESOLUTION_640x480
    capL->set(CV_CAP_PROP_FRAME_WIDTH, 640);
    capL->set(CV_CAP_PROP_FRAME_HEIGHT,480);
    capR->set(CV_CAP_PROP_FRAME_WIDTH, 640);
    capR->set(CV_CAP_PROP_FRAME_HEIGHT,480);
#endif

#ifdef RESOLUTION_1280x960
    capL->set(CV_CAP_PROP_FRAME_WIDTH,1280);
    capL->set(CV_CAP_PROP_FRAME_HEIGHT,720);
    capR->set(CV_CAP_PROP_FRAME_WIDTH,1280);
    capR->set(CV_CAP_PROP_FRAME_HEIGHT,720);
#endif

    cout << "Camera 1 Resolution: " << capL->get(CV_CAP_PROP_FRAME_WIDTH) << "x" << capL->get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
    cout << "Camera 2 Resolution: " << capR->get(CV_CAP_PROP_FRAME_WIDTH) << "x" << capR->get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
}

void imageProcessing1(Mat Image, Mat MedianImage, Mat MedianImageBGR){

    // Apply Median Filter
    medianBlur(Image,MedianImage,5);
    applyColorMap(MedianImage,MedianImageBGR, COLORMAP_JET);

    // Output
    imshow("Disparity Map Median Filter 3x3",MedianImage);
    imshow("Disparity Map Median Filter 3x3 - RGB",MedianImageBGR);
}

void imageProcessing2(Mat src, Mat imgE, Mat imgED,Mat cameraFeedL,bool isTrackingObjects){
    Mat erosionElement = getStructuringElement( MORPH_RECT,Size( 2*EROSION_SIZE + 1, 2*EROSION_SIZE+1 ),Point( EROSION_SIZE, EROSION_SIZE ) );
    Mat dilationElement = getStructuringElement( MORPH_RECT,Size( 2*DILATION_SIZE + 1, 2*DILATION_SIZE+1 ),Point( DILATION_SIZE, DILATION_SIZE ) );
    Mat imgEBGR,imgEDBGR;
    Mat imgEDMedian,imgEDMedianBGR;
    int x,y;

    Mat imgThreshold;			static Mat lastimgThreshold;
    int nPixels,nTotal;		  	//static int lastThresholdSum=0;

    // Near Object Detection

    //Prefiltering
    // Apply Erosion and Dilation to take out spurious noise
    erode(src,imgE,erosionElement);
    dilate(imgE,imgED,erosionElement);

    applyColorMap(imgE,imgEBGR, COLORMAP_JET);
    applyColorMap(imgED,imgEDBGR, COLORMAP_JET);

    // Apply Median Filter
    //GaussianBlur(imgED,imgEDMedian,Size(3,3),0,0);
    medianBlur(imgED,imgEDMedian,5);
    applyColorMap(imgEDMedian,imgEDMedianBGR, COLORMAP_JET);

    // Thresholding
    //adaptiveThreshold(imgEDMedian,imgThreshold,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,11,-1);
    //adaptiveThreshold(imgEDMedian,imgThreshold,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,11,0);
    threshold(imgEDMedian, imgThreshold, THRESH_VALUE, 255,THRESH_BINARY);
    erode(imgThreshold,imgThreshold,erosionElement);
    dilate(imgThreshold,imgThreshold,dilationElement);

    // Solving Lighting Noise Problem
    nPixels = sum(imgThreshold)[0]/255;
    nTotal = imgThreshold.total();

    //	cout << "Number of Pixels:" << nPixels << endl;
    //	cout << "Ratio is: " << ((float)nPixels)/nTotal << endl << endl;

    if((((float)nPixels)/nTotal)>0.5){
        //		sleep(1);
        //		cout << "Lighting Noise!!!" << endl;
        //		cout << "Number of Pixels:" << nPixels << endl;
        //		cout << "Ratio is: " << ((float)nPixels)/nTotal << endl << endl;

        // Invalidates the last frame
        imgThreshold = lastimgThreshold;
    }else{
        // Saves the last valid frame
        lastimgThreshold=imgThreshold;
        //lastThresholdSum = CurrentThresholdSum;
    }

    // Output
    //	imshow("Eroded Image",imgE);
    //	imshow("Eroded Image BGR",imgEBGR);
    //
    //	imshow("Eroded+Dilated Image",imgED);
    //	imshow("Eroded+Dilated Image BGR",imgEDBGR);

    imshow("Eroded+Dilated+Median Image",imgEDMedian);
    imshow("Eroded+Dilated+Median Image BGR",imgEDMedianBGR);

    imshow("Thresholded Image",imgThreshold);

    // Tracking Object
    if(isTrackingObjects){
        trackFilteredObject(x,y,imgThreshold,cameraFeedL);
        imshow("Tracking Object",cameraFeedL);
    }
}

void contrast_and_brightness(Mat &left,Mat &right,float alpha,float beta){
    //Contrast and Brightness. Do the operation: new_image(i,j) = alpha*image(i,j) + beta
    for( int y = 0; y < left.rows; y++ ){
        for( int x = 0; x < left.cols; x++ ){
            for( int c = 0; c < 3; c++ ){
                left .at<Vec3b>(y,x)[c] = saturate_cast<uchar>( alpha*( left .at<Vec3b>(y,x)[c] ) + beta );
                right.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( alpha*( right.at<Vec3b>(y,x)[c] ) + beta );
            }
        }
    }
}

QImage MainWindow::putImage(const Mat& mat){
    // 8-bits unsigned, NO. OF CHANNELS=1
    if(mat.type()==CV_8UC1)
    {
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
    if(mat.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}
