#ifndef STEREOCUSTOM_H
#define STEREOCUSTOM_H

/* Threshold, Erosion, Dilation and Blur Constants */
#define THRESH_VALUE   100
#define EROSION_SIZE     5
#define DILATION_SIZE    5
#define BLUR_SIZE        3

/* Trackbars Variables
 * Initial min and max BM Parameters values.These will be changed using trackbars
 */
const int preFilterSize_MAX		 	= 100;
const int preFilterCap_MAX		 	= 100;
const int SADWindowSize_MAX		 	= 100;
const int minDisparity_MAX		 	= 100;
const int numberOfDisparities_MAX 	= 16;
const int textureThreshold_MAX		= 100;
const int uniquenessRatio_MAX		= 100;
const int speckleWindowSize_MAX	 	= 100;
const int speckleRange_MAX		 	= 100;
const int disp12MaxDiff_MAX		 	= 1;

/* Libraries */
#include <opencv2/opencv.hpp>

/* Custom Libraries */
#include "3DReconstruction.h"
#include "setstereoparams.h"

using namespace cv;
using namespace std;

/* Custom Classes */
class StereoFlags{
public:
    StereoFlags(); //Constructor

    bool showInputImages;
    bool showXYZ;
    bool showStereoParam;
    bool showStereoParamValues;
    bool showFPS;
    bool showDisparityMap;
    bool show3Dreconstruction;
    bool showTrackingObjectView;
    bool showDiffImage;
    bool showWarningLines;
};

class StereoDiff{
public:
    StereoDiff(); //Constructor
    void createDiffImage(Mat,Mat);
    void createResAND(Mat,Mat);
    void convertToBGR();
    void addRedLines();

    bool StartDiff;
    Mat diffImage;

    Mat res_AND;
    Mat imageL;
    Mat res_AND_BGR;
    Mat res_AND_BGR_channels[3];

    double alpha;
    double beta;
    Mat res_ADD;
};

class StereoConfig{
public:
    StereoConfig(); //Constructor
    StereoConfig getConfig();

    int preFilterSize;
    int preFilterCap;
    int SADWindowSize;
    int minDisparity;
    int numberOfDisparities;
    int textureThreshold;
    int uniquenessRatio;
    int speckleWindowSize;
    int speckleRange;
    int disp12MaxDiff;
};

class StereoCalib{
public:
    StereoCalib(); //Constructor
    void readQMatrix();
    void calculateQMatrix();
    void createKMatrix();

    string intrinsicsFileName;
    string extrinsicsFileName;
    string QmatrixFileName;
    string StereoParamFileName;

    Point2d imageCenter;

    Mat K,Q;
    double focalLength;
    double baseline;
    bool is320x240;
    bool is640x480;
    bool is1280x720;

    Mat M1,D1,M2,D2;
    Mat R,T,R1,P1,R2,P2;
    Rect roi1, roi2;
    bool isKcreated;
};

class StereoDisparityMap{
public:
    StereoDisparityMap(); //Constructor

    Mat disp_16S;
    Mat disp_8U;
    Mat disp_BGR;
};

/* Global Variables */
const std::string trackbarWindowName = "Stereo Param Setup";

#endif // STEREOCUSTOM_H
