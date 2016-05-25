// Test to check the OpenCV version
// Build on Linux with:
// g++ test_opencv.cpp -o test_opencv -lopencv_core

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

int main() {
    cout << "Hello, OpenCV version "<< CV_VERSION << "!" << endl;
 	return 0;
}
