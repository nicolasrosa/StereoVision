/*
 * 3DReconstruction.h
 *
 *  Created on: Oct 25, 2015
 *      Author: nicolasrosa
 */

#ifndef RECONSTRUCTION_3D_H
#define RECONSTRUCTION_3D_H

/* Libraries */
#include "opencv2/opencv.hpp"

//TODO: Arrumar Bibliotecas
//#include <pcl/common/common_headers.h>
//#include <pcl/io/pcd_io.h>
//#include <pcl/visualization/pcl_visualizer.h>
//#include <boost/thread/thread.hpp>

using namespace cv;
using namespace std;

class Reconstruction3D{
public:
    /* Constructor and Destructor */
    Reconstruction3D();
    ~Reconstruction3D();

    void setViewPoint(double x,double y,double z);
    void setLookAtPoint(double x,double y,double z);
    void PointCloudInit(double baseline,bool isSub);


    /* 3D Reconstruction Functions */
    void eular2rot(double yaw,double pitch, double roll,Mat& dest);
    void lookat(Point3d from, Point3d to, Mat& destR);
    void projectImagefromXYZ(Mat &image, Mat &destimage, Mat &disp, Mat &destdisp, Mat &xyz, Mat &R, Mat &t, Mat &K, Mat &dist, bool isSub);
    void fillOcclusion(Mat& src, int invalidvalue, bool isInv);

    /* Templates Declaration */
    template <class T>
    static void projectImagefromXYZ_(Mat& image, Mat& destimage, Mat& disp, Mat& destdisp, Mat& xyz, Mat& R, Mat& t, Mat& K, Mat& dist, Mat& mask, bool isSub){
        if(destimage.empty())destimage=Mat::zeros(Size(image.size()),image.type());
        if(destdisp.empty())destdisp=Mat::zeros(Size(image.size()),disp.type());

        vector<Point2f> pt;
        if(dist.empty()) dist = Mat::zeros(Size(5,1),CV_32F);
        cv::projectPoints(xyz,R,t,K,dist,pt);
        destimage.setTo(0);
        destdisp.setTo(0);

        //#pragma omp parallel for
        for(int j=1;j<image.rows-1;j++){
            int count=j*image.cols;
            uchar* img=image.ptr<uchar>(j);
            uchar* m=mask.ptr<uchar>(j);
            for(int i=0;i<image.cols;i++,count++){
                int x=(int)(pt[count].x+0.5);
                int y=(int)(pt[count].y+0.5);
                if(m[i]==255)continue;
                if(pt[count].x>=1 && pt[count].x<image.cols-1 && pt[count].y>=1 && pt[count].y<image.rows-1){
                    short v=destdisp.at<T>(y,x);
                    if(v<disp.at<T>(j,i)){
                        destimage.at<uchar>(y,3*x+0)=img[3*i+0];
                        destimage.at<uchar>(y,3*x+1)=img[3*i+1];
                        destimage.at<uchar>(y,3*x+2)=img[3*i+2];
                        destdisp.at<T>(y,x)=disp.at<T>(j,i);

                        if(isSub){
                            if((int)pt[count+image.cols].y-y>1 && (int)pt[count+1].x-x>1){
                                destimage.at<uchar>(y,3*x+3)=img[3*i+0];
                                destimage.at<uchar>(y,3*x+4)=img[3*i+1];
                                destimage.at<uchar>(y,3*x+5)=img[3*i+2];

                                destimage.at<uchar>(y+1,3*x+0)=img[3*i+0];
                                destimage.at<uchar>(y+1,3*x+1)=img[3*i+1];
                                destimage.at<uchar>(y+1,3*x+2)=img[3*i+2];

                                destimage.at<uchar>(y+1,3*x+3)=img[3*i+0];
                                destimage.at<uchar>(y+1,3*x+4)=img[3*i+1];
                                destimage.at<uchar>(y+1,3*x+5)=img[3*i+2];

                                destdisp.at<T>(y,x+1)=disp.at<T>(j,i);
                                destdisp.at<T>(y+1,x)=disp.at<T>(j,i);
                                destdisp.at<T>(y+1,x+1)=disp.at<T>(j,i);
                            }
                            else if((int)pt[count-image.cols].y-y<-1 && (int)pt[count-1].x-x<-1){
                                destimage.at<uchar>(y,3*x-3)=img[3*i+0];
                                destimage.at<uchar>(y,3*x-2)=img[3*i+1];
                                destimage.at<uchar>(y,3*x-1)=img[3*i+2];

                                destimage.at<uchar>(y-1,3*x+0)=img[3*i+0];
                                destimage.at<uchar>(y-1,3*x+1)=img[3*i+1];
                                destimage.at<uchar>(y-1,3*x+2)=img[3*i+2];

                                destimage.at<uchar>(y-1,3*x-3)=img[3*i+0];
                                destimage.at<uchar>(y-1,3*x-2)=img[3*i+1];
                                destimage.at<uchar>(y-1,3*x-1)=img[3*i+2];

                                destdisp.at<T>(y,x-1)=disp.at<T>(j,i);
                                destdisp.at<T>(y-1,x)=disp.at<T>(j,i);
                                destdisp.at<T>(y-1,x-1)=disp.at<T>(j,i);
                            }
                            else if((int)pt[count+1].x-x>1){
                                destimage.at<uchar>(y,3*x+3)=img[3*i+0];
                                destimage.at<uchar>(y,3*x+4)=img[3*i+1];
                                destimage.at<uchar>(y,3*x+5)=img[3*i+2];

                                destdisp.at<T>(y,x+1)=disp.at<T>(j,i);
                            }
                            else if((int)pt[count-1].x-x<-1){
                                destimage.at<uchar>(y,3*x-3)=img[3*i+0];
                                destimage.at<uchar>(y,3*x-2)=img[3*i+1];
                                destimage.at<uchar>(y,3*x-1)=img[3*i+2];

                                destdisp.at<T>(y,x-1)=disp.at<T>(j,i);
                            }
                            else if((int)pt[count+image.cols].y-y>1){
                                destimage.at<uchar>(y+1,3*x+0)=img[3*i+0];
                                destimage.at<uchar>(y+1,3*x+1)=img[3*i+1];
                                destimage.at<uchar>(y+1,3*x+2)=img[3*i+2];

                                destdisp.at<T>(y+1,x)=disp.at<T>(j,i);
                            }
                            else if((int)pt[count-image.cols].y-y<-1){
                                destimage.at<uchar>(y-1,3*x+0)=img[3*i+0];
                                destimage.at<uchar>(y-1,3*x+1)=img[3*i+1];
                                destimage.at<uchar>(y-1,3*x+2)=img[3*i+2];

                                destdisp.at<T>(y-1,x)=disp.at<T>(j,i);
                            }
                        }
                    }
                }
            }
        }

        if(isSub)
        {
            Mat image2;
            Mat disp2;
            destimage.copyTo(image2);
            destdisp.copyTo(disp2);
            const int BS=1;
            //#pragma omp parallel for
            for(int j=BS;j<image.rows-BS;j++){
                uchar* img=destimage.ptr<uchar>(j);
                T* m = disp2.ptr<T>(j);
                T* dp = destdisp.ptr<T>(j);
                for(int i=BS;i<image.cols-BS;i++){
                    if(m[i]==0){
                        int count=0;
                        int d=0;
                        int r=0;
                        int g=0;
                        int b=0;
                        for(int l=-BS;l<=BS;l++){
                            T* dp2 = disp2.ptr<T>(j+l);
                            uchar* imageR = image2.ptr<uchar>(j+l);
                            for(int k=-BS;k<=BS;k++){
                                if(dp2[i+k]!=0){
                                    count++;
                                    d+=dp2[i+k];
                                    r+=imageR[3*(i+k)+0];
                                    g+=imageR[3*(i+k)+1];
                                    b+=imageR[3*(i+k)+2];
                                }
                            }
                        }
                        if(count!=0){
                            double div = 1.0/count;
                            dp[i]=d*div;
                            img[3*i+0]=r*div;
                            img[3*i+1]=g*div;
                            img[3*i+2]=b*div;
                        }
                    }
                }
            }
        }
    }

    template <class T>
    static void fillOcclusion_(Mat& src, T invalidvalue){
        int bb=1;
        const int MAX_LENGTH=src.cols*0.5;
        //#pragma omp parallel for
        for(int j=bb;j<src.rows-bb;j++){
            T* s = src.ptr<T>(j);
            //const T st = s[0];
            //const T ed = s[src.cols-1];
            s[0]=255;
            s[src.cols-1]=255;
            for(int i=0;i<src.cols;i++){
                if(s[i]<=invalidvalue){
                    int t=i;
                    do{
                        t++;
                        if(t>src.cols-1)break;
                    }while(s[t]<=invalidvalue);

                    const T dd = min(s[i-1],s[t]);
                    if(t-i>MAX_LENGTH){
                        for(int n=0;n<src.cols;n++){
                            s[n]=invalidvalue;
                        }
                    }
                    else{
                        for(;i<t;i++){
                            s[i]=dd;
                        }
                    }
                }
            }
        }
    }

    template <class T>
    static void fillOcclusionInv_(Mat& src, T invalidvalue){
        int bb=1;
        const int MAX_LENGTH=src.cols*0.8;
        //#pragma omp parallel for
        for(int j=bb;j<src.rows-bb;j++){
            T* s = src.ptr<T>(j);
            //const T st = s[0];
            //const T ed = s[src.cols-1];
            s[0]=0;
            s[src.cols-1]=0;
            for(int i=0;i<src.cols;i++){
                if(s[i]==invalidvalue){
                    int t=i;
                    do{
                        t++;
                        if(t>src.cols-1)break;
                    }while(s[t]==invalidvalue);

                    const T dd = max(s[i-1],s[t]);
                    if(t-i>MAX_LENGTH){
                        for(int n=0;n<src.cols;n++){
                            s[n]=invalidvalue;
                        }
                    }
                    else{
                        for(;i<t;i++){
                            s[i]=dd;
                        }
                    }
                }
            }
        }
    }

    /* Results */
    Mat disp3Dviewer;
    Mat disp3D;
    Mat disp3D_8U;
    Mat disp3D_BGR;


    Point3d viewpoint;
    Point3d lookatpoint;

    Mat dist;
    Mat Rotation;
    Mat t;

    Mat xyz;
    Mat depth;

    double step;
    bool isSub;
};

#endif // RECONSTRUCTION_3D_H
