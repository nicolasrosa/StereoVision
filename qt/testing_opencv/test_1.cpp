// Test to check the OpenCV version
// Build on Linux with:
// g++ test_opencv.cpp -o test_opencv -lopencv_core
 
#include <opencv2/opencv.hpp>
#include <iostream>
 
int main() {
 	std::cout << "Hello, OpenCV version "<< CV_VERSION << std::endl;
 	return 0;
}
