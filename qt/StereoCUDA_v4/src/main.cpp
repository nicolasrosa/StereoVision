/* Libraries */
#include "inc/app.h"
#include "inc/params.h"

using namespace cv;
using namespace std;

/* Global Variables */
bool help_showed = false;

static void printHelp(){
    cout << "Usage: stereo_match_gpu\n"
         << "\t--left <left_view> --right <right_view> # must be rectified\n"
         << "\t--method <stereo_match_method> # BM | SGBM | BMGPU | BP | CSBP\n"
         << "\t--ndisp <number> # number of disparity levels\n"
         << "\t--calib <bool> # Set 'true' or 'false' for enabling the Calibration.\n";
    help_showed = true;
}

int main(int argc, char** argv){
    try{
        if(argc < 2){
            printHelp();
            return 1;
        }

        Params args = Params::read(argc, argv);

        //args.setResolutionDesired(320,240);
        args.setResolutionDesired(640,480);
        //args.setResolutionDesired(1280,720);

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

Params Params::read(int argc, char** argv){
    Params p;

    //cout << "Number of arguments: " << argc << endl;

    for (int i = 1; i < argc; i++){
        if (string(argv[i]) == "--left") p.left = argv[++i];
        else if (string(argv[i]) == "--right")
            p.right = argv[++i];
        else if (string(argv[i]) == "--method"){
            if (string(argv[i + 1]) == "BM") p.method = BM;
            else if(string(argv[i + 1]) == "SGBM") p.method = SGBM;
            else if(string(argv[i + 1]) == "BMGPU") p.method = BMGPU;
            else throw runtime_error("unknown stereo match method: " + string(argv[i + 1]));
            i++;
        }
        else if (string(argv[i]) == "--ndisp") p.ndisp = atoi(argv[++i]);
        else if (string(argv[i]) == "--calib"){
            std::istringstream ss(argv[++i]);
            ss >> std::boolalpha >> p.needCalibration;
            //cout << "needCalibration: " << p.needCalibration << endl;
        }
        else if (string(argv[i]) == "--help") printHelp();
        else throw runtime_error("unknown key: " + string(argv[i]));
    }

    return p;
}
