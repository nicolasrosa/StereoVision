#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/cudaarithm.hpp"

using namespace std;
using namespace cv;

int main (int argc, char* argv[]){

    try{
        /* Check CUDA Compability */
        if(cuda::DeviceInfo::isCompatible()){
            cout << "The CUDA module can be run on this specified device: Ok" << endl;
        }else{
            cout << "The CUDA module can't be run on this specified devide: Failed" << endl;
        }


        /* Declaration */
        cv::Mat src,dst;
        cv::cuda::GpuMat src_gpu,dst_gpu;

        //cv::cuda::GpuMat src_gpu(src.size(),CV_8U),dst_gpu(src.size(),CV_8U);

        /* Input */
        src = cv::imread("teddy_l.png",cv::IMREAD_GRAYSCALE);

        /* CPU-GPU Copy - Upload*/
        src_gpu.upload(src);
        cout << "[exampleGPU] Uploading data to GPU..." << endl;

        /* GPU-GPU Copy */
        src_gpu.copyTo(dst_gpu);
        cout << "[exampleGPU] Copying image GPU-GPU..." << endl;

        /* GPU-CPU Copy - Download*/
        //cv::Mat dst(dst_gpu);
        dst_gpu.download(dst);
        cout << "[exampleGPU] Downloading data from GPU..." << endl;

        /* Display Result */
        imshow("src",src);
        imshow("dst",dst);
        cout << "[exampleGPU] Displaying Images..." << endl;

        cv::waitKey();
    }
    catch(const cv::Exception& ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
    return 0;
}

/* Original */
//#include <iostream>
//#include "opencv2/opencv.hpp"
//#include "opencv2/core.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/cudaarithm.hpp"

//using namespace cv;

//int main (int argc, char* argv[])
//{
//    try
//    {
//        cv::Mat src_host = cv::imread("file.png", cv::IMREAD_GRAYSCALE);
//        cv::cuda::GpuMat dst, src;
//        src.upload(src_host);
//        cv::cuda::threshold(src, dst, 128.0, 255.0, cv::THRESH_BINARY);
//        cv::Mat result_host(dst);
//        cv::imshow("Result", result_host);
//        cv::waitKey();
//    }
//    catch(const cv::Exception& ex)
//    {
//        std::cout << "Error: " << ex.what() << std::endl;
//    }
//    return 0;
//}
