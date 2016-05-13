#ifndef PARAMS_H
#define PARAMS_H

/* Libraries */
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class Params{
public:
    Params();
    static Params read(int argc, char** argv);
    void setResolutionDesired(int width, int weight);
    Size getResolutionDesired();

    string left;
    string right;

    string method_str() const
    {
        switch (method){
        case BM: return "BM";
        case SGBM: return "SGBM";
        case BMGPU: return "BMGPU";
        }
        return "";
    }

    string inputType_str() const
    {
        switch (inputType){
        case AVI: return "avi";
        case JPG: return "jpg";
        case PNG: return "png";
        }
        return "";
    }

    enum {AVI,JPG,PNG} inputType;
    enum {BM,SGBM,BMGPU} method;

    int ndisp; // Max disparity + 1
    int minDisparity;
    bool needCalibration;
private:
    Size resolution;

};


#endif // PARAMS_H
