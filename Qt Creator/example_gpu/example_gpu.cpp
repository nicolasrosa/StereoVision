#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#include <opencv2/gpu/gpu.hpp>
#include <opencv2/gpu/gpumat.hpp>

int main (int argc, char* argv[]){


    try{
        cv::Mat src_host = cv::imread("file.png", CV_LOAD_IMAGE_GRAYSCALE);
        cv::gpu::GpuMat dst,src;
        src.upload(src_host);

        cv::gpu::threshold(src, dst, 128.0, 255.0, CV_THRESH_BINARY);


//        cv::Mat tran(dst);

//        cv::Mat dst_cv(dst);

        cv::Mat result_host;
        dst.download(result_host);
//        cv::Mat result_host(dst);
        cv::imshow("Result", result_host);
        cv::waitKey();
    }
    catch(const cv::Exception& ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
    return 0;
}
