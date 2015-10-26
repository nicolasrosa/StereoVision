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

//MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow),stereo(StereoProcessor::StereoProcessor(6)){
MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow),StereoProcessor(6){
    ui->setupUi(this);

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
        ui->btnPauseOrResume->setText("Resume");
    }else{
        tmrTimer->start(20);
        ui->btnPauseOrResume->setText("Pause");
    }
}

void MainWindow::on_btnShowStereoParamSetup_clicked(){
    //showStereoParam = !showStereoParam;

    SetStereoParams SetStereoParamsWindow;
    SetStereoParamsWindow.setModal(true);
    SetStereoParamsWindow.exec();
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
    cerr << "Arrumar a estrutura do código, evitar que a inicialização seja executada mais de uma vez." << endl;

    openStereoSource(this->getInputNum(),&capL,&capR,&imageL[0],&imageR[0]);
    this->readConfigFile();

    //(1) Open Image Source
    //StereoProcessor stereo(6);  //Initialize StereoProcessor Object

    //openStereoSource(this->getInputNum(),&capL,&capR,&imageL[0],&imageR[0]);
    //this->readConfigFile();

    //(5) Camera Setting

    // Checking Resolution
    this->calib.is320x240  = false;
    this->calib.is640x480  = true;
    this->calib.is1280x720 = false;

    if(isVideoFile){
        this->imageSize.width = capL.get(CV_CAP_PROP_FRAME_WIDTH);
        this->imageSize.height = capL.get(CV_CAP_PROP_FRAME_HEIGHT);
    }else{
        this->imageSize.width = imageL[0].cols;
        this->imageSize.height = imageL[0].rows;
    }

    if(this->imageSize.width==0 && this->imageSize.height==0){
        cerr << "Number of Cols and Number of Rows equal to ZERO!" << endl;
    }else{
        cout << "Input Resolution(Width,Height): (" << this->imageSize.width << "," << this->imageSize.height << ")" << endl << endl;
    }

    //(2) Stereo Initialization
    this->bm = StereoBM::create(16,9);
    this->stereoInit();

    //(3) Stereo Calibration
    Mat M1,D1,M2,D2;
    Mat R,T,R1,P1,R2,P2;
    Rect roi1, roi2;

    if(needCalibration){
        cout << "Calibration: ON" << endl;
        this->stereoCalib();

        //(4) Compute the Q Matrix
        this->readQMatrix(); //true=640x480 false=others

        //Point2d imageCenter = Point2d((imageL[0].cols-1.0)/2.0,(imageL[0].rows-1.0)/2.0);
        //this->calculateQMatrix();

        //(5) Camera Setting
        ////        // Checking Intrinsic Matrix
        ////        if(this->calib.isKcreated){
        ////           cout << "The Intrinsic Matrix is already Created." << endl << endl;
        ////        }else{
        //            //this->createKMatrix();
        // //       }
        this->createKMatrix();

    }else{
        cout << "Calibration: OFF" << endl << endl;
    }

    //(6) Point Cloud Initialization
    this->view3D.PointCloudInit(this->calib.baseline/10,true);

    this->view3D.setViewPoint(20.0,20.0,-this->calib.baseline*10);
    this->view3D.setLookAtPoint(22.0,16.0,this->calib.baseline*10.0);



    isStereoParamSetupTrackbarsCreated=createTrackbars();
    //createTrackbars();

}

void MainWindow::StereoVisionProcessAndUpdateGUI(){
    //Local Variables
    char key=0;
    bool isBGR2RGBflipped = false;

    //Diff
    bool StartDiff=false;
    Mat diffImage;

    //Timing
    int frameCounter=0;
    float fps,lastTime = clock();

    //(7) Rendering Loop
    while(key!='q'){
        if(isVideoFile){
            capL >> imageL[0];
            capR >> imageR[0];

            resizeFrames(&imageL[0],&imageR[0]);

            if(needCalibration){
                this->imageSize = imageL[0].size();
                stereoRectify(this->calib.M1,this->calib.D1,this->calib.M2,this->calib.D2,this->imageSize,this->calib.R,this->calib.T,this->calib.R1,this->calib.R2,this->calib.P1,this->calib.P2,this->calib.Q,CALIB_ZERO_DISPARITY,-1,this->imageSize,&this->calib.roi1,&this->calib.roi2);
                Mat rmap[2][2];

                initUndistortRectifyMap(this->calib.M1, this->calib.D1, this->calib.R1, this->calib.P1, this->imageSize, CV_16SC2, rmap[0][0], rmap[0][1]);
                initUndistortRectifyMap(this->calib.M2, this->calib.D2, this->calib.R2, this->calib.P2, this->imageSize, CV_16SC2, rmap[1][0], rmap[1][1]);
                Mat imageLr, imageRr;
                remap(imageL[0], imageLr, rmap[0][0], rmap[0][1], INTER_LINEAR);
                remap(imageR[0], imageRr, rmap[1][0], rmap[1][1], INTER_LINEAR);

                imageL[0] = imageLr;
                imageR[0] = imageRr;
            }
        }

        //Setting StereoBM Parameters
        stereoSetparams(&this->calib.roi1,&this->calib.roi2,this->bm,imageL[0].rows,showStereoParamValues);

        // Convert BGR to Gray_Scale
        cvtColor(imageL[0],imageL_grey[0],CV_BGR2GRAY);
        cvtColor(imageR[0],imageR_grey[0],CV_BGR2GRAY);

        Mat disp;
        //Mat disp     = Mat(imageR[0].rows, imageR[0].cols, CV_16UC1);
        Mat disp_8U  = Mat(imageR[0].rows, imageR[0].cols, CV_8UC1);
        Mat disp_BGR = Mat(imageR[0].rows, imageR[0].cols, CV_8UC3);

        this->bm->compute(imageL_grey[0],imageR_grey[0],disp);
        //fillOcclusion(disp,16,false);

        normalize(disp, disp_8U, 0, 255, CV_MINMAX, CV_8U);
        applyColorMap(disp_8U,disp_BGR, COLORMAP_JET);

        /* Image Processing */
        Mat disp_8Median,disp_8MedianBGR;
        Mat disp_8eroded;Mat disp_8_eroded_dilated;

        //imageProcessing1(disp8,disp8Median,disp8Median);
        //imageProcessing2(disp8,disp8eroded,disp8_eroded_dilated,imageL[0],true);

        //(8) Projecting 3D point cloud to image
        if(show3Dreconstruction){
            Mat depth;
            cv::reprojectImageTo3D(disp,depth,this->calib.Q);
            Mat xyz= depth.reshape(3,depth.size().area());

            lookat(this->view3D.viewpoint, this->view3D.lookatpoint , this->view3D.Rotation);
            this->view3D.t.at<double>(0,0)=this->view3D.viewpoint.x;
            this->view3D.t.at<double>(1,0)=this->view3D.viewpoint.y;
            this->view3D.t.at<double>(2,0)=this->view3D.viewpoint.z;

            if(showXYZ){
                //cout<<t<<endl;
                cout << "x: " << this->view3D.t.at<double>(0,0) << endl;
                cout << "y: " << this->view3D.t.at<double>(1,0) << endl;
                cout << "z: " << this->view3D.t.at<double>(2,0) << endl;
            }

            this->view3D.t=this->view3D.Rotation*this->view3D.t;

            //projectImagefromXYZ(imageL[0],disp3Dviewer,disp,disp3D,xyz,Rotation,t,K,dist,isSub);
            projectImagefromXYZ(disp_BGR,this->view3D.disp3D_BGR,disp,this->view3D.disp3D,xyz,this->view3D.Rotation,this->view3D.t,this->calib.K,this->view3D.dist,this->view3D.isSub);

            // GUI Output
            this->view3D.disp3D.convertTo(this->view3D.disp3D_8U,CV_8U,0.5);
            //imshow("3D Depth",disp3D);
            //imshow("3D Viewer",disp3Dviewer);
            //imshow("3D Depth RGB",disp3DBGR);

            cv::cvtColor(this->view3D.disp3D_BGR,this->view3D.disp3D_BGR,CV_BGR2RGB);

            QImage qimageL((uchar*)this->view3D.disp3D_8U.data,this->view3D.disp3D_8U.cols,this->view3D.disp3D_8U.rows,this->view3D.disp3D_8U.step,QImage::Format_Indexed8);
            QImage qimageR((uchar*)this->view3D.disp3D_BGR.data,this->view3D.disp3D_BGR.cols,this->view3D.disp3D_BGR.rows,this->view3D.disp3D_BGR.step,QImage::Format_RGB888);

            ui->lblOriginalLeft->setPixmap(QPixmap::fromImage(qimageL));
            ui->lblOriginalRight->setPixmap(QPixmap::fromImage(qimageR));
        }
        else{
            destroyWindow("3D Depth");
            destroyWindow("3D Viewer");
            destroyWindow("3D Depth RGB");
        }

        //(9)Movement Difference between Frames
        Mat thresholdImage;
        if(StartDiff && showDiffImage){
            absdiff(imageL_grey[0],imageL_grey[1],diffImage);
            threshold(diffImage, thresholdImage, THRESH_VALUE, 255,THRESH_BINARY);
        }

        //Saving Previous Frame
        imageL[0].copyTo(imageL[1]);
        imageR[0].copyTo(imageR[1]);
        imageL_grey[0].copyTo(imageL_grey[1]);
        imageR_grey[0].copyTo(imageR_grey[1]);

        //imshow("Previous Left",imageL[1]);
        //imshow("Previous Right",imageR[1]);

        StartDiff=1;

        //(10)OpenCV and GUI Output
        if(showInputImages){
            //  imshow("Left",imageL[0]);
            //  imshow("Right",imageR[0]);

            if(isVideoFile){
                cv::cvtColor(imageL[0],imageL[0],CV_BGR2RGB);
                cv::cvtColor(imageR[0],imageR[0],CV_BGR2RGB);
            }

            if(!isVideoFile && isBGR2RGBflipped == false){
                cv::cvtColor(imageL[0],imageL[0],CV_BGR2RGB);
                cv::cvtColor(imageR[0],imageR[0],CV_BGR2RGB);
                bool isBGR2RGBflipped = true;
            }
            QImage qimageL((uchar*)imageL[0].data,imageL[0].cols,imageL[0].rows,imageL[0].step,QImage::Format_RGB888);
            QImage qimageR((uchar*)imageR[0].data,imageR[0].cols,imageR[0].rows,imageR[0].step,QImage::Format_RGB888);

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
            Mat disp_RGB;
            cv::cvtColor(disp_BGR,disp_RGB,CV_BGR2RGB);

            QImage qimageL((uchar*)disp_8U.data,disp_8U.cols,disp_8U.rows,disp_8U.step,QImage::Format_Indexed8);
            QImage qimageR((uchar*)disp_RGB.data,disp_RGB.cols,disp_RGB.rows,disp_RGB.step,QImage::Format_RGB888);

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


        //(11)Shortcuts
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
            this->view3D.isSub=this->view3D.isSub?false:true;
        if(key=='h')
            this->view3D.viewpoint.x+=this->view3D.step;
        if(key=='g')
            this->view3D.viewpoint.x-=this->view3D.step;
        if(key=='l')
            this->view3D.viewpoint.y+=this->view3D.step;
        if(key=='k')
            this->view3D.viewpoint.y-=this->view3D.step;
        if(key=='n')
            this->view3D.viewpoint.z+=this->view3D.step;
        if(key=='m')
            this->view3D.viewpoint.z-=this->view3D.step;

        if(key=='q')
            break;

        //(12)Video Loop - If the last frame is reached, reset the capture and the frameCounter
        frameCounter += 1;

        if(frameCounter == capR.get(CV_CAP_PROP_FRAME_COUNT)){
            frameCounter = 0;
            capL.set(CV_CAP_PROP_POS_FRAMES,0);
            capR.set(CV_CAP_PROP_POS_FRAMES,0);
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

void MainWindow::openStereoSource(int inputNum,VideoCapture* capL,VideoCapture* capR,Mat* imageL,Mat* imageR){
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

        capL->open(imageL_filename);
        capR->open(imageR_filename);

        if(!capL->isOpened() || !capR->isOpened()){		// Check if we succeeded
            ui->txtOutputBox->appendPlainText(QString( "Could not open or find the input videos!"));
            //return -1;
        }

        ui->txtOutputBox->appendPlainText(QString("Input 1 Resolution: ") + QString::number(capL->get(CV_CAP_PROP_FRAME_WIDTH)) + QString("x") + QString::number(capL->get(CV_CAP_PROP_FRAME_HEIGHT)));
        ui->txtOutputBox->appendPlainText(QString("Input 2 Resolution: ") + QString::number(capR->get(CV_CAP_PROP_FRAME_WIDTH)) + QString("x") + QString::number(capR->get(CV_CAP_PROP_FRAME_HEIGHT)));
    }else{
        ui->txtOutputBox->appendPlainText(QString( "It is not a Video file"));
        if(imageL_filename.substr(imageL_filename.find_last_of(".") + 1) == "jpg" || imageL_filename.substr(imageL_filename.find_last_of(".") + 1) == "png"){
            ui->txtOutputBox->appendPlainText(QString( "It's a Image file"));
            isImageFile=true;

            imageL[0] = imread(imageL_filename, CV_LOAD_IMAGE_COLOR);	// Read the file
            imageR[0] = imread(imageR_filename, CV_LOAD_IMAGE_COLOR);	// Read the file

            if(!imageL[0].data || !imageR[0].data){                     // Check for invalid input
                ui->txtOutputBox->appendPlainText(QString("Could not open or find the input images!"));
                return;
            }
        }else{
            ui->txtOutputBox->appendPlainText(QString( "It is not a Image file"));
        }
    }
}

bool createTrackbars(){ //Create Window for trackbars
    char TrackbarName[50];

    // Create TrackBars Window
    namedWindow(trackbarWindowName,0);

    // Create memory to store Trackbar name on window
    sprintf( TrackbarName, "preFilterSize");
    sprintf( TrackbarName, "preFilterCap");
    sprintf( TrackbarName, "SADWindowSize");
    sprintf( TrackbarName, "minDisparity");
    sprintf( TrackbarName, "numberOfDisparities");
    sprintf( TrackbarName, "textureThreshold");
    sprintf( TrackbarName, "uniquenessRatio");
    sprintf( TrackbarName, "speckleWindowSize");
    sprintf( TrackbarName, "speckleRange");
    sprintf( TrackbarName, "disp12MaxDiff");

    //Create Trackbars and insert them into window
    createTrackbar( "preFilterSize", trackbarWindowName, &preFilterSize, preFilterSize_MAX, on_trackbar );
    createTrackbar( "preFilterCap", trackbarWindowName, &preFilterCap, preFilterCap_MAX, on_trackbar );
    createTrackbar( "SADWindowSize", trackbarWindowName, &SADWindowSize, SADWindowSize_MAX, on_trackbar );
    createTrackbar( "minDisparity", trackbarWindowName, &minDisparity, minDisparity_MAX, on_trackbar );
    createTrackbar( "numberOfDisparities", trackbarWindowName, &numberOfDisparities, numberOfDisparities_MAX, on_trackbar );
    createTrackbar( "textureThreshold", trackbarWindowName, &textureThreshold, textureThreshold_MAX, on_trackbar );
    createTrackbar( "uniquenessRatio", trackbarWindowName, &uniquenessRatio, uniquenessRatio_MAX, on_trackbar );
    createTrackbar( "speckleWindowSize", trackbarWindowName, &speckleWindowSize, speckleWindowSize_MAX, on_trackbar );
    createTrackbar( "speckleRange", trackbarWindowName, &speckleRange, speckleRange_MAX, on_trackbar );
    createTrackbar( "disp12MaxDiff", trackbarWindowName, &disp12MaxDiff, disp12MaxDiff_MAX, on_trackbar );

    return(true);
}

void on_trackbar( int, void* ){}; //This function gets called whenever a trackbar position is changed

/*** Stereo Parameters Configuration function
  ** Description: Executes the setup of parameters of the StereoBM object by changing the trackbars
  ** @param rect roi1: Region of Interest 1
  ** @param rect roi2: Region of Interest 2
  ** @param StereoBM bm: Correspondence Object
  ** @param int numRows: Number of Rows of the input Images
  ** @param bool showStereoBMparams
  ** Returns:     Nothing
  ***/
void stereoSetparams(Rect* roi1,Rect* roi2,StereoBM* bm,int numRows,bool showStereoParamsValues){
    int trackbarsAux[10];

    trackbarsAux[0]= getTrackbarPos("preFilterSize",trackbarWindowName)*2.5+5;
    trackbarsAux[1]= getTrackbarPos("preFilterCap",trackbarWindowName)*0.625+1;
    trackbarsAux[2]= getTrackbarPos("SADWindowSize",trackbarWindowName)*2.5+5;
    trackbarsAux[3]= getTrackbarPos("minDisparity",trackbarWindowName)*2.0-100;
    trackbarsAux[4]= getTrackbarPos("numberOfDisparities",trackbarWindowName)*16;
    trackbarsAux[5]= getTrackbarPos("textureThreshold",trackbarWindowName)*320;
    trackbarsAux[6]= getTrackbarPos("uniquenessRatio",trackbarWindowName)*2.555;
    trackbarsAux[7]= getTrackbarPos("speckleWindowSize",trackbarWindowName)*1.0;
    trackbarsAux[8]= getTrackbarPos("speckleRange",trackbarWindowName)*1.0;
    trackbarsAux[9]= getTrackbarPos("disp12MaxDiff",trackbarWindowName)*1.0;

    bm->setROI1(*roi1);
    bm->setROI2(*roi2);

    if(trackbarsAux[0]%2==1 && trackbarsAux[0]>=5 && trackbarsAux[0]<=255){
        //bm.state->preFilterSize = trackbarsAux[0];
        bm->setPreFilterSize(trackbarsAux[0]);
    }

    if(trackbarsAux[1]>=1 && trackbarsAux[1]<=63){
        //bm.state->preFilterCap = trackbarsAux[1];
        bm->setPreFilterCap(trackbarsAux[1]);
    }

    if(trackbarsAux[2]%2==1 && trackbarsAux[2]>=5  && trackbarsAux[2]<=255 && trackbarsAux[2]<=numRows){
        //bm.state->SADWindowSize = trackbarsAux[2];
        bm->setBlockSize(trackbarsAux[2]);
    }

    if(trackbarsAux[3]>=-100 && trackbarsAux[3]<=100){
        //bm.state->minDisparity = trackbarsAux[3];
        bm->setMinDisparity(trackbarsAux[3]);
    }

    if(trackbarsAux[4]%16==0 && trackbarsAux[4]>=16 && trackbarsAux[4]<=256){
        //bm.state->numberOfDisparities = trackbarsAux[4];
        bm->setNumDisparities(trackbarsAux[4]);
    }

    if(trackbarsAux[5]>=0 && trackbarsAux[5]<=32000){
        //bm.state->textureThreshold = trackbarsAux[5];
        bm->setTextureThreshold(trackbarsAux[5]);
    }

    if(trackbarsAux[6]>=0 && trackbarsAux[6]<=255){
        //bm.state->uniquenessRatio = trackbarsAux[6];
        bm->setUniquenessRatio(trackbarsAux[6]);
    }

    if(trackbarsAux[7]>=0 && trackbarsAux[7]<=100){
        //bm.state->speckleWindowSize = trackbarsAux[7];
        bm->setSpeckleWindowSize(trackbarsAux[7]);
    }

    if(trackbarsAux[8]>=0 && trackbarsAux[8]<=100){
        //bm.state->speckleRange = trackbarsAux[8];
        bm->setSpeckleRange(trackbarsAux[8]);
    }

    if(trackbarsAux[9]>=0 && trackbarsAux[9]<=100){
        //bm.state->disp12MaxDiff = trackbarsAux[9];
        bm->setDisp12MaxDiff(trackbarsAux[9]);
    }

    if(showStereoParamsValues){
        cout << getTrackbarPos("preFilterSize",trackbarWindowName)			<< "\t" << trackbarsAux[0] << endl;
        cout << getTrackbarPos("preFilterCap",trackbarWindowName)			<< "\t" << trackbarsAux[1] << endl;
        cout << getTrackbarPos("SADWindowSize",trackbarWindowName)			<< "\t" << trackbarsAux[2] << endl;
        cout << getTrackbarPos("minDisparity",trackbarWindowName)			<< "\t" << trackbarsAux[3] << endl;
        cout << getTrackbarPos("numberOfDisparities",trackbarWindowName)	<< "\t" << trackbarsAux[4] << endl;
        cout << getTrackbarPos("textureThreshold",trackbarWindowName)		<< "\t" << trackbarsAux[5] << endl;
        cout << getTrackbarPos("uniquenessRatio",trackbarWindowName)		<< "\t" << trackbarsAux[6] << endl;
        cout << getTrackbarPos("speckleWindowSize",trackbarWindowName)		<< "\t" << trackbarsAux[7] << endl;
        cout << getTrackbarPos("speckleRange",trackbarWindowName)			<< "\t" << trackbarsAux[8] << endl;
        cout << getTrackbarPos("disp12MaxDiff",trackbarWindowName)			<< "\t" << trackbarsAux[9] << endl;
    }

}

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


//Image MainWindow::putImage(const Mat& mat)
//{
//    // 8-bits unsigned, NO. OF CHANNELS=1
//    if(mat.type()==CV_8UC1)
//    {
//        // Set the color table (used to translate colour indexes to qRgb values)
//        QVector<QRgb> colorTable;
//        for (int i=0; i<256; i++)
//            colorTable.push_back(qRgb(i,i,i));
//        // Copy input Mat
//        const uchar *qImageBuffer = (const uchar*)mat.data;
//        // Create QImage with same dimensions as input Mat
//        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
//        img.setColorTable(colorTable);
//        return img;
//    }
//    // 8-bits unsigned, NO. OF CHANNELS=3
//    if(mat.type()==CV_8UC3)
//    {
//        // Copy input Mat
//        const uchar *qImageBuffer = (const uchar*)mat.data;
//        // Create QImage with same dimensions as input Mat
//        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
//        return img.rgbSwapped();
//    }
//    else
//    {
//        qDebug() << "ERROR: Mat could not be converted to QImage.";
//        return QImage();
//    }
//}
