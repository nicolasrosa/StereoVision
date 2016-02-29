#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/cudaarithm.hpp"

using namespace cv;

int main (int argc, char* argv[])
{
    try
    {
        cv::Mat src_host = cv::imread("/home/nicolas/repository/StereoVision/data/teddy_l.png", cv::IMREAD_GRAYSCALE);
        cv::cuda::GpuMat dst, src;
        src.upload(src_host);
        cv::cuda::threshold(src, dst, 128.0, 255.0, cv::THRESH_BINARY);
        cv::Mat result_host(dst);
        cv::imshow("Result", result_host);
        cv::waitKey();
    }
    catch(const cv::Exception& ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
    return 0;
}
