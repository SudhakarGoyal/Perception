#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <vector>
#include <unistd.h>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;
Mat image = imread("binary.png");


void distance_transform()
{
    Mat gray;
    cvtColor(image, gray,CV_BGR2GRAY );

    for(int i = 0; i < gray.cols; i++)
    {
        for(int j = 0; j < gray.rows; j++)
        {
            if(gray.at<uchar>(j,i) > 200)
                gray.at<uchar>(j,i) = 255;
            else
                gray.at<uchar>(j,i) = 0;
        }
    }
    Mat final_pass = gray.clone();
    Mat temp = gray.clone();

    for(int i = 0; i < gray.cols; i++)
    {
        for(int j = 0; j < gray.rows; j++)
        {
            if(gray.at<uchar>(j,i) == 255)
                temp.at<uchar>(j,i) = 1;
            else
                temp.at<uchar>(j,i) = 0;

        }
    }

    for(int i = 1; i < temp.rows; i++)
    {
        for(int j = 1; j < temp.cols; j++)
        {
            temp.at<uchar>(i,j) = (min(temp.at<uchar>(i,j-1), temp.at<uchar>(i-1,j)) + 1);
        }
    }

    Mat forward_pass = temp.clone();
    temp = gray.clone();

    for(int i = temp.cols - 1; i > 0; i--)
    {
        for(int j = temp.rows -1; j > 0; j--)
        {
            temp.at<uchar>(j,i) = (min(temp.at<uchar>(j+1,i), temp.at<uchar>(j,i+1)) + 1);
        }
    }
    Mat backward_pass = temp.clone();

    for(int i = 0; i < temp.cols; i++)
    {
        for(int j = 0; j <temp.rows; j++)
        {
            final_pass.at<uchar>(j,i) = min(forward_pass.at<uchar>(j,i),backward_pass.at<uchar>(j,i));
        }
    }

    namedWindow("binary", WINDOW_AUTOSIZE );
    imshow("binary", final_pass);
}


int main()
{

//    Mat gray = Mat(image.rows, image.cols, CV_8UC1);
//    for(int i = 0; i < image.cols; i++)
//    {
//        for(int j = 0; j < image.rows; j++)
//        {
//            gray.at<uchar>(j,i) = saturate_cast<uchar>(floor((image.at<Vec3b>(j,i)[0] + image.at<Vec3b>(j,i)[1] + image.at<Vec3b>(j,i)[2])/3));
//        }
//    }
//    namedWindow("gray", WINDOW_AUTOSIZE);
//    imshow("gray", gray);

    distance_transform();
    cv::waitKey(0);
    return 0;




}








