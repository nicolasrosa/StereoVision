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

using namespace std;
using namespace cv;

int main (int argc, char* argv[]){

    try{
        cv::Mat src_host = cv::imread("teddy_l.png", CV_LOAD_IMAGE_GRAYSCALE);
        cv::cuda::GpuMat dst,src;
        src.upload(src_host);

        cv::cuda::threshold(src, dst, 128.0, 255.0, CV_THRESH_BINARY);
//        cv::cuda::

////        cv::Mat tran(dst);

////        cv::Mat dst_cv(dst);

//        cv::Mat result_host;
//        dst.download(result_host);
////        cv::Mat result_host(dst);
//        cv::imshow("Result", result_host);
//        cv::waitKey();
    }
    catch(const cv::Exception& ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
    return 0;
}
