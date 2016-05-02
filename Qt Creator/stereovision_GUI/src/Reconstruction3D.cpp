/*
 * 3DReconstruction.cpp
 *
 *  Created on: Oct 25, 2015
 *      Author: nicolasrosa
 */

/* Libraries */
//TODO: Remover Biblioteca e Arquivos do Método de Visualização 3D Antigo
#include "inc/Reconstruction3D.h"

/* Constructor and Destructor */
Reconstruction3D::Reconstruction3D(){}
Reconstruction3D::~Reconstruction3D(){}

/* Instance Methods */
void Reconstruction3D::setViewPoint(double x,double y,double z){
    viewpoint.x = x;
    viewpoint.y = y;
    viewpoint.z = z;
}


void Reconstruction3D::setLookAtPoint(double x,double y,double z){
    lookatpoint.x = x;
    lookatpoint.y = y;
    lookatpoint.z = z;
}


void Reconstruction3D::PointCloudInit(double baseline,bool isSub){
    dist=Mat::zeros(5,1,CV_64F);
    Rotation=Mat::eye(3,3,CV_64F);
    t=Mat::zeros(3,1,CV_64F);

    isSub = isSub;
    step = baseline/10;

    //Point3d viewpoint(20.0,20.0,-baseline*10);
    //Point3d lookatpoint(22.0,16.0,baseline*10.0);

    setViewPoint(10,10,-2*baseline*10);
    setLookAtPoint(0,0,-baseline*10);
}


void Reconstruction3D::eular2rot(double yaw,double pitch, double roll,Mat& dest){
    double theta = yaw/180.0*CV_PI;
    double pusai = pitch/180.0*CV_PI;
    double phi = roll/180.0*CV_PI;

    double datax[3][3] = {{1.0,0.0,0.0},{0.0,cos(theta),-sin(theta)},{0.0,sin(theta),cos(theta)}};
    double datay[3][3] = {{cos(pusai),0.0,sin(pusai)},{0.0,1.0,0.0},{-sin(pusai),0.0,cos(pusai)}};
    double dataz[3][3] = {{cos(phi),-sin(phi),0.0},{sin(phi),cos(phi),0.0},{0.0,0.0,1.0}};
    Mat Rx(3,3,CV_64F,datax);
    Mat Ry(3,3,CV_64F,datay);
    Mat Rz(3,3,CV_64F,dataz);
    Mat rr=Rz*Rx*Ry;
    rr.copyTo(dest);
}


void Reconstruction3D::lookat(Point3d from, Point3d to, Mat& destR){
    double x=(to.x-from.x);
    double y=(to.y-from.y);
    double z=(to.z-from.z);

    double pitch =asin(x/sqrt(x*x+z*z))/CV_PI*180.0;
    double yaw =asin(-y/sqrt(y*y+z*z))/CV_PI*180.0;

    eular2rot(yaw, pitch, 0,destR);
}


void Reconstruction3D::projectImagefromXYZ(Mat &image, Mat &destimage, Mat &disp, Mat &destdisp, Mat &xyz, Mat &R, Mat &t, Mat &K, Mat &dist, bool isSub){
    Mat mask;
    if(mask.empty())mask=Mat::zeros(image.size(),CV_8U);
    if(disp.type()==CV_8U){
        projectImagefromXYZ_<unsigned char>(image,destimage, disp, destdisp, xyz, R, t, K, dist, mask,isSub);
    }
    else if(disp.type()==CV_16S){
        projectImagefromXYZ_<short>(image,destimage, disp, destdisp, xyz, R, t, K, dist, mask,isSub);
    }
    else if(disp.type()==CV_16U){
        projectImagefromXYZ_<unsigned short>(image,destimage, disp, destdisp, xyz, R, t, K, dist, mask,isSub);
    }
    else if(disp.type()==CV_32F){
        projectImagefromXYZ_<float>(image,destimage, disp, destdisp, xyz, R, t, K, dist, mask,isSub);
    }
    else if(disp.type()==CV_64F){
        projectImagefromXYZ_<double>(image,destimage, disp, destdisp, xyz, R, t, K, dist, mask,isSub);
    }
}


void Reconstruction3D::fillOcclusion(Mat& src, int invalidvalue, bool isInv){
    if(isInv){
        if(src.type()==CV_8U){
            fillOcclusionInv_<uchar>(src, (uchar)invalidvalue);
        }
        else if(src.type()==CV_16S){
            fillOcclusionInv_<short>(src, (short)invalidvalue);
        }
        else if(src.type()==CV_16U){
            fillOcclusionInv_<unsigned short>(src, (unsigned short)invalidvalue);
        }
        else if(src.type()==CV_32F){
            fillOcclusionInv_<float>(src, (float)invalidvalue);
        }
    }
    else{
        if(src.type()==CV_8U){
            fillOcclusion_<uchar>(src, (uchar)invalidvalue);
        }
        else if(src.type()==CV_16S){
            fillOcclusion_<short>(src, (short)invalidvalue);
        }
        else if(src.type()==CV_16U){
            fillOcclusion_<unsigned short>(src, (unsigned short)invalidvalue);
        }
        else if(src.type()==CV_32F){
            fillOcclusion_<float>(src, (float)invalidvalue);
        }
    }
}
