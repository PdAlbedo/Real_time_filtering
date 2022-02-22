#ifndef __filter_h
#define __filter_h

#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;

int greyscale(Mat &src, Mat &dst);
int blur5x5(Mat &src, Mat &dst);
int sobelX3x3(Mat &src, Mat &dst );
int sobelY3x3(Mat &src, Mat &dst );
int magnitude(Mat &sx, Mat &sy, Mat &dst);
int blurQuantize(Mat &src, Mat &dst, int levels);
int cartoon(Mat &src, Mat&dst, int levels, int magThreshold);
int brightnesscontrast(Mat &src, Mat &dst, double alpha, int beta);
int sharp(Mat &src, Mat &dst);
int bigMess(Mat &src, Mat &dst, int quantize_lv, int threshold);

#endif
