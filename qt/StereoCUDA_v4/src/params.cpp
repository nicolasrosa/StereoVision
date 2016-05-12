#include "inc/params.h"

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <stdlib.h>

Params::Params(){
    method = BM;
    ndisp = 64;
    minDisparity = 0;
    needCalibration=false;
}

void Params::setResolutionDesired(int width,int height){
    resolution.width = width;
    resolution.height = height;
}

Size Params::getResolutionDesired(){
    return(resolution);
}
