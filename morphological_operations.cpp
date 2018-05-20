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

using namespace std;
using namespace cv;

const int alpha_slider_max = 100;
int alpha_slider;
int brightness_slider;
int contrast_slider;
int gamma_slider;
const int brightness_slider_max = 100;
const int contrast_slider_max = 300;
const int gamma_slider_max = 100;

double alpha;
double beta;
Mat src_gray;
Mat grad;

Mat image = imread("lanes.jpg");

void cumulative_distribution()
{
    Mat new_image = image.clone();
    float cumulative_sum[3];
    for(int i = 0; i < image.cols; i++)
    {
        for(int j = 0 ; j < image.rows; j++)
        {
            for(int k =0; k < 3; k++)
            {
                cumulative_sum[k]+=image.at<Vec3b>(j,i)[k];
            }
        }
    }

    float cumulative_dis[3];
    for(int i = 0; i < 3; i++)
    {
        cumulative_dis[i] = cumulative_sum[i]/(image.rows*image.cols);

    }
    float alpha = 0.8;
    for(int i = 0 ; i < image.cols; i++)
    {
        for(int j = 0 ; j < image.rows; j++)
        {
            for(int k = 0; k < 3; k++)
                new_image.at<Vec3b>(j,i)[k] = saturate_cast<uchar>(alpha*cumulative_dis[k] - (1-alpha)*image.at<Vec3b>(j,i)[k]);
        }
    }

    namedWindow("cum",WINDOW_AUTOSIZE);
    imshow("cum", new_image);


}

Mat average_smoothing(Mat img)
{
    Mat new_img = img.clone();
    Mat kernel = Mat::ones(Size(3,3), CV_32FC1);
    for(int k = 2; k < img.cols-2;k++)
    {
        for(int l = 2; l < img.rows-2; l++)
        {
            float sum_r = 0,sum_g = 0,sum_b = 0;
            for(int i = -2; i <=2; i++)
            {
                for(int j = -2; j<=2; j++)
                {
                    sum_b+= img.at<Vec3b>(l+j, k+i)[0] ;
                    sum_g+= img.at<Vec3b>(l+j, k+i)[1];
                    sum_r+= img.at<Vec3b>(l+j, k+i)[2];
                }
            }
            //            cout<<"sum "<<sum<<endl;
            new_img.at<Vec3b>(l,k)[0] = (sum_b/25);
            new_img.at<Vec3b>(l,k)[1] = (sum_g/25);
            new_img.at<Vec3b>(l,k)[2] = (sum_r/25);
        }
    }

    return new_img;


}

void gamma_trackbar(int, void*)
{

    float gm = double(gamma_slider)/10;
    Mat new_img =image.clone();

    for(int i =1; i < image.cols ;i++)
    {
        for(int j = 1; j < image.rows;j++)
        {
            for(int k= 0; k < 3;k++)
            {
                new_img.at<Vec3b>(j,i)[k] = saturate_cast<uchar>(pow(new_img.at<Vec3b>(j,i)[k],gm));
            }
        }

    }
    imshow("gamma", new_img);
}

void brightness_trackbar(int, void*)
{

    float br = double(brightness_slider);
    Mat new_img =image.clone();

    for(int i =1; i < image.cols ;i++)
    {
        for(int j = 1; j < image.rows;j++)
        {
            for(int k= 0; k < 3;k++)
            {
                new_img.at<Vec3b>(j,i)[k] = saturate_cast<uchar>(new_img.at<Vec3b>(j,i)[k] + br);

            }
        }

    }
    imshow("brightness", new_img);
}

void contrast_trackbar(int, void*)
{

    float cr = double(contrast_slider)/100;
    Mat new_img =image.clone();

    for(int i =1; i < image.cols ;i++)
    {
        for(int j = 1; j < image.rows;j++)
        {
            for(int k= 0; k < 3;k++)
            {
                new_img.at<Vec3b>(j,i)[k] = saturate_cast<uchar>(cr*new_img.at<Vec3b>(j,i)[k]);
            }
        }

    }
    imshow("contrast", new_img);
}

void edge_trackbar( int, void* )
{
    alpha = (double) alpha_slider ;
    Mat new_img =src_gray.clone();

    for(int i =1; i < src_gray.cols ;i++)
    {

        for(int j = 1; j < src_gray.rows;j++)
        {
            float hor_grad, ver_grad;
            hor_grad = abs(-1*src_gray.at<uchar>(j-1,i) + 2*src_gray.at<uchar>(j,i) - src_gray.at<uchar>(j+1,i));
            ver_grad  = abs(-1*src_gray.at<uchar>(j,i-1) + 2*src_gray.at<uchar>(j,i) - src_gray.at<uchar>(j,i+1));
            hor_grad = pow(hor_grad,2);
            ver_grad = pow(ver_grad,2);

            if(sqrt(hor_grad + ver_grad) > alpha /*&& atan2(sqrt(ver_grad), sqrt(hor_grad)) < 1*/)
            {
                new_img.at<uchar>(j,i) = src_gray.at<uchar>(j,i);
            }
            else
                new_img.at<uchar>(j,i) = 0;
        }

    }

    cv::imshow("edge", new_img );

}
int main()
{
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;

    cvtColor( image, src_gray, CV_BGR2GRAY );

    cumulative_distribution();
    char edge[50];
    char brightness[50];
    char contrast[50];
    char gamma[50];

    namedWindow("edge",cv::WINDOW_AUTOSIZE);
    namedWindow("brightness",WINDOW_AUTOSIZE);
    namedWindow("contrast",WINDOW_AUTOSIZE);
    namedWindow("gamma",WINDOW_AUTOSIZE);


    createTrackbar( edge, "edge", &alpha_slider, alpha_slider_max, edge_trackbar );
    createTrackbar( brightness, "brightness", &brightness_slider, brightness_slider_max, brightness_trackbar );
    createTrackbar( contrast, "contrast", &contrast_slider, contrast_slider_max, contrast_trackbar );
    createTrackbar( gamma, "gamma", &gamma_slider, gamma_slider_max, gamma_trackbar );

    edge_trackbar( alpha_slider, 0 );
    brightness_trackbar(brightness_slider , 0);
    contrast_trackbar(contrast_slider , 0);
    gamma_trackbar(gamma_slider , 0);

    cv::waitKey(0);
    return 0;




}







