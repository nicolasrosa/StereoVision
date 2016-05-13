#ifndef STEREOCUDA_H
#define STEREOCUDA_H

/* Deployment Platform */
#define x64
//#define jetsonTK1

/* Enable Disparity Map Normalization */
#define DISP_NORMALIZATION

/* Libraries */
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>

#ifdef x64
#include <opencv2/core/utility.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/cudastereo.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#endif

#ifdef jetsonTK1
#include "opencv2/gpu/gpu.hpp"
#include "opencv2/gpu/gpumat.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"
#endif

#endif // STEREOCUDA_H
