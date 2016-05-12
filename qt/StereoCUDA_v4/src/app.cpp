#include "inc/app.h"

#include <iostream>

#define DISP_NORMALIZATION

using namespace std;

App::App(const Params& params) : p(params), running(false){
    isVideoFile=false;
    isImageFile=false;
    frameCounter=0;

#ifdef x64
    cuda::printShortCudaDeviceInfo(cuda::getDevice());
#endif

#ifdef jetsonTK1
    gpu::printShortCudaDeviceInfo(gpu::getDevice());
#endif

}

void App::printHelp(){
    cout << "stereo_match_gpu sample\n"
         << "\nControls:\n"
         << "\tesc - exit\n"
         << "\tp - print current parameters\n"
         << "\tg - convert source images into gray\n"
         << "\tm - change stereo match method\n"
         << "\ts - change Sobel prefiltering flag (for BM only)\n"
         << "\t1/q - increase/decrease maximum disparity\n"
         << "\t2/w - increase/decrease window size (for BM only)\n"
         << "\t3/e - increase/decrease iteration count (for BP and CSBP only)\n"
         << "\t4/r - increase/decrease level count (for BP and CSBP only)\n";
}

void App::run(){
    App::printHelp();
    App::open();
    App::init();
    App::loop();
}

void App::open(){
    /* Identify the type of the input file. */
    if(p.left.substr(p.left.find_last_of(".") + 1) == "avi"){
        isVideoFile=true;
        capL.open(p.left);
        capR.open(p.right);

        if(!capL.isOpened() || !capR.isOpened()){		// Check if it succeeded
            cerr <<  "Could not open or find the input videos!" << endl ;
            //return -1;
        }

        /* Console Output */
        cout << "It's a Video file" << endl;
        cout << "Input 1 Resolution: " << capR.get(CV_CAP_PROP_FRAME_WIDTH) << "x" << capR.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
        cout << "Input 2 Resolution: " << capL.get(CV_CAP_PROP_FRAME_WIDTH) << "x" << capL.get(CV_CAP_PROP_FRAME_HEIGHT) << endl << endl;

    }else{
        cout << "It is not a Video file" << endl;
        if(p.left.substr(p.left.find_last_of(".") + 1) == "jpg" || p.left.substr(p.left.find_last_of(".") + 1) == "png"){
            cout << "It's a Image file" << endl;
            isImageFile=true;

            imageL_src = imread(p.left);
            imageR_src = imread(p.right);

            //if (imageL_src.empty()) throw runtime_error("can't open file \"" + p.left + "\"");
            //if (imageR_src.empty()) throw runtime_error("can't open file \"" + p.right + "\"");

            if(!imageL.data || !imageR.data){      // Check if it succeeded
                cout << "Could not open or find the input images!" << endl;
                //return -1;
            }
        }else{
            cout << "It is not a Image file" << endl;
        }
    }
}

void App::init(){
    if(isVideoFile){

    }else{
        cvtColor(imageL_src, imageL, COLOR_BGR2GRAY);
        cvtColor(imageR_src, imageR, COLOR_BGR2GRAY);
        d_imageL.upload(imageL);
        d_imageR.upload(imageR);

        imshow("left", imageL);
        imshow("right", imageR);
    }

    /* Initializing Stereo Matching Methods - Set Common Parameters */
#ifdef x64
    stereoBM_GPU_Init();
    bm = cuda::createStereoBM(p.ndisp);

    bp = cuda::createStereoBeliefPropagation(p.ndisp);
    csbp = cuda::createStereoConstantSpaceBP(p.ndisp);
#endif

    printParams();

    /* (4) Stereo Calibration */
    if(p.needCalibration){

        /* Read Calibration Files */
        calib.readCalibrationFiles();

        /* Read/Calculate the Q Matrix */
        //TODO: Fix the reading process of the Q.yml file
        if(calib.hasQMatrix){
            //if(p.needCalibration){
            calib.readQMatrix(); //true=640x480 false=others
        }else{
            calib.imageCenter = Point2d((imageL.cols-1.0)/2.0,(imageL.rows-1.0)/2.0);
            calib.calculateQMatrix();
        }

        /* Calculate the K Matrix */
        ////        // Checking Intrinsic Matrix
        ////        if(calib.isKcreated){
        ////           cout << "The Intrinsic Matrix is already Created." << endl << endl;
        ////        }else{
        //            //createKMatrix();
        // //       }
        calib.createKMatrix();

    }else{
        cout << "Calibration: OFF" << endl << endl;
        cerr << "Warning: Can't generate 3D Reconstruction. Please, check Q,K Matrix." << endl;

        //readQMatrix(); //true=640x480 false=others
        //createKMatrix();
    }

    running = true;

    /* Stereo Rectification Init */
    stereoRectify(calib.M1,calib.D1,calib.M2,calib.D2,p.getResolutionDesired(),calib.R,calib.T,calib.R1,calib.R2,calib.P1,calib.P2,calib.Q,CALIB_ZERO_DISPARITY,-1,p.getResolutionDesired(),&calib.roi1,&calib.roi2);

}

void App::loop(){
    while (running){
        startClock();
        switch (p.method){
        case Params::BM:
            if (d_imageL.channels() > 1 || d_imageR.channels() > 1){
                cout << "BM doesn't support color images\n";
                cvtColor(imageL_src, imageL, COLOR_BGR2GRAY);
                cvtColor(imageR_src, imageR, COLOR_BGR2GRAY);
                cout << "image_channels: " << imageL.channels() << endl;
                d_imageL.upload(imageL);
                d_imageR.upload(imageR);
                imshow("left", imageL);
                imshow("right", imageR);
            }

            if(isVideoFile){
                capL >> imageL;
                capR >> imageR;

                resizeFrames(&imageL,&imageR,p.getResolutionDesired());

                /* Stereo Rectification */
                if(p.needCalibration){
                    Mat rmap[2][2];
                    initUndistortRectifyMap(calib.M1, calib.D1, calib.R1, calib.P1, p.getResolutionDesired(), CV_16SC2, rmap[0][0], rmap[0][1]);
                    initUndistortRectifyMap(calib.M2, calib.D2, calib.R2, calib.P2, p.getResolutionDesired(), CV_16SC2, rmap[1][0], rmap[1][1]);

                    Mat imageLr, imageRr;
                    remap(imageL, imageLr, rmap[0][0], rmap[0][1], INTER_LINEAR);
                    remap(imageR, imageRr, rmap[1][0], rmap[1][1], INTER_LINEAR);

                    imageL = imageLr;
                    imageR = imageRr;
                }

                /* Converts the new frames to grey */
                //if (imageL.channels() > 1 || imageR.channels() > 1){
                cvtColor(imageL, imageL, COLOR_BGR2GRAY);
                cvtColor(imageR, imageR, COLOR_BGR2GRAY);
                //}

                d_imageL.upload(imageL);
                d_imageR.upload(imageR);

                imshow("left", imageL);
                imshow("right", imageR);
            }
            //startClock();
#ifdef x64
            timer.startClock(&timer.clockInitial_d);
            bm->compute(d_imageL, d_imageR, d_disp);
            timer.stopClock(&timer.clockFinal_d);
            timer.printElapsedTime(timer.clockInitial_d,timer.clockFinal_d);
#endif

#ifdef jetsonTK1
            bm.operator ()(d_imageL, d_imageR, d_disp);
#endif

            //stopClock();
            break;
        }
        stopClock();

        // Show results
        d_disp.download(disp);

        /* TODO: Descomentar Trecho Original */
        //putText(disp, text(), Point(5, 25), FONT_HERSHEY_SIMPLEX, 1.0, Scalar::all(255));
        //imshow("disparity", disp);

        /* TODO: Remover Depois */
#ifdef x64
        cuda::GpuMat d_disp8U(imageL.size(),CV_8U);
#endif

#ifdef jetsonTK1
        gpu::GpuMat d_disp8U(imageL.size(),CV_8U);
#endif
        Mat disp8U(imageL.size(),CV_8U),dispBGR;

        /* Normalization */
#ifdef DISP_NORMALIZATION
        normalize(disp, disp8U, 0, 255, CV_MINMAX,CV_8U); // NORMALIZE = ON
#else
        disp.convertTo(disp8U,CV_8U);                     // NORMALIZE = OFF
#endif
        //cuda::normalize(d_disp,d_disp8U,1,0,NORM_MINMAX,-1);
        //cuda::normalize(d_disp, d_disp8U, 1, 0, NORM_MINMAX, -1, Mat() );
        //d_disp8U.download(disp8U);

        applyColorMap(disp8U,dispBGR, COLORMAP_JET);

        putText(disp8U, text(2), Point(5, 25), FONT_HERSHEY_SIMPLEX, 1.0, Scalar::all(255));
        imshow("disp", disp8U);
        imshow("dispBGR",dispBGR);

        videoLooper();

        handleKey((char)waitKey(3));
    }
}

void App::printParams() const{
    cout << "--- Parameters ---\n";
    cout << "image_size: (" << imageL.cols << ", " << imageL.rows << ")\n";
    cout << "image_channels: " << imageL.channels() << endl;
    cout << "method: " << p.method_str() << endl
         << "ndisp: " << p.ndisp << endl;
    switch (p.method){
    case Params::BM:
#ifdef x64
        cout << "win_size: " << bm->getBlockSize() << endl;
        cout << "prefilter_sobel: " << bm->getPreFilterType() << endl;
#endif

#ifdef jetsonTK1
        cout << "win_size: " << bm.winSize << endl;
        cout << "prefilter_sobel: " << bm.winSize << endl;
#endif
        break;
    }
    cout << endl;
}

//TODO: Add HandleKey for the Threshold value
void App::handleKey(char key){
    switch (key){
    case 27:
        running = false;
        break;
    case 'p': case 'P':
        printParams();
        break;
    case 'g': case 'G':
        if (imageL.channels() == 1 && p.method != Params::BM){
            imageL = imageL_src;
            imageR = imageR_src;
        }
        else{
            cvtColor(imageL_src, imageL, COLOR_BGR2GRAY);
            cvtColor(imageR_src, imageR, COLOR_BGR2GRAY);
        }
        d_imageL.upload(imageL);
        d_imageR.upload(imageR);
        cout << "image_channels: " << imageL.channels() << endl;
        imshow("left", imageL);
        imshow("right", imageR);
        break;
        //    case 'm': case 'M':
        //        switch (p.method){
        //        case Params::BM:
        //            p.method = Params::BP;
        //            break;
        //        case Params::BP:
        //            p.method = Params::CSBP;
        //            break;
        //        case Params::CSBP:
        //            p.method = Params::BM;
        //            break;
        //        }
        //        cout << "method: " << p.method_str() << endl;
        //        break;
        //    case 's': case 'S':
        //        if (p.method == Params::BM){
        //            switch (bm->getPreFilterType()){
        //            case 0:
        //                bm->setPreFilterType(StereoBM::PREFILTER_XSOBEL);
        //                break;
        //            case StereoBM::PREFILTER_XSOBEL:
        //                bm->setPreFilterType(0);
        //                break;
        //            }
        //            cout << "prefilter_sobel: " << bm->getPreFilterType() << endl;
        //        }
        //        break;
    case '1':
        p.ndisp = p.ndisp == 1 ? 8 : min(p.ndisp + 8,256);
        cout << "ndisp: " << p.ndisp << endl;
#ifdef x64
        bm->setNumDisparities(p.ndisp);
        bp->setNumDisparities(p.ndisp);
        csbp->setNumDisparities(p.ndisp);
#endif

#ifdef jetsonTK1
        bm.ndisp = p.ndisp;
#endif
        break;
    case 'q': case 'Q':
        p.ndisp = max(p.ndisp - 8, 8);
        cout << "ndisp: " << p.ndisp << endl;
#ifdef x64
        bm->setNumDisparities(p.ndisp);
        bp->setNumDisparities(p.ndisp);
        csbp->setNumDisparities(p.ndisp);
#endif

#ifdef jetsonTK1
        bm.ndisp = p.ndisp;
#endif
        break;
    case '2':
        if (p.method == Params::BM){
#ifdef x64
            bm->setBlockSize(min(bm->getBlockSize() + 2, 51));
            cout << "win_size: " << bm->getBlockSize() << endl;
#endif

#ifdef jetsonTK1
            bm.winSize = min(bm.winSize + 1, 51);
            cout << "win_size: " << bm.winSize << endl;
#endif
        }
        break;
    case 'w': case 'W':
        if (p.method == Params::BM){
#ifdef x64
            bm->setBlockSize(max(bm->getBlockSize() - 2, 3));
            cout << "win_size: " << bm->getBlockSize() << endl;
#endif

#ifdef jetsonTK1
            bm.winSize = max(bm.winSize - 1, 2);
            cout << "win_size: " << bm.winSize << endl;
#endif
        }
        break;
        //    case '3':
        //        if (p.method == Params::BP){
        //            bp->setNumIters(bp->getNumIters() + 1);
        //            cout << "iter_count: " << bp->getNumIters() << endl;
        //        }
        //        else if (p.method == Params::CSBP){
        //            csbp->setNumIters(csbp->getNumIters() + 1);
        //            cout << "iter_count: " << csbp->getNumIters() << endl;
        //        }
        //        break;
        //    case 'e': case 'E':
        //        if (p.method == Params::BP){
        //            bp->setNumIters(max(bp->getNumIters() - 1, 1));
        //            cout << "iter_count: " << bp->getNumIters() << endl;
        //        }
        //        else if (p.method == Params::CSBP){
        //            csbp->setNumIters(max(csbp->getNumIters() - 1, 1));
        //            cout << "iter_count: " << csbp->getNumIters() << endl;
        //        }
        //        break;
        //    case '4':
        //        if (p.method == Params::BP){
        //            bp->setNumLevels(bp->getNumLevels() + 1);
        //            cout << "level_count: " << bp->getNumLevels() << endl;
        //        }
        //        else if (p.method == Params::CSBP){
        //            csbp->setNumLevels(csbp->getNumLevels() + 1);
        //            cout << "level_count: " << csbp->getNumLevels() << endl;
        //        }
        //        break;
        //    case 'r': case 'R':
        //        if (p.method == Params::BP){
        //            bp->setNumLevels(max(bp->getNumLevels() - 1, 1));
        //            cout << "level_count: " << bp->getNumLevels() << endl;
        //        }
        //        else if (p.method == Params::CSBP){
        //            csbp->setNumLevels(max(csbp->getNumLevels() - 1, 1));
        //            cout << "level_count: " << csbp->getNumLevels() << endl;
        //        }
        //        break;
    }
}


//TODO: Checar se os Valores são iguais ao local na main
void App::stereoBM_GPU_Init(){
    bm = cuda::createStereoBM(p.ndisp);

    //TODO: Create initialization p.variables for the following variables
#ifdef x64
    //    bm->setPreFilterSize(127);
    //    bm->setPreFilterCap(61);
    bm->setBlockSize(15);
    //    bm->setMinDisparity(0);
    bm->setNumDisparities(64);
    bm->setTextureThreshold(4);
    //    bm->setUniquenessRatio(0);
    //    bm->setSpeckleWindowSize(0);
    //    bm->setSpeckleRange(0);
    //    bm->setDisp12MaxDiff(1);
#endif

#ifdef jetsonTK1
    bm.winSize = 15;
    bm.ndisp = 16;
    bm.avergeTexThreshold = 4;
#endif


}

void App::resizeFrames(Mat* frame1,Mat* frame2,Size resolution){
    if(frame1->cols != 0 || !frame2->cols != 0){
        resize(*frame1, *frame1, resolution, 0, 0, INTER_CUBIC);
        resize(*frame2, *frame2, resolution, 0, 0, INTER_CUBIC);
    }
}

void App::videoLooper(){
    frameCounter += 1;

    //Debug
    //cout << "Frames: " << frameCounter << "/" << capL.get(CV_CAP_PROP_FRAME_COUNT) << endl;

    if(frameCounter == capL.get(CV_CAP_PROP_FRAME_COUNT)){
        frameCounter = 0;
        capL.set(CV_CAP_PROP_POS_FRAMES,0);
        capR.set(CV_CAP_PROP_POS_FRAMES,0);
    }
}

void App::startClock(){
    clockInitial = getTickCount();
}

void App::stopClock(){
    d = getTickCount() - clockInitial;
    f = getTickFrequency();
    fps = f / d;
}

string App::text(int precision) const{
    stringstream ss;
    ss << "(" << p.method_str() << ") FPS: " << setiosflags(ios::left) << setprecision(precision) << fps;
    return ss.str();
}
