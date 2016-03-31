#include "params.h"

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <stdlib.h>

Params::Params(){
    method = BM;
    ndisp = 64;
}

void Params::setResolution(int width,int height){
    resolution.width = width;
    resolution.height = height;
}

Size Params::getResolution(){
    return(resolution);
}
