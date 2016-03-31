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
    void setResolution(int width, int weight);
    Size getResolution();

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
private:
    Size resolution;

};


#endif // PARAMS_H
