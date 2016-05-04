/*
 * trackObject.h
 *
 *  Author: Kyle Hounslow
 * 	Link: https://www.youtube.com/watch?v=bSeFrPrqZ2A
 *  Published in: March 11, 2013
 */

#ifndef SRC_TRACKOBJECT_H_
#define SRC_TRACKOBJECT_H_

/* Libraries */
#include <opencv2/opencv.hpp>
#include "inc/StereoUtils.h"

using namespace cv;
using namespace std;

class TrackObject{
public:
    /* Constructor and Destructor */
//    TrackObject(StereoTime::StereoUtils **ptr);
    TrackObject();
    ~TrackObject();

    void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed);
    void drawObject(int x, int y,Mat &frame);
    string intToString(int number);

private:
    /* Default capture width and height */
    const int FRAME_WIDTH;
    const int FRAME_HEIGHT;

    /* Max number of objects to be detected in frame */
    const int MAX_NUM_OBJECTS;

    /* Minimum and maximum object area */
    const int MIN_OBJECT_AREA;
    const int MAX_OBJECT_AREA;
};

#endif /* SRC_TRACKOBJECT_H_ */
