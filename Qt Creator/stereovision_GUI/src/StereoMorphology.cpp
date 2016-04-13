/*
 * StereoMorphology.cpp
 *
 *  Created on: Feb 29, 2016
 *      Author: nicolasrosa
 */

/* Libraries */
#include "inc/StereoMorphology.h"

/* Constructor and Destructor */
StereoMorphology::StereoMorphology(){
    erosionElement  = getStructuringElement(MORPH_RECT,Size(2*EROSION_SIZE +1, 2*EROSION_SIZE+1 ),Point(EROSION_SIZE,  EROSION_SIZE ));
    dilationElement = getStructuringElement(MORPH_RECT,Size(2*DILATION_SIZE+1, 2*DILATION_SIZE+1),Point(DILATION_SIZE, DILATION_SIZE));
}


StereoMorphology::~StereoMorphology(){}


/* Harris - GlobalVariables */
//TODO: Alocar as variaveis globais dentro da StereoMorphology class.
Mat src_harris, dst_harris,result_harris;
Mat Harris_result;
int thresh = 200;
int max_thresh = 255;
const string source_window = "Source image";
const string corners_window = "Corners detected";

/** @function cornerHarris_demo */
void cornerHarris_demo(int,void*)
{
  //Mat src_gray;
  //cvtColor( src_harris, src_gray, CV_BGR2GRAY );


  Mat dst_norm, dst_norm_scaled;
  dst_harris = Mat::zeros( src_harris.size(), CV_32FC1 );

  /// Detector parameters
  int blockSize = 2;
  int apertureSize = 3;
  double k = 0.04;

  /// Detecting corners
  cornerHarris( src_harris, dst_harris, blockSize, apertureSize, k, BORDER_DEFAULT );
  //imshow("dst",dst_harris);

  /// Normalizing
  normalize( dst_harris, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
  convertScaleAbs( dst_norm, dst_norm_scaled );
  //imshow("dst_norm",dst_norm_scaled);

  src_harris.copyTo(result_harris);
  /// Drawing a circle around corners
  for( int j = 0; j < dst_norm.rows ; j++ )
     { for( int i = 0; i < dst_norm.cols; i++ )
          {
            if( (int) dst_norm.at<float>(j,i) > thresh )
              {
               //circle( dst_norm_scaled, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
               circle( result_harris, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
            }
          }
     }
  /// Showing the result
  namedWindow( corners_window, CV_WINDOW_AUTOSIZE );
  //imshow( corners_window, dst_norm_scaled );
  imshow( corners_window, result_harris );
}


void StereoMorphology::applyMorphology(Mat src, Mat cameraFeedL,bool isTrackingObjects,int inputType,bool enableLightingNoiseDetector){
    /* Local Variables */
    Mat srcFiltered;

    //! Near Object Detection !//
    /* Prefiltering */
    apply_preFiltering(&src,&srcFiltered);

    /* Threshold */
    int T_Otsu = threshold(srcFiltered, imgThreshold, 0, 255, THRESH_BINARY | THRESH_OTSU);
    imgThreshold.copyTo(imgThresholdDraw);
    putText(imgThresholdDraw,"T: "+utils.intToString(T_Otsu),Point(0,25),1,1,Scalar(255,255,255),2);

    /* Lighting Noise Detector*/
    //TODO: Solve Lighting Noise Problem
    if(enableLightingNoiseDetector){
        apply_lightingNoiseDetector();
    }

    /* Tracking Object */
    if(isTrackingObjects){
        cameraFeedL.copyTo(trackingView);
        if(inputType == StereoCalib::VideoFile){
           trackObject.trackFilteredObject(x,y,imgThreshold,trackingView);
        }
    }

    //! Testing Methods !//
    imshow("RAW",src);
    imshow("Filtered", srcFiltered);

    //! Testing Blobs !//
    // Setup SimpleBlobDetector parameters.
    SimpleBlobDetector::Params params;

    // Change thresholds
    params.minThreshold = 0;
    params.maxThreshold = 255;

    // Filter by Area.
    params.filterByArea = true;
    params.minArea = 10;

    // Filter by Circularity
    params.filterByCircularity = false;
    params.minCircularity = 0.1;

    // Filter by Convexity
    params.filterByConvexity = false;
    params.minConvexity = 0.87;

    // Filter by Inertia
    params.filterByInertia = true;
    params.minInertiaRatio = 0.01;


    // Storage for blobs
    vector<KeyPoint> keypoints;

    // Set up detector with params
    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

    // Detect blobs
    detector->detect( srcFiltered, keypoints);

    // Draw detected blobs as red circles.
    // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
    // the size of the circle corresponds to the size of blob

    Mat im_with_keypoints;
    drawKeypoints( srcFiltered, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

    // Show blobs
    imshow("keypoints", im_with_keypoints );

    /* Harris - Corner Detector */
    //apply_harris(src);

    /* Watershed */
    //cameraFeedL.copyTo(src);
    //apply_watershed(src);

    //FIXME: Distribuir ao longo do código
    // Output
    //imshow("Eroded Image",imgE);
    //imshow("Eroded Image BGR",imgEBGR);
    //imshow("Eroded+Dilated Image",imgED);
    //imshow("Eroded+Dilated Image BGR",imgEDBGR);
    //imshow("Eroded+Dilated+Median Image",imgEDMedian);
    //imshow("Eroded+Dilated+Median Image BGR",imgEDMedianBGR);
    //imshow("Tracking Object",trackingView);
    //imshow("Thresholded Image",imgThreshold);
}

void StereoMorphology::apply_preFiltering(Mat *src,Mat *dst){
    /* Parcial Results */
    Mat imgE       ,imgEBGR;
    Mat imgED      ,imgEDBGR;
    Mat imgEDMedian,imgEDMedianBGR;

    /* Erosion and Dilation */
    // Take out spurious noise
    erode(*src,imgE,erosionElement);
    dilate(imgE,imgED,erosionElement);

    /* Gaussian Filter */
    //GaussianBlur(imgED,imgEDMedian,Size(3,3),0,0);

    /* Median Filter */
    medianBlur(imgED,imgEDMedian,5);

    /* Result */
    imgEDMedian.copyTo(*dst);

    /* Get the BGR images */
    //applyColorMap(imgEDMedian,imgEDMedianBGR, COLORMAP_JET);
    //applyColorMap(imgE,imgEBGR, COLORMAP_JET);
    //applyColorMap(imgED,imgEDBGR, COLORMAP_JET);
}

void StereoMorphology::apply_lightingNoiseDetector(){
    static Mat lastImgThreshold;

    int nPixels = sum(imgThreshold)[0]/255;
    int nTotal = imgThreshold.total();

    //	cout << "Number of Pixels:" << nPixels << endl;
    //	cout << "Ratio is: " << ((float)nPixels)/nTotal << endl << endl;

    if((((float)nPixels)/nTotal)>0.5){
        //		sleep(1);
        //		cout << "Lighting Noise!!!" << endl;
        //		cout << "Number of Pixels:" << nPixels << endl;
        //		cout << "Ratio is: " << ((float)nPixels)/nTotal << endl << endl;

        // Invalidates the last frame
        imgThreshold = lastImgThreshold;
    }else{
        // Saves the last valid frame
        lastImgThreshold=imgThreshold;
        //lastThresholdSum = CurrentThresholdSum;
    }
}

void StereoMorphology::apply_harris(Mat src){
    src.copyTo(src_harris);
    namedWindow( source_window, CV_WINDOW_AUTOSIZE );
    createTrackbar( "Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo );
    cornerHarris_demo( 0, 0 );
    imshow( source_window, src );
}

/**
 * @function Watershed_and_Distance_Transform.cpp
 * @brief Sample code showing how to segment overlapping objects using Laplacian filtering, in addition to Watershed and Distance Transformation
 * @author OpenCV Team
 */
void StereoMorphology::apply_watershed(Mat src){
//! [load_image]
    applyColorMap(src,src, COLORMAP_JET); //disp_BGR
    imshow("Source Image", src);
//! [load_image]

//! [black_bg]
    // Change the background from white to black, since that will help later to extract
    // better results during the use of Distance Transform
    for( int x = 0; x < src.rows; x++ ) {
      for( int y = 0; y < src.cols; y++ ) {
          if ( src.at<Vec3b>(x, y) == Vec3b(255,255,255) ) {
            src.at<Vec3b>(x, y)[0] = 0;
            src.at<Vec3b>(x, y)[1] = 0;
            src.at<Vec3b>(x, y)[2] = 0;
          }
        }
    }

    // Show output image
    imshow("Black Background Image", src);
//! [black_bg]

//! [sharp]
    // Create a kernel that we will use for accuting/sharpening our image
    Mat kernel = (Mat_<float>(3,3) <<
            1,  1, 1,
            1, -8, 1,
            1,  1, 1); // an approximation of second derivative, a quite strong kernel

    // do the laplacian filtering as it is
    // well, we need to convert everything in something more deeper then CV_8U
    // because the kernel has some negative values,
    // and we can expect in general to have a Laplacian image with negative values
    // BUT a 8bits unsigned int (the one we are working with) can contain values from 0 to 255
    // so the possible negative number will be truncated
    Mat imgLaplacian;
    Mat sharp = src; // copy source image to another temporary one
    filter2D(sharp, imgLaplacian, CV_32F, kernel);
    src.convertTo(sharp, CV_32F);
    Mat imgResult = sharp - imgLaplacian;

    // convert back to 8bits gray scale
    imgResult.convertTo(imgResult, CV_8UC3);
    imgLaplacian.convertTo(imgLaplacian, CV_8UC3);

    // imshow( "Laplace Filtered Image", imgLaplacian );
    imshow( "New Sharped Image", imgResult );
//! [sharp]

    src = imgResult; // copy back

//! [bin]
    // Create binary image from source image
    Mat bw;
    cvtColor(src, bw, CV_BGR2GRAY);
    threshold(bw, bw, 40, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    imshow("Binary Image", bw);
//! [bin]

//! [dist]
    // Perform the distance transform algorithm
    Mat dist;
    distanceTransform(bw, dist, CV_DIST_L2, 3);

    // Normalize the distance image for range = {0.0, 1.0}
    // so we can visualize and threshold it
    normalize(dist, dist, 0, 1., NORM_MINMAX);
    imshow("Distance Transform Image", dist);
//! [dist]

//! [peaks]
    // Threshold to obtain the peaks
    // This will be the markers for the foreground objects
    threshold(dist, dist, .4, 1., CV_THRESH_BINARY);

    // Dilate a bit the dist image
    Mat kernel1 = Mat::ones(3, 3, CV_8UC1);
    dilate(dist, dist, kernel1);
    imshow("Peaks", dist);
//! [peaks]

//! [seeds]
    // Create the CV_8U version of the distance image
    // It is needed for findContours()
    Mat dist_8u;
    dist.convertTo(dist_8u, CV_8U);

    // Find total markers
    vector<vector<Point> > contours;
    findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    // Create the marker image for the watershed algorithm
    Mat markers = Mat::zeros(dist.size(), CV_32SC1);

    // Draw the foreground markers
    for (size_t i = 0; i < contours.size(); i++)
        drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i)+1), -1);

    // Draw the background marker
    circle(markers, Point(5,5), 3, CV_RGB(255,255,255), -1);
    imshow("Markers", markers*10000);
//! [seeds]

//! [watershed]
    // Perform the watershed algorithm
    watershed(src, markers);

    Mat mark = Mat::zeros(markers.size(), CV_8UC1);
    markers.convertTo(mark, CV_8UC1);
    bitwise_not(mark, mark);
//    imshow("Markers_v2", mark); // uncomment this if you want to see how the mark
                                  // image looks like at that point

    // Generate random colors
    vector<Vec3b> colors;
    for (size_t i = 0; i < contours.size(); i++)
    {
        int b = theRNG().uniform(0, 255);
        int g = theRNG().uniform(0, 255);
        int r = theRNG().uniform(0, 255);

        colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }

    // Create the result image
    Mat dst = Mat::zeros(markers.size(), CV_8UC3);

    // Fill labeled objects with random colors
    for (int i = 0; i < markers.rows; i++)
    {
        for (int j = 0; j < markers.cols; j++)
        {
            int index = markers.at<int>(i,j);
            if (index > 0 && index <= static_cast<int>(contours.size()))
                dst.at<Vec3b>(i,j) = colors[index-1];
            else
                dst.at<Vec3b>(i,j) = Vec3b(0,0,0);
        }
    }

    // Visualize the final image
    imshow("Final Result", dst);
//! [watershed]
}

void StereoMorphology::Disp_diff(Mat disp8U,Mat disp8U_last,Mat disp8U_diff){
    absdiff(disp8U,disp8U_last,disp8U_diff);
    Mat disp_diff_th;
    threshold(disp8U_diff,disp_diff_th, 10, 255, THRESH_BINARY);

    Mat disp_sum;
    addWeighted(disp8U, 1, disp_diff_th, 1, 0.0, disp_sum);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat disp_countours = Mat::zeros(disp_diff_th.rows, disp_diff_th.cols, CV_8UC3);
    Scalar white = CV_RGB( 255, 255, 255 );
    //                Scalar color( rand()&255, rand()&255, rand()&255 );

    findContours( disp_diff_th, contours, hierarchy,
                  CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

    // iterate through all the top-level contours,
    // draw each connected component with its own random color
    int idx = 0;
    for( ; idx >= 0; idx = hierarchy[idx][0] )
    {
        //drawContours( disp_countours, contours, idx, color, CV_FILLED, 8, hierarchy );
        drawContours( disp_countours, contours, idx, white, CV_FILLED);
    }

    namedWindow( "Components", 1 );


    cv::Mat holes=disp_diff_th.clone();
    cv::floodFill(holes,cv::Point2i(0,0),cv::Scalar(1));
    for(int i=0;i<disp_diff_th.rows*disp_diff_th.cols;i++)                {
        if(holes.data[i]==0)
            disp_diff_th.data[i]=1;
    }

    imshow( "holes", holes );
    imshow( "Components", disp_countours );


    //imshow("Disp",stereo->disp.disp_8U);
    //imshow("Disp_last",stereo->disp.disp_8U_last);
    imshow("Disp_diff",disp8U_diff);
    imshow("Disp_diff_th",disp_diff_th);
    imshow("Disp_sum",disp_sum);
}
