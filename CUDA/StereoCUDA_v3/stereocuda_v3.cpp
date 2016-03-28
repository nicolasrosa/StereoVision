#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <opencv2/core/utility.hpp>
#include "opencv2/cudastereo.hpp"
#include "opencv2/cudaarithm.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

#define RESOLUTION_640x480

bool help_showed = false;

struct Params{
    Params();
    static Params read(int argc, char** argv);

    string left;
    string right;

    string method_str() const
    {
        switch (method){
        case BM: return "BM";
        }
        return "";
    }
    enum {BM} method;
    int ndisp; // Max disparity + 1
};


struct App{
    App(const Params& p);
    void run();
    void handleKey(char key);
    void printParams() const;
    void open();
    void resizeFrames(Mat* frame1,Mat* frame2);
    void videoLooper();
    void startClock(){
        clockInitial = getTickCount();
    }

    void stopClock(){
        d = getTickCount() - clockInitial;
        f = getTickFrequency();
        fps = f / d;
    }

    string text() const{
        stringstream ss;
        ss << "(" << p.method_str() << ") FPS: " << setiosflags(ios::left)
           << setprecision(4) << fps;
        return ss.str();
    }
private:
    Params p;
    int frameCounter;
    bool running;
    bool isVideoFile;
    bool isImageFile;

    VideoCapture capR;
    VideoCapture capL;

    Mat imageL_src, imageR_src;
    Mat imageL, imageR;
    cuda::GpuMat d_imageL, d_imageR;

    Ptr<cuda::StereoBM> bm;
    Ptr<cuda::StereoBeliefPropagation> bp;
    Ptr<cuda::StereoConstantSpaceBP> csbp;

    int64 clockInitial;
    int64 d;
    double f;
    double fps;
};

static void printHelp(){
    cout << "Usage: stereo_match_gpu\n"
         << "\t--left <left_view> --right <right_view> # must be rectified\n"
         << "\t--method <stereo_match_method> # BM | BP | CSBP\n"
         << "\t--ndisp <number> # number of disparity levels\n";
    help_showed = true;
}


int main(int argc, char** argv){
    try{
        if(argc < 2){
            printHelp();
            return 1;
        }

        Params args = Params::read(argc, argv);

        /* Forced exit */
        if (help_showed)
            return -1;

        /* Initializing and runnning the application object */
        App app(args);
        app.run();
    }
    catch(const exception& e){
        cout << "error: " << e.what() << endl;
    }
    return 0;
}


Params::Params(){
    method = BM;
    ndisp = 64;
}


Params Params::read(int argc, char** argv){
    Params p;

    for (int i = 1; i < argc; i++){
        if (string(argv[i]) == "--left") p.left = argv[++i];
        else if (string(argv[i]) == "--right")
            p.right = argv[++i];
        else if (string(argv[i]) == "--method"){
            if (string(argv[i + 1]) == "BM") p.method = BM;
            else throw runtime_error("unknown stereo match method: " + string(argv[i + 1]));
            i++;
        }
        else if (string(argv[i]) == "--ndisp") p.ndisp = atoi(argv[++i]);
        else if (string(argv[i]) == "--help") printHelp();
        else throw runtime_error("unknown key: " + string(argv[i]));
    }

    return p;
}


App::App(const Params& params) : p(params), running(false){
    isVideoFile=false;
    isImageFile=false;
    frameCounter=0;

    cuda::printShortCudaDeviceInfo(cuda::getDevice());

    cout << "stereo_match_gpu sample\n";
    cout << "\nControls:\n"
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
    App::open();

    if(isVideoFile){

    }else{
        cvtColor(imageL_src, imageL, COLOR_BGR2GRAY);
        cvtColor(imageR_src, imageR, COLOR_BGR2GRAY);
        d_imageL.upload(imageL);
        d_imageR.upload(imageR);

        imshow("left", imageL);
        imshow("right", imageR);
    }

    // Set common parameters
    bm = cuda::createStereoBM(p.ndisp);

    //    bm->setPreFilterSize(127);
    //    bm->setPreFilterCap(61);
    bm->setBlockSize(15);
    //    bm->setMinDisparity(0);
    //    bm->setNumDisparities(16);
    bm->setTextureThreshold(4);
    //    bm->setUniquenessRatio(0);
    //    bm->setSpeckleWindowSize(0);
    //    bm->setSpeckleRange(0);
    //    bm->setDisp12MaxDiff(1);

    bp = cuda::createStereoBeliefPropagation(p.ndisp);
    csbp = cuda::createStereoConstantSpaceBP(p.ndisp);

    /* Disparity Map Declaration */
    Mat disp(imageL.size(), CV_8U);
    cuda::GpuMat d_disp(imageL.size(), CV_16S);

    cout << endl;
    printParams();

    int test = 1 << (sizeof(unsigned char) * 8);
    cout << "oi" <<  test << endl;

    running = true;
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

                resizeFrames(&imageL,&imageR);

                /* Converts the new frames to grey */
                cvtColor(imageL, imageL, COLOR_BGR2GRAY);
                cvtColor(imageR, imageR, COLOR_BGR2GRAY);

                d_imageL.upload(imageL);
                d_imageR.upload(imageR);

                imshow("left", imageL);
                imshow("right", imageR);
            }
            //startClock();
            bm->compute(d_imageL, d_imageR, d_disp);
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
        cuda::GpuMat d_disp8U(imageL.size(),CV_8U);
        Mat disp8U(imageL.size(),CV_8U),dispBGR;

        //disp.convertTo(disp8U,CV_8U);
        normalize(disp, disp8U, 0, 255, CV_MINMAX,CV_8U);
        //cuda::normalize(d_disp,d_disp8U,1,0,NORM_MINMAX,-1);
        //cuda::normalize(d_disp, d_disp8U, 1, 0, NORM_MINMAX, -1, Mat() );
        //d_disp8U.download(disp8U);

        applyColorMap(disp8U,dispBGR, COLORMAP_JET);

        putText(disp8U, text(), Point(5, 25), FONT_HERSHEY_SIMPLEX, 1.0, Scalar::all(255));

        putText(disp8U, text(), Point(5, 25), FONT_HERSHEY_SIMPLEX, 1.0, Scalar::all(255));
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
        cout << "win_size: " << bm->getBlockSize() << endl;
        cout << "prefilter_sobel: " << bm->getPreFilterType() << endl;
        break;
    }
    cout << endl;
}


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
    case 's': case 'S':
        if (p.method == Params::BM){
            switch (bm->getPreFilterType()){
            case 0:
                bm->setPreFilterType(StereoBM::PREFILTER_XSOBEL);
                break;
            case StereoBM::PREFILTER_XSOBEL:
                bm->setPreFilterType(0);
                break;
            }
            cout << "prefilter_sobel: " << bm->getPreFilterType() << endl;
        }
        break;
    case '1':
        p.ndisp = p.ndisp == 1 ? 8 : min(p.ndisp + 8,256);
        cout << "ndisp: " << p.ndisp << endl;
        bm->setNumDisparities(p.ndisp);
        bp->setNumDisparities(p.ndisp);
        csbp->setNumDisparities(p.ndisp);
        break;
    case 'q': case 'Q':
        p.ndisp = max(p.ndisp - 8, 8);
        cout << "ndisp: " << p.ndisp << endl;
        bm->setNumDisparities(p.ndisp);
        bp->setNumDisparities(p.ndisp);
        csbp->setNumDisparities(p.ndisp);
        break;
    case '2':
        if (p.method == Params::BM){
            bm->setBlockSize(min(bm->getBlockSize() + 2, 51));
            cout << "win_size: " << bm->getBlockSize() << endl;
        }
        break;
    case 'w': case 'W':
        if (p.method == Params::BM){
            bm->setBlockSize(max(bm->getBlockSize() - 2, 3));
            cout << "win_size: " << bm->getBlockSize() << endl;
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

void App::resizeFrames(Mat* frame1,Mat* frame2){
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
