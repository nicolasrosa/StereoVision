// reprojectImageTo3D - BlockMatching Algorithm
// Nicolas Rosa, June 2015.
// Credits: http://opencv.jp/opencv2-x-samples/point-cloud-rendering

// Libraries
#include "reprojectImageTo3D.h"

//Qt Creator Libs
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>

using namespace cv;
using namespace std;

<<<<<<< HEAD
MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow){
=======
    MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow){
>>>>>>> 9661ca4503c204694344162c384e144f40ca3a85
    ui->setupUi(this);

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
    showStereoParam = !showStereoParam;
}

<<<<<<< HEAD
void MainWindow::on_btnShowInputImages_clicked(){
    showInputImages = true;
    showDisparityMap = false;
    show3Dreconstruction = false;
}

void MainWindow::on_btnShowDisparityMap_clicked(){
    showInputImages = false;
    showDisparityMap = true;
    show3Dreconstruction = false;
=======
void MainWindow::on_btnShowStereoParamSetup_clicked(){
    showStereoParam = !showStereoParam;
>>>>>>> 9661ca4503c204694344162c384e144f40ca3a85
}

void MainWindow::on_btnShow3DReconstruction_clicked(){
    showInputImages = false;
    showDisparityMap = false;
    show3Dreconstruction = true;
}

void MainWindow::StereoVisionProcessAndUpdateGUI(){
    int frameCounter=0;
    float fps,lastTime = clock();
    bool StartDiff=false;
    char key=0;

    ConfigFile cfg;
    Mat diffImage;
    Mat disp3Dviewer,disp3D,disp83D,disp3DBGR;

    printHelp();

    //(1) Open Image Source
    int inputNum=0;
    //    Mat imageL[2],imageR[2];
    //    Mat	imageL_grey[2],imageR_grey[2];
    //    VideoCapture capL,capR;

    //ImageProcessor test(0.1);

    //openImageSource(inputNum,&capL,&capR,&imageL[0],&imageR[0]);
    openImageSource(6,&capL,&capR,&imageL[0],&imageR[0]);
    cfg.readConfigFile(&cfg);

    //(2) Stereo Initialization
    Ptr<StereoBM> bm = StereoBM::create(16,9);

    stereoInit(bm);

    //(3) Stereo Calibration
    Mat M1,D1,M2,D2;
    Mat R,T,R1,P1,R2,P2;
    Rect roi1, roi2;

    if(needCalibration){
        cout << "Calibration: ON\n" << endl;
        stereoCalib(M1,D1,M2,D2,R,T,&cfg);
    }else{
        cout << "Calibration: OFF\n" << endl;
    }

    //(4) Compute the Q Matrix
    Mat Q;
    double focalLength,baseline;

    readQMatrix(Q,&focalLength,&baseline,&cfg);

    //Point2d imageCenter = Point2d((imageL[0].cols-1.0)/2.0,(imageL[0].rows-1.0)/2.0);
    //calculateQMatrix(Q,imageCenter,focalLength,baseline*16);


    //(5) Camera Setting
    Mat K=Mat::eye(3,3,CV_64F);
    K.at<double>(0,0)=focalLength;
    K.at<double>(1,1)=focalLength;
    K.at<double>(0,2)=(imageL[0].cols-1.0)/2.0;
    K.at<double>(1,2)=(imageL[0].rows-1.0)/2.0;
    //cout << "K:" << endl << K << endl;

    //(6) Point Cloud Initialization
    Mat dist=Mat::zeros(5,1,CV_64F);
    Mat Rotation=Mat::eye(3,3,CV_64F);
    Mat t=Mat::zeros(3,1,CV_64F);

    //Point3d viewpoint(0.0,0.0,5);
    //Point3d lookatpoint(0.0,0.0,baseline*10.0);
    Point3d viewpoint(20.0,20.0,-baseline*10);
    Point3d lookatpoint(22.0,16.0,baseline*10.0);
    const double step=baseline/10;
    bool isSub=true;

    isStereoParamSetupTrackbarsCreated=createTrackbars();
    //createTrackbars();

    //(7) Rendering Loop
    while(key!='q'){
        if(isVideoFile){
            capL >> imageL[0];
            capR >> imageR[0];

            resizeFrame(&imageL[0],&imageR[0]);

            if(needCalibration){
                Size imageSize = imageL[0].size();
                stereoRectify(M1,D1,M2,D2,imageSize,R,T,R1,R2,P1,P2,Q,CALIB_ZERO_DISPARITY,-1,imageSize,&roi1,&roi2);
                Mat rmap[2][2];
                initUndistortRectifyMap(M1, D1, R1, P1, imageSize, CV_16SC2, rmap[0][0], rmap[0][1]);
                initUndistortRectifyMap(M2, D2, R2, P2, imageSize, CV_16SC2, rmap[1][0], rmap[1][1]);
                Mat imageLr, imageRr;
                remap(imageL[0], imageLr, rmap[0][0], rmap[0][1], INTER_LINEAR);
                remap(imageR[0], imageRr, rmap[1][0], rmap[1][1], INTER_LINEAR);

                imageL[0] = imageLr;
                imageR[0] = imageRr;
            }
        }

        //Setting StereoBM Parameters
        stereoSetparams(&roi1,&roi2,bm,imageL[0].rows,showStereoParamValues);

        // Convert BGR to Gray_Scale
        cvtColor(imageL[0],imageL_grey[0],CV_BGR2GRAY);
        cvtColor(imageR[0],imageR_grey[0],CV_BGR2GRAY);

        Mat disp;
        Mat disp8  = Mat(imageR[0].rows, imageR[0].cols, CV_8UC1 );
        Mat disp8BGR;

        bm->compute(imageL_grey[0],imageR_grey[0],disp);
        //fillOcclusion(disp,16,false);

        normalize(disp, disp8, 0, 255, CV_MINMAX, CV_8U);
        applyColorMap(disp8,disp8BGR, COLORMAP_JET);

        /* Image Processing */
        Mat disp8Median,disp8MedianBGR;
        Mat disp8eroded;Mat disp8_eroded_dilated;

        //imageProcessing1(disp8,disp8Median,disp8Median);
        //imageProcessing2(disp8,disp8eroded,disp8_eroded_dilated);


        //(8) Projecting 3D point cloud to image
        if(show3Dreconstruction){
            Mat depth;
            cv::reprojectImageTo3D(disp,depth,Q);
            Mat xyz= depth.reshape(3,depth.size().area());

            lookat(viewpoint, lookatpoint , Rotation);
            t.at<double>(0,0)=viewpoint.x;
            t.at<double>(1,0)=viewpoint.y;
            t.at<double>(2,0)=viewpoint.z;

            if(showXYZ){
                //cout<<t<<endl;
                cout << "x: " << t.at<double>(0,0) << endl;
                cout << "y: " << t.at<double>(1,0) << endl;
                cout << "z: " << t.at<double>(2,0) << endl;
            }

            t=Rotation*t;

            //projectImagefromXYZ(imageL[0],disp3Dviewer,disp,disp3D,xyz,Rotation,t,K,dist,isSub);
            projectImagefromXYZ(disp8BGR,disp3DBGR,disp,disp3D,xyz,Rotation,t,K,dist,isSub);

            // GUI Output
            disp3D.convertTo(disp83D,CV_8U,0.5);
            //imshow("3D Depth",disp3D);
            //imshow("3D Viewer",disp3Dviewer);
            //imshow("3D Depth RGB",disp3DBGR);

            cv::cvtColor(disp3DBGR,disp3DBGR,CV_BGR2RGB);

            QImage qimageL((uchar*)disp83D.data,disp83D.cols,disp83D.rows,disp83D.step,QImage::Format_Indexed8);
            QImage qimageR((uchar*)disp3DBGR.data,disp3DBGR.cols,disp3DBGR.rows,disp3DBGR.step,QImage::Format_RGB888);

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

            cv::cvtColor(imageL[0],imageL[0],CV_BGR2RGB);
            cv::cvtColor(imageR[0],imageR[0],CV_BGR2RGB);

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
            //imshow("Disparity Map",disp8);
            //imshow("Disparity Map BGR",disp8BGR);

            cv::cvtColor(disp8BGR,disp8BGR,CV_BGR2RGB);

            QImage qimageL((uchar*)disp8.data,disp8.cols,disp8.rows,disp8.step,QImage::Format_Indexed8);
            QImage qimageR((uchar*)disp8BGR.data,disp8BGR.cols,disp8BGR.rows,disp8BGR.step,QImage::Format_RGB888);

            ui->lblOriginalLeft->setPixmap(QPixmap::fromImage(qimageL));
            ui->lblOriginalRight->setPixmap(QPixmap::fromImage(qimageR));
        }
        else{
            destroyWindow("Disparity Map");
            destroyWindow("Disparity Map BGR");
        }

<<<<<<< HEAD
        //       // if(showStereoParam && !isStereoParamSetupTrackbarsCreated){
        //       if(showStereoParam && !isStereoParamSetupTrackbarsCreated){
        //           isStereoParamSetupTrackbarsCreated=true;
        //           createTrackbars();
        //            cout << "oi" << endl;
        //        }else{
        //            destroyWindow(trackbarWindowName);
        //            isStereoParamSetupTrackbarsCreated=false;
        //        }
=======
//       // if(showStereoParam && !isStereoParamSetupTrackbarsCreated){
//       if(showStereoParam && !isStereoParamSetupTrackbarsCreated){
//           isStereoParamSetupTrackbarsCreated=true;
//           createTrackbars();
//            cout << "oi" << endl;
//        }else{
//            destroyWindow(trackbarWindowName);
//            isStereoParamSetupTrackbarsCreated=false;
//        }
>>>>>>> 9661ca4503c204694344162c384e144f40ca3a85

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
            isSub=isSub?false:true;
        if(key=='h')
            viewpoint.x+=step;
        if(key=='g')
            viewpoint.x-=step;
        if(key=='l')
            viewpoint.y+=step;
        if(key=='k')
            viewpoint.y-=step;
        if(key=='n')
            viewpoint.z+=step;
        if(key=='m')
            viewpoint.z-=step;

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

<<<<<<< HEAD
void MainWindow::printHelp(){
    ui->txtOutputBox->appendPlainText(QString("-----------------Help Menu-----------------\n")+
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
=======

void printHelp(){
    std::cout << "\n\n-----------------Help Menu-----------------\n"
              << "Run command: ./reprojectImageTo3D\n"
              << "Keys:\n"
              << "'`' -\tShow Help\n"
              << "'1' -\tShow L/R Windows\n"
              << "'2' -\tShow Disparity Map\n"
              << "'3' -\tShow 3D Reconstruction\n"
              << "'4' -\tShow XYZ\n"
              << "'5' -\tShow FPS\n"
              << "'6' -\tShow Stereo Parameters\n"
              << "\n3D Viewer Navigation:\n"
              << "x-axis:\t'g'/'h' -> +x,-x\n"
              << "y-axis:\t'l'/'k' -> +y,-y\n"
              << "z-axis:\t'n'/'m' -> +z,-z\n"
              << "-------------------------------------------\n"
              << "\n\n";
>>>>>>> 9661ca4503c204694344162c384e144f40ca3a85
}

void openImageSource(int inputNum,VideoCapture* capL,VideoCapture* capR,Mat* imageL,Mat* imageR){
    std::string imageL_filename;
    std::string imageR_filename;

    // Create an object that decodes the input Video stream.
    cout << "Enter Video Number(1,2,3,4,5,6,7,8,9): " << inputNum << endl;
    //	scanf("%d",&inputNum);
    cout << "Input File:";
    switch(inputNum){
    case 1:
        imageL_filename = "../data/left/video2_denoised_long.avi";
        imageR_filename = "../data/right/video2_denoised_long.avi";
        needCalibration=true;
        cout << "video2_denoised_long.avi" << endl;
        break;
    case 2:
        imageL_filename = "../data/left/video0.avi";
        imageR_filename = "../data/right/video0.avi";
        needCalibration=false;
        cout << "video0.avi" << endl;
        break;
    case 3:
        imageL_filename = "../data/left/video1.avi";
        imageR_filename = "../data/right/video1.avi";
        needCalibration=true;
        cout << "video1.avi" << endl;
        break;
    case 4:
        imageL_filename = "../data/left/video2_noised.avi";
        imageR_filename = "../data/right/video2_noised.avi";
        needCalibration=true;
        cout << "video2_noised.avi" << endl;
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
        imageL_filename = "../data/left/left1.png";
        imageR_filename = "../data/right/right1.png";
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
        cout << "It's a Video file" << endl;
        isVideoFile=true;

        capL->open(imageL_filename);
        capR->open(imageR_filename);

        if(!capL->isOpened() || !capR->isOpened()){		// Check if we succeeded
            cout <<  "Could not open or find the input videos!" << std::endl ;
            //return -1;
        }

        cout << "Input 1 Resolution: " << capR->get(CV_CAP_PROP_FRAME_WIDTH) << "x" << capR->get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
        cout << "Input 2 Resolution: " << capL->get(CV_CAP_PROP_FRAME_WIDTH) << "x" << capL->get(CV_CAP_PROP_FRAME_HEIGHT) << endl << endl;
    }else{
        cout << "It is not a Video file" << endl;
        if(imageL_filename.substr(imageL_filename.find_last_of(".") + 1) == "jpg" || imageL_filename.substr(imageL_filename.find_last_of(".") + 1) == "png"){
            cout << "It's a Image file" << endl;
            isImageFile=true;

            imageL[0] = imread(imageL_filename, CV_LOAD_IMAGE_COLOR);	// Read the file
            imageR[0] = imread(imageR_filename, CV_LOAD_IMAGE_COLOR);	// Read the file

            if(!imageL[0].data || !imageR[0].data){                          	// Check for invalid input
                cout <<  "Could not open or find the input images!" << std::endl;
                //return -1;
            }
        }else{
            cout << "It is not a Image file" << endl;
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

/*** Stereo Initialization function
  ** Description: Executes the PreSetup of parameters of the StereoBM object
  ** @param StereoBM bm: Correspondence Object
  ** Returns:     Nothing
  ***/
void stereoInit(StereoBM* bm){
    bm->setPreFilterCap(31);
    bm->setBlockSize(SADWindowSize > 0 ? SADWindowSize : 9);
    bm->setMinDisparity(0);
    bm->setNumDisparities(numberOfDisparities);
    bm->setTextureThreshold(10);
    bm->setUniquenessRatio(15);
    bm->setSpeckleWindowSize(100);
    bm->setSpeckleRange(32);
    bm->setDisp12MaxDiff(1);
}

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

/*** Stereo Calibration function
  ** Description: Reads the Calibrations in *.yml files
  ** Receives:    Matrices Addresses for storage
  ** @param Mat M1,M2: Intrinsic Matrices from camera 1 and 2
  ** @param Mat D1,D2: Distortion Coefficients from camera 1 and 2
  ** @param Mat R: Rotation Matrix
  ** @param Mat t: Translation Vector
  ** Returns:     Nothing
  ***/
void stereoCalib(Mat &M1,Mat &D1,Mat &M2,Mat &D2,Mat &R,Mat &T,ConfigFile* cfg){
    //FileStorage fs("../data/calib/calib5_640_480/intrinsics.yml", FileStorage::READ);
    FileStorage fs(cfg->intrinsicsFileName, FileStorage::READ);
    if(!fs.isOpened()){
        printf("Failed to open intrinsics.yml file\n");
        return;
    }

    fs["M1"] >> M1;
    fs["D1"] >> D1;
    fs["M2"] >> M2;
    fs["D2"] >> D2;

    fs.release();

    float scale = 1.f;
    M1 *= scale;
    M2 *= scale;

    //fs.open("../data/calib/calib5_640_480/extrinsics.yml", FileStorage::READ);
    fs.open(cfg->extrinsicsFileName, FileStorage::READ);
    if(!fs.isOpened()){
        printf("Failed to open extrinsics.yml file\n");
        return;
    }

    fs["R"] >> R;
    fs["T"] >> T;

    fs.release();

    cout << "Intrinsics: " << endl;
    cout << "M1: " << endl << M1 << endl;
    cout << "D1: " << endl << D1 << endl;
    cout << "M2: " << endl << M2 << endl;
    cout << "D2: " << endl << D2 << endl;

    cout << "\nExtrinsics: "   << endl;
    cout << "R: " << endl << R << endl;
    cout << "T: " << endl << T << endl << endl;
}

void resizeFrame(Mat* frame1,Mat* frame2){
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

void imageProcessing2(Mat src, Mat imgE, Mat imgED){
    Mat element = getStructuringElement( MORPH_RECT,Size( 2*EROSION_SIZE + 1, 2*EROSION_SIZE+1 ),Point( EROSION_SIZE, EROSION_SIZE ) );
    Mat imgEBGR,imgEDBGR;
    Mat imgEDMedian,imgEDMedianBGR;
    Mat imgThresholded;

    // Erode and Dilate to take out spurious noise
    // Apply Erosion and Dilation
    erode(src,imgE,element);
    dilate(imgE,imgED,element);

    applyColorMap(imgE,imgEBGR, COLORMAP_JET);
    applyColorMap(imgED,imgEDBGR, COLORMAP_JET);

    // Apply Median Filter
    GaussianBlur(imgED,imgEDMedian,Size(3,3),0,0);
    //medianBlur(imgED,imgEDMedian,5);
    applyColorMap(imgEDMedian,imgEDMedianBGR, COLORMAP_JET);

    // Thresholding
    threshold( imgEDMedian, imgThresholded, 128, 255,0);


    // Output
    imshow("Eroded Image",imgE);
    imshow("Eroded Image BGR",imgEBGR);

    imshow("Eroded+Dilated Image",imgED);
    imshow("Eroded+Dilated Image BGR",imgEDBGR);

    imshow("Eroded+Dilated+Median Image",imgEDMedian);
    imshow("Eroded+Dilated+Median Image BGR",imgEDMedianBGR);

    imshow("Thresholded Image",imgThresholded);
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

/*** Read Q Matrix function
  ** Description: Reads the Q Matrix in the *.yml file
  ** Receives:    Matrices Addresses for storage
  ** Returns:     Nothing
  **
  ** Perspective transformation matrix(Q)
  ** [ 1  0    0	   -cx     ]
  ** [ 0  1    0 	   -cy     ]
  ** [ 0  0    0		f      ]
  ** [ 0  0  -1/Tx 	(cx-cx')/Tx]
  ***/
void readQMatrix(Mat &Q,double* focalLength,double* baseline,ConfigFile* cfg){
<<<<<<< HEAD
#ifdef RESOLUTION_640x480
    //FileStorage fs("../data/calib/calib5_640_480/Q.yml", FileStorage::READ);
    FileStorage fs(cfg->QmatrixFileName, FileStorage::READ);
#endif
=======
    #ifdef RESOLUTION_640x480
        //FileStorage fs("../data/calib/calib5_640_480/Q.yml", FileStorage::READ);
        FileStorage fs(cfg->QmatrixFileName, FileStorage::READ);
    #endif
>>>>>>> 9661ca4503c204694344162c384e144f40ca3a85

    if(!fs.isOpened()){
        printf("Failed to open Q.yml file\n");
        return;
    }

    fs["Q"] >> Q;
    cout << "Q:" << endl << Q << endl;

    *focalLength = Q.at<double>(2,3);  cout << "f:" << *focalLength << endl;
    *baseline = -1.0/Q.at<double>(3,2); cout << "baseline: " << *baseline << endl;
}

void eular2rot(double yaw,double pitch, double roll,Mat& dest){
    double theta = yaw/180.0*CV_PI;
    double pusai = pitch/180.0*CV_PI;
    double phi = roll/180.0*CV_PI;

    double datax[3][3] = {{1.0,0.0,0.0},{0.0,cos(theta),-sin(theta)},{0.0,sin(theta),cos(theta)}};
    double datay[3][3] = {{cos(pusai),0.0,sin(pusai)},{0.0,1.0,0.0},{-sin(pusai),0.0,cos(pusai)}};
    double dataz[3][3] = {{cos(phi),-sin(phi),0.0},{sin(phi),cos(phi),0.0},{0.0,0.0,1.0}};
    Mat Rx(3,3,CV_64F,datax);
    Mat Ry(3,3,CV_64F,datay);
    Mat Rz(3,3,CV_64F,dataz);
    Mat rr=Rz*Rx*Ry;
    rr.copyTo(dest);
}

void lookat(Point3d from, Point3d to, Mat& destR){
    double x=(to.x-from.x);
    double y=(to.y-from.y);
    double z=(to.z-from.z);

    double pitch =asin(x/sqrt(x*x+z*z))/CV_PI*180.0;
    double yaw =asin(-y/sqrt(y*y+z*z))/CV_PI*180.0;

    eular2rot(yaw, pitch, 0,destR);
}

void projectImagefromXYZ(Mat &image, Mat &destimage, Mat &disp, Mat &destdisp, Mat &xyz, Mat &R, Mat &t, Mat &K, Mat &dist, bool isSub){
    Mat mask;
    if(mask.empty())mask=Mat::zeros(image.size(),CV_8U);
    if(disp.type()==CV_8U){
        projectImagefromXYZ_<unsigned char>(image,destimage, disp, destdisp, xyz, R, t, K, dist, mask,isSub);
    }
    else if(disp.type()==CV_16S){
        projectImagefromXYZ_<short>(image,destimage, disp, destdisp, xyz, R, t, K, dist, mask,isSub);
    }
    else if(disp.type()==CV_16U){
        projectImagefromXYZ_<unsigned short>(image,destimage, disp, destdisp, xyz, R, t, K, dist, mask,isSub);
    }
    else if(disp.type()==CV_32F){
        projectImagefromXYZ_<float>(image,destimage, disp, destdisp, xyz, R, t, K, dist, mask,isSub);
    }
    else if(disp.type()==CV_64F){
        projectImagefromXYZ_<double>(image,destimage, disp, destdisp, xyz, R, t, K, dist, mask,isSub);
    }
}

template <class T>
static void fillOcclusionInv_(Mat& src, T invalidvalue){
    int bb=1;
    const int MAX_LENGTH=src.cols*0.8;
    //#pragma omp parallel for
    for(int j=bb;j<src.rows-bb;j++){
        T* s = src.ptr<T>(j);
        //const T st = s[0];
        //const T ed = s[src.cols-1];
        s[0]=0;
        s[src.cols-1]=0;
        for(int i=0;i<src.cols;i++){
            if(s[i]==invalidvalue){
                int t=i;
                do{
                    t++;
                    if(t>src.cols-1)break;
                }while(s[t]==invalidvalue);

                const T dd = max(s[i-1],s[t]);
                if(t-i>MAX_LENGTH){
                    for(int n=0;n<src.cols;n++){
                        s[n]=invalidvalue;
                    }
                }
                else{
                    for(;i<t;i++){
                        s[i]=dd;
                    }
                }
            }
        }
    }
}

template <class T>
static void projectImagefromXYZ_(Mat& image, Mat& destimage, Mat& disp, Mat& destdisp, Mat& xyz, Mat& R, Mat& t, Mat& K, Mat& dist, Mat& mask, bool isSub){
    if(destimage.empty())destimage=Mat::zeros(Size(image.size()),image.type());
    if(destdisp.empty())destdisp=Mat::zeros(Size(image.size()),disp.type());

    vector<Point2f> pt;
    if(dist.empty()) dist = Mat::zeros(Size(5,1),CV_32F);
    cv::projectPoints(xyz,R,t,K,dist,pt);
    destimage.setTo(0);
    destdisp.setTo(0);

    //#pragma omp parallel for
    for(int j=1;j<image.rows-1;j++){
        int count=j*image.cols;
        uchar* img=image.ptr<uchar>(j);
        uchar* m=mask.ptr<uchar>(j);
        for(int i=0;i<image.cols;i++,count++){
            int x=(int)(pt[count].x+0.5);
            int y=(int)(pt[count].y+0.5);
            if(m[i]==255)continue;
            if(pt[count].x>=1 && pt[count].x<image.cols-1 && pt[count].y>=1 && pt[count].y<image.rows-1){
                short v=destdisp.at<T>(y,x);
                if(v<disp.at<T>(j,i)){
                    destimage.at<uchar>(y,3*x+0)=img[3*i+0];
                    destimage.at<uchar>(y,3*x+1)=img[3*i+1];
                    destimage.at<uchar>(y,3*x+2)=img[3*i+2];
                    destdisp.at<T>(y,x)=disp.at<T>(j,i);

                    if(isSub){
                        if((int)pt[count+image.cols].y-y>1 && (int)pt[count+1].x-x>1){
                            destimage.at<uchar>(y,3*x+3)=img[3*i+0];
                            destimage.at<uchar>(y,3*x+4)=img[3*i+1];
                            destimage.at<uchar>(y,3*x+5)=img[3*i+2];

                            destimage.at<uchar>(y+1,3*x+0)=img[3*i+0];
                            destimage.at<uchar>(y+1,3*x+1)=img[3*i+1];
                            destimage.at<uchar>(y+1,3*x+2)=img[3*i+2];

                            destimage.at<uchar>(y+1,3*x+3)=img[3*i+0];
                            destimage.at<uchar>(y+1,3*x+4)=img[3*i+1];
                            destimage.at<uchar>(y+1,3*x+5)=img[3*i+2];

                            destdisp.at<T>(y,x+1)=disp.at<T>(j,i);
                            destdisp.at<T>(y+1,x)=disp.at<T>(j,i);
                            destdisp.at<T>(y+1,x+1)=disp.at<T>(j,i);
                        }
                        else if((int)pt[count-image.cols].y-y<-1 && (int)pt[count-1].x-x<-1){
                            destimage.at<uchar>(y,3*x-3)=img[3*i+0];
                            destimage.at<uchar>(y,3*x-2)=img[3*i+1];
                            destimage.at<uchar>(y,3*x-1)=img[3*i+2];

                            destimage.at<uchar>(y-1,3*x+0)=img[3*i+0];
                            destimage.at<uchar>(y-1,3*x+1)=img[3*i+1];
                            destimage.at<uchar>(y-1,3*x+2)=img[3*i+2];

                            destimage.at<uchar>(y-1,3*x-3)=img[3*i+0];
                            destimage.at<uchar>(y-1,3*x-2)=img[3*i+1];
                            destimage.at<uchar>(y-1,3*x-1)=img[3*i+2];

                            destdisp.at<T>(y,x-1)=disp.at<T>(j,i);
                            destdisp.at<T>(y-1,x)=disp.at<T>(j,i);
                            destdisp.at<T>(y-1,x-1)=disp.at<T>(j,i);
                        }
                        else if((int)pt[count+1].x-x>1){
                            destimage.at<uchar>(y,3*x+3)=img[3*i+0];
                            destimage.at<uchar>(y,3*x+4)=img[3*i+1];
                            destimage.at<uchar>(y,3*x+5)=img[3*i+2];

                            destdisp.at<T>(y,x+1)=disp.at<T>(j,i);
                        }
                        else if((int)pt[count-1].x-x<-1){
                            destimage.at<uchar>(y,3*x-3)=img[3*i+0];
                            destimage.at<uchar>(y,3*x-2)=img[3*i+1];
                            destimage.at<uchar>(y,3*x-1)=img[3*i+2];

                            destdisp.at<T>(y,x-1)=disp.at<T>(j,i);
                        }
                        else if((int)pt[count+image.cols].y-y>1){
                            destimage.at<uchar>(y+1,3*x+0)=img[3*i+0];
                            destimage.at<uchar>(y+1,3*x+1)=img[3*i+1];
                            destimage.at<uchar>(y+1,3*x+2)=img[3*i+2];

                            destdisp.at<T>(y+1,x)=disp.at<T>(j,i);
                        }
                        else if((int)pt[count-image.cols].y-y<-1){
                            destimage.at<uchar>(y-1,3*x+0)=img[3*i+0];
                            destimage.at<uchar>(y-1,3*x+1)=img[3*i+1];
                            destimage.at<uchar>(y-1,3*x+2)=img[3*i+2];

                            destdisp.at<T>(y-1,x)=disp.at<T>(j,i);
                        }
                    }
                }
            }
        }
    }

    if(isSub)
    {
        Mat image2;
        Mat disp2;
        destimage.copyTo(image2);
        destdisp.copyTo(disp2);
        const int BS=1;
        //#pragma omp parallel for
        for(int j=BS;j<image.rows-BS;j++){
            uchar* img=destimage.ptr<uchar>(j);
            T* m = disp2.ptr<T>(j);
            T* dp = destdisp.ptr<T>(j);
            for(int i=BS;i<image.cols-BS;i++){
                if(m[i]==0){
                    int count=0;
                    int d=0;
                    int r=0;
                    int g=0;
                    int b=0;
                    for(int l=-BS;l<=BS;l++){
                        T* dp2 = disp2.ptr<T>(j+l);
                        uchar* imageR = image2.ptr<uchar>(j+l);
                        for(int k=-BS;k<=BS;k++){
                            if(dp2[i+k]!=0){
                                count++;
                                d+=dp2[i+k];
                                r+=imageR[3*(i+k)+0];
                                g+=imageR[3*(i+k)+1];
                                b+=imageR[3*(i+k)+2];
                            }
                        }
                    }
                    if(count!=0){
                        double div = 1.0/count;
                        dp[i]=d*div;
                        img[3*i+0]=r*div;
                        img[3*i+1]=g*div;
                        img[3*i+2]=b*div;
                    }
                }
            }
        }
    }
}

void fillOcclusion(Mat& src, int invalidvalue, bool isInv){
    if(isInv){
        if(src.type()==CV_8U){
            fillOcclusionInv_<uchar>(src, (uchar)invalidvalue);
        }
        else if(src.type()==CV_16S){
            fillOcclusionInv_<short>(src, (short)invalidvalue);
        }
        else if(src.type()==CV_16U){
            fillOcclusionInv_<unsigned short>(src, (unsigned short)invalidvalue);
        }
        else if(src.type()==CV_32F){
            fillOcclusionInv_<float>(src, (float)invalidvalue);
        }
    }
    else{
        if(src.type()==CV_8U){
            fillOcclusion_<uchar>(src, (uchar)invalidvalue);
        }
        else if(src.type()==CV_16S){
            fillOcclusion_<short>(src, (short)invalidvalue);
        }
        else if(src.type()==CV_16U){
            fillOcclusion_<unsigned short>(src, (unsigned short)invalidvalue);
        }
        else if(src.type()==CV_32F){
            fillOcclusion_<float>(src, (float)invalidvalue);
        }
    }
}

template <class T>
static void fillOcclusion_(Mat& src, T invalidvalue){
    int bb=1;
    const int MAX_LENGTH=src.cols*0.5;
    //#pragma omp parallel for
    for(int j=bb;j<src.rows-bb;j++){
        T* s = src.ptr<T>(j);
        //const T st = s[0];
        //const T ed = s[src.cols-1];
        s[0]=255;
        s[src.cols-1]=255;
        for(int i=0;i<src.cols;i++){
            if(s[i]<=invalidvalue){
                int t=i;
                do{
                    t++;
                    if(t>src.cols-1)break;
                }while(s[t]<=invalidvalue);

                const T dd = min(s[i-1],s[t]);
                if(t-i>MAX_LENGTH){
                    for(int n=0;n<src.cols;n++){
                        s[n]=invalidvalue;
                    }
                }
                else{
                    for(;i<t;i++){
                        s[i]=dd;
                    }
                }
            }
        }
    }
}


//QImage MainWindow::putImage(const Mat& mat)
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
