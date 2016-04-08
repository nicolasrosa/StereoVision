// Test to convert a color image to gray
// Build on Linux with:
// g++ test_2.cpp -o test_2 -lopencv_core -lopencv_imgproc -lopencv_highgui

#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Load the image file and check for success
    cv::Mat input = cv::imread("lena.jpg", 1);
    if(!input.data) {
        std::cout << "Unable to open the image file" << std::endl;
        return -1;
    }

    // Convert the input file to gray
    cv::Mat gray_image;
    cvtColor(input, gray_image, cv::COLOR_BGR2GRAY);

    // Save the result
    cv::imwrite("lena_gray.jpg", gray_image);

    return 0;
}
