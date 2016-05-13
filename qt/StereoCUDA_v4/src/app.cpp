#include "inc/app.h"

#include <iostream>

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
    cout << "Program used the stereo_match_gpu sample as base.\n"
         << "\nControls:\n"
         << "\tesc - exit\n"
         << "\tp - print current parameters\n"
         << "\tg - convert source images into gray\n"
         << "\tm - change stereo match method\n"
         << "\ts - change Sobel prefiltering flag (for BM only)\n"
         << "\t1/q - increase/decrease maximum disparity\n"
         << "\t2/w - increase/decrease window size (for BM only)\n"
         << "\t3/e - increase/decrease iteration count (for BP and CSBP only)\n"
         << "\t4/r - increase/decrease level count (for BP and CSBP only)\n\n";
}

void App::run(){
    App::printHelp();
    App::open();    //t = 0.0157288
    App::init();    //t = 0.0525973
    App::loop();    //t = 0.0452023
}

void App::open(){
    /* Identify the type of the input file. */
    //TODO: Add Exception
    if (p.left.substr(p.left.find_last_of(".") + 1) == "avi") p.inputType = Params::AVI;
    else if(p.left.substr(p.left.find_last_of(".") + 1) == "jpg") p.inputType = Params::JPG;
    else if(p.left.substr(p.left.find_last_of(".") + 1) == "png") p.inputType = Params::PNG;

    switch(p.inputType){
    case Params::AVI:
        isVideoFile=true;

        capL.open(p.left);
        capR.open(p.right);

        if(!capL.isOpened() || !capR.isOpened()){   // Check if it succeeded
            cerr <<  "Could not open or find the input videos!" << endl ;
            return;
        }

        /* Console Output */
        cout << "It's a Video file" << endl;
        cout << "Input 1 Resolution: " << capR.get(CV_CAP_PROP_FRAME_WIDTH) << "x" << capR.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
        cout << "Input 2 Resolution: " << capL.get(CV_CAP_PROP_FRAME_WIDTH) << "x" << capL.get(CV_CAP_PROP_FRAME_HEIGHT) << endl << endl;

        break;
    case Params::JPG:
    case Params::PNG:
        cout << "It's a Image file" << endl;
        isImageFile=true;

        imageL = imread(p.left);
        imageR = imread(p.right);

        //if (imageL_src.empty()) throw runtime_error("can't open file \"" + p.left + "\"");
        //if (imageR_src.empty()) throw runtime_error("can't open file \"" + p.right + "\"");

        if(!imageL_grey.data || !imageR_grey.data){      // Check if it succeeded
            cout << "Could not open or find the input images!" << endl;
            return;
        }
        break;
    default:
        cout << "It is not a Video or Image file" << endl;
    }
}


void App::init(){
    if(isVideoFile){

    }else{
        cvtColor(imageL, imageL_grey, COLOR_BGR2GRAY);
        cvtColor(imageR, imageR_grey, COLOR_BGR2GRAY);
        d_imageL.upload(imageL_grey);
        d_imageR.upload(imageR_grey);

        imshow("left", imageL_grey);
        imshow("right", imageR_grey);
    }

    /* Initializing Stereo Matching Methods - Set Common Parameters */
#ifdef x64
    stereoBMGPU_Init();
    bm = StereoBM::create(16,9);
    sgbm = StereoSGBM::create(0,16,3);

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
            calib.imageCenter = Point2d((imageL_grey.cols-1.0)/2.0,(imageL_grey.rows-1.0)/2.0);
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
    StereoRectificationInit();
}

void App::StereoRectificationInit(){
    stereoRectify(calib.M1,calib.D1,calib.M2,calib.D2,p.getResolutionDesired(),calib.R,calib.T,calib.R1,calib.R2,calib.P1,calib.P2,calib.Q,CALIB_ZERO_DISPARITY,-1,p.getResolutionDesired(),&calib.roi1,&calib.roi2);

    initUndistortRectifyMap(calib.M1, calib.D1, calib.R1, calib.P1, p.getResolutionDesired(), CV_16SC2, rmap[0][0], rmap[0][1]);
    initUndistortRectifyMap(calib.M2, calib.D2, calib.R2, calib.P2, p.getResolutionDesired(), CV_16SC2, rmap[1][0], rmap[1][1]);
}

void App::loop(){
    while (running){
        timer1.startClock();
        switch (p.method){
        case Params::BM:
        case Params::BMGPU:
            //FIXME: Fix Loop for Image Files
            if(isVideoFile){
                capL >> imageL;
                capR >> imageR;

                resizeFrames(&imageL,&imageR,p.getResolutionDesired());

                /* Stereo Rectification */
                if(p.needCalibration){
                    Mat imageLr, imageRr;
                    remap(imageL, imageLr, rmap[0][0], rmap[0][1], INTER_LINEAR);
                    remap(imageR, imageRr, rmap[1][0], rmap[1][1], INTER_LINEAR);

                    imageL = imageLr;
                    imageR = imageRr;
                }

                /* Converts the new frames to grey */
                cvtColor(imageL, imageL_grey, COLOR_BGR2GRAY);
                cvtColor(imageR, imageR_grey, COLOR_BGR2GRAY);

                d_imageL.upload(imageL_grey);
                d_imageR.upload(imageR_grey);
            }

            timer2.startClock();
#ifdef x64
            switch(p.method){
            case Params::BM:
                bm->compute(imageL_grey,imageR_grey,disp);
                break;
            case Params::SGBM:
                sgbm->compute(imageL,imageR,disp);
            case Params::BMGPU:
                bm_gpu->compute(d_imageL, d_imageR, d_disp);
                break;
            }
#endif

#ifdef jetsonTK1
            bm_gpu.operator ()(d_imageL, d_imageR, d_disp);
#endif

            timer2.stopClock();
            //timer2.printElapsedTime();

            timer1.stopClock();
            timer1.calculateFPS();
            //timer1.printElapsedTime();
            break;

        case Params::SGBM:
            break;
        }

        // Show results
        d_disp.download(disp);

        /* TODO: Remover Depois */
#ifdef x64
        cuda::GpuMat d_disp8U(imageL_grey.size(),CV_8U);
#endif

#ifdef jetsonTK1
        gpu::GpuMat d_disp8U(imageL.size(),CV_8U);
#endif
        Mat disp8U(imageL_grey.size(),CV_8U);
        Mat dispBGR;
        cuda::GpuMat d_dispBGR;

        /* Normalization */
#ifdef DISP_NORMALIZATION
        normalize(disp, disp8U, 0, 255, CV_MINMAX,CV_8U); // NORMALIZE = ON
#else
        disp.convertTo(disp8U,CV_8U);                     // NORMALIZE = OFF
#endif
        //cuda::normalize(d_disp,d_disp8U,1,0,NORM_MINMAX,-1);
        //cuda::normalize(d_disp, d_disp8U, 1, 0, NORM_MINMAX, -1, Mat() );
        //d_disp8U.download(disp8U);

        //! Option 1 !//
        //cuda::drawColorDisp(d_disp,d_dispBGR,64);
        //d_dispBGR.download(dispBGR);

        //! Option 2 //
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
    cout << "image_size: (" << imageL_grey.cols << ", " << imageL_grey.rows << ")\n";
    cout << "image_channels: " << imageL_grey.channels() << endl;
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

    case Params::SGBM:
#ifdef x64
        cout << "win_size: " << sgbm->getBlockSize() << endl;
        //cout << "prefilter_sobel: " << sgbm->getPreFilterType() << endl;
#endif

#ifdef jetsonTK1
        cout << "win_size: " << sgbm.winSize << endl;
        cout << "prefilter_sobel: " << sgbm.winSize << endl;
#endif
        break;

    case Params::BMGPU:
#ifdef x64
        cout << "win_size: " << bm_gpu->getBlockSize() << endl;
        cout << "prefilter_sobel: " << bm_gpu->getPreFilterType() << endl;
#endif

#ifdef jetsonTK1
        cout << "win_size: " << bm_gpu.winSize << endl;
        cout << "prefilter_sobel: " << bm_gpu.winSize << endl;
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
        if (imageL_grey.channels() == 1 && p.method != Params::BMGPU){
            imageL_grey = imageL;
            imageR_grey = imageR;
        }
        else{
            cvtColor(imageL, imageL_grey, COLOR_BGR2GRAY);
            cvtColor(imageR, imageR_grey, COLOR_BGR2GRAY);
        }
        d_imageL.upload(imageL_grey);
        d_imageR.upload(imageR_grey);
        cout << "image_channels: " << imageL_grey.channels() << endl;
        imshow("left", imageL_grey);
        imshow("right", imageR_grey);
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
        bm_gpu->setNumDisparities(p.ndisp);
        bp->setNumDisparities(p.ndisp);
        csbp->setNumDisparities(p.ndisp);
#endif

#ifdef jetsonTK1
        bm_gpu.ndisp = p.ndisp;
#endif
        break;
    case 'q': case 'Q':
        p.ndisp = max(p.ndisp - 8, 8);
        cout << "ndisp: " << p.ndisp << endl;
#ifdef x64
        bm_gpu->setNumDisparities(p.ndisp);
        bp->setNumDisparities(p.ndisp);
        csbp->setNumDisparities(p.ndisp);
#endif

#ifdef jetsonTK1
        bm_gpu.ndisp = p.ndisp;
#endif
        break;
    case '2':
        if (p.method == Params::BMGPU){
#ifdef x64
            bm_gpu->setBlockSize(min(bm_gpu->getBlockSize() + 2, 51));
            cout << "win_size: " << bm_gpu->getBlockSize() << endl;
#endif

#ifdef jetsonTK1
            bm_gpu.winSize = min(bm_gpu.winSize + 1, 51);
            cout << "win_size: " << bm_gpu.winSize << endl;
#endif
        }
        break;
    case 'w': case 'W':
        if (p.method == Params::BMGPU){
#ifdef x64
            bm_gpu->setBlockSize(max(bm_gpu->getBlockSize() - 2, 3));
            cout << "win_size: " << bm_gpu->getBlockSize() << endl;
#endif

#ifdef jetsonTK1
            bm_gpu.winSize = max(bm_gpu.winSize - 1, 2);
            cout << "win_size: " << bm_gpu.winSize << endl;
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


void App::stereoBMGPU_Init(){
    //TODO: Create initialization p.variables for the following variables
#ifdef x64
    bm_gpu = cuda::createStereoBM(p.ndisp);


    //    bm->setPreFilterSize(127);
    //    bm->setPreFilterCap(61);
    bm_gpu->setBlockSize(19);
    //    bm->setMinDisparity(0);
    bm_gpu->setNumDisparities(64);
    bm_gpu->setTextureThreshold(4);
    //    bm->setUniquenessRatio(0);
    //    bm->setSpeckleWindowSize(0);
    //    bm->setSpeckleRange(0);
    //    bm->setDisp12MaxDiff(1);
#endif

#ifdef jetsonTK1
    bm_gpu.winSize = 15;
    bm_gpu.ndisp = 16;
    bm_gpu.avergeTexThreshold = 4;
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

string App::text(int precision) const{
    stringstream ss;
    ss << "(" << p.method_str() << ") FPS: " << setiosflags(ios::left) << setprecision(precision) << timer1.getFPS();
    return ss.str();
}
