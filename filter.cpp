/*
Xichen Liu

CS5330 CV Project-1
Image manipulation functions
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include "filter.h"

using namespace cv;
using namespace std;


int greyscale(Mat &src, Mat &dst) {

    if (!src.data) {
        printf("Error loading src image \n");
        return -1;
    }

    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            // Vec3b src_color = src.at<Vec3b>(i, j);
            // Scalar dst_color = 0.114 * src_color[0] + 0.587 * src_color[1] + 0.299 * src_color[2];
            dst.at<uchar>(i, j) = 0.114 * src.at<Vec3b>(i, j)[0] + 0.587 * src.at<Vec3b>(i, j)[1] + 0.299 * src.at<Vec3b>(i, j)[2];
        }
    }
    return 0;
}


int blur5x5(Mat &src, Mat &dst) {

    Mat mid = dst.clone();

    // generate 1-D Gaussian filter
    double gaussian_filter[5] = {1, 2, 4, 2, 1};
    for(int i = 0; i < 5; i++) {
        gaussian_filter[i] /= 10;
    }

    // the outer two lines will not be covered by filter
    int rows = src.rows - 2;
    int cols = src.cols - 2;

    // go through the pixels with the filter horizontally.
    // since the filter is applied horizontally, we can start at the first row
    for (int i = 0; i < rows; i++) {
        // skip the first two lines
        for (int j = 2; j < cols; j++) {
            // stores the bgr value
            double color[3] = {0};
            for (int k = -2; k <= 2; k++) {
                // color stores the current value which is calculated value from the 5 pixels and the filter
                Vec3b bgr = src.at<Vec3b>(i, j + k);
                color[0] += gaussian_filter[2 + k] * bgr[0];
                color[1] += gaussian_filter[2 + k] * bgr[1];
                color[2] += gaussian_filter[2 + k] * bgr[2];
            }
            for (int k = 0; k < 3; k++) {
                if (color[k] < 0) color[k] = 0;
                else if (color[k] > 255) color[k] = 255;
            }
            Vec3b bgr = {static_cast<uchar>(color[0]), static_cast<uchar>(color[1]), static_cast<uchar>(color[2])};
            mid.at<Vec3b>(i, j) = bgr;
        }
    }

    // go through the pixels with the filter vertically.
    // skip the first two lines
    for (int i = 2; i < rows; i++) {
        // since the filter is applied horizontally, we can start at the first column
        for (int j = 0; j < cols; j++) {
            // stores the bgr value
            double color[3] = {0};
            for (int k = -2; k <= 2; k++) {
                // color stores the current value which is calculated value from the 5 pixels and the filter
                Vec3b bgr = mid.at<Vec3b>(i + k, j);
                color[0] += gaussian_filter[2 + k] * bgr[0];
                color[1] += gaussian_filter[2 + k] * bgr[1];
                color[2] += gaussian_filter[2 + k] * bgr[2];
            }
            for (int k = 0; k < 3; k++) {
                if (color[k] < 0) color[k] = 0;
                else if (color[k] > 255) color[k] = 255;
            }
            Vec3b bgr = {static_cast<uchar>(color[0]), static_cast<uchar>(color[1]), static_cast<uchar>(color[2])};
            dst.at<Vec3b>(i, j) = bgr;
        }
    }

    return 0;
}


int sobelX3x3(Mat &src, Mat &dst) {

    // generate 1-D sobel filter
    double sobels_x_filter_v[3] = {1, 2, 1};
    double sobels_x_filter_h[3] = {-1, 0, 1};

    for(int i = 0; i < 3; i++) {
        sobels_x_filter_v[i] /= 4;
    }

    // the outer line will not be covered by filter
    int rows = src.rows - 1;
    int cols = src.cols - 1;

    // mid stores the intermidate status after the first separated filter is applied
    Mat mid = dst.clone();

    // go through the pixels with the filter horizontally.
    // since the filter is applied horizontally, we can start from the first row
    for (int i = 0; i < rows; i++) {

        // skip the first line
        for (int j = 1; j < cols; j++) {

            // stores the bgr value
            double color[3] = {0};

            for (int k = -1; k <= 1; k++) {
                Vec3b bgr = src.at<Vec3b>(i, j + k);
                color[0] += sobels_x_filter_h[1 + k] * bgr[0];
                color[1] += sobels_x_filter_h[1 + k] * bgr[1];
                color[2] += sobels_x_filter_h[1 + k] * bgr[2];
            }
            
            Vec3s bgr = {static_cast<short>(color[0]), static_cast<short>(color[1]), static_cast<short>(color[2])};
            mid.at<Vec3s>(i, j) = bgr;
        }
    }

    // go through the pixels with the filter vertically.
    // skip the first line
    for (int i = 1; i < rows; i++) {
        // since the filter is applied horizontally, we can start from the first column
        for (int j = 0; j < cols; j++) {
            // stores the bgr value
            double color[3] = {0};

            for (int k = -1; k <= 1; k++) {
                Vec3b bgr = mid.at<Vec3s>(i + k, j);
                color[0] += sobels_x_filter_v[1 + k] * bgr[0];
                color[1] += sobels_x_filter_v[1 + k] * bgr[1];
                color[2] += sobels_x_filter_v[1 + k] * bgr[2];
            }

            Vec3s bgr = {static_cast<short>(color[0]), static_cast<short>(color[1]), static_cast<short>(color[2])};
            dst.at<Vec3s>(i, j) = bgr;
        }
    }

    return 0;
}


int sobelY3x3(Mat &src, Mat &dst) {

    // generate 1-D sobel filter
    double sobels_y_filter_v[3] = {1, 0, -1};
    double sobels_y_filter_h[3] = {1, 2, 1};

    for(int i = 0; i < 3; i++) {
        sobels_y_filter_h[i] /= 4;
    }

    // the outer line will not be covered by filter
    int rows = src.rows - 1;
    int cols = src.cols - 1;

    Mat mid = dst.clone();

    // go through the pixels with the filter horizontally.
    // since the filter is applied horizontally, we can start from the first row
    for (int i = 0; i < rows; i++) {

        // skip the first line
        for (int j = 1; j < cols; j++) {

            // stores the bgr value
            double color[3] = {0};

            for (int k = -1; k <= 1; k++) {
                Vec3b bgr = src.at<Vec3b>(i, j + k);
                color[0] += sobels_y_filter_h[1 + k] * bgr[0];
                color[1] += sobels_y_filter_h[1 + k] * bgr[1];
                color[2] += sobels_y_filter_h[1 + k] * bgr[2];
            }

            Vec3s bgr = {static_cast<short>(color[0]), static_cast<short>(color[1]), static_cast<short>(color[2])};
            mid.at<Vec3s>(i, j) = bgr;
        }
    }

    // go through the pixels with the filter vertically.
    // skip the first line
    for (int i = 1; i < rows; i++) {

        // since the filter is applied horizontally, we can start from the first column
        for (int j = 0; j < cols; j++) {

            // stores the bgr value
            double color[3] = {0};

            for (int k = -1; k <= 1; k++) {
                Vec3b bgr = mid.at<Vec3s>(i + k, j);
                color[0] += sobels_y_filter_v[1 + k] * bgr[0];
                color[1] += sobels_y_filter_v[1 + k] * bgr[1];
                color[2] += sobels_y_filter_v[1 + k] * bgr[2];
            }
            
            Vec3s bgr = {static_cast<short>(color[0]), static_cast<short>(color[1]), static_cast<short>(color[2])};
            dst.at<Vec3s>(i, j) = bgr;
        }
    }

    return 0;
}


int magnitude(Mat &sx, Mat &sy, Mat &dst) {

    int rows = sx.rows;
    int cols = sx.cols;

    for (int i = 0; i < rows; i++) {

        for (int j = 0; j < cols; j++) {

            double color[3] = {0};

            Vec3s bgr_x = sx.at<Vec3s>(i, j);
            Vec3s bgr_y = sy.at<Vec3s>(i, j);
            color[0] += sqrt(pow(bgr_x[0], 2) + pow(bgr_y[0], 2));
            color[1] += sqrt(pow(bgr_x[1], 2) + pow(bgr_y[1], 2));
            color[2] += sqrt(pow(bgr_x[2], 2) + pow(bgr_y[2], 2));

            Vec3s bgr = {static_cast<short>(color[0]), static_cast<short>(color[1]), static_cast<short>(color[2])};
            dst.at<Vec3s>(i, j) = bgr;
        }
    }

    return 0;
}


int blurQuantize(Mat &src, Mat &dst, int levels){

    Mat mid = dst.clone();
    blur5x5(src, mid);

    int rows = mid.rows;
    int cols = mid.cols;

    int b = 255/levels;

    for (int i = 0; i < rows; i++) {

        for (int j = 0; j < cols; j++) {

            double color[3] = {0};

            // get a cofficient by current color/b
            // and use the cofficient*b to calculate the quantized colors
            // b can be considered as the interval
            Vec3b bgr_src = mid.at<Vec3b>(i, j);
            int temp;
            temp = bgr_src[0]/b;
            color[0] = temp * b;
            temp = bgr_src[1]/b;
            color[1] = temp * b;
            temp = bgr_src[2]/b;
            color[2] = temp * b;

            Vec3b bgr = {static_cast<uchar>(color[0]), static_cast<uchar>(color[1]), static_cast<uchar>(color[2])};
            dst.at<Vec3b>(i, j) = bgr;
        }
    }

    return 0;
}


int cartoon(Mat &src, Mat&dst, int levels, int magThreshold) {

    Mat mag = dst.clone();
    Mat quantized = dst.clone();
    Mat sx = dst.clone();
    Mat sy = dst.clone();

    sobelX3x3(src, sx);
    sobelY3x3(src, sy);

    magnitude(sx, sy, mag);
    convertScaleAbs(mag, mag);
    
    convertScaleAbs(dst, dst);
    blurQuantize(src, dst, levels);

    int rows = src.rows;
    int cols = src.cols;

    for (int i = 0; i < rows; i++) {

        for (int j = 0; j < cols; j++) {

            double color[3] = {0};

            Vec3b bgr_src = mag.at<Vec3b>(i, j);
            if ((bgr_src[0] + bgr_src[1] + bgr_src[2])/3 > magThreshold) {
                
                Vec3b bgr = {0, 0, 0};
                dst.at<Vec3b>(i, j) = bgr;
            }
        }
    }

    return 0;
}


int brightnesscontrast(Mat &src, Mat &dst, double alpha, int beta) {
    
    int rows = src.rows;
    int cols = src.cols;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < src.channels(); k++) {
                dst.at<Vec3b>(i, j)[k] = saturate_cast<uchar>(alpha*src.at<Vec3b>(i, j)[k] + beta);
            }
        }
    }

    return 0;
}


int sharp(Mat &src, Mat &dst) {

    // generate 1-D Gaussian filter
    double sharpen_filter[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
    // for(int i = 0; i < 3; i++) {
    //     for (int j = 0; j < 3; j++) {
    //         sharpen_filter[i][j] /= 2;
    //     }
    // }

    // the outer line will not be covered by filter
    int rows = src.rows - 1;
    int cols = src.cols - 1;

    // go through the pixels with the filter horizontally.
    // since the filter is applied horizontally, we can start at the first row
    for (int i = 1; i < rows; i++) {
        // skip the first two lines
        for (int j = 1; j < cols; j++) {
            // stores the bgr value
            double color[3] = {0};
            for (int k = -1; k <= 1; k++) {
                for (int p = -1; p <= 1; p++) {
                    Vec3b bgr = src.at<Vec3b>(i + k, j + p);
                    color[0] += sharpen_filter[1 + k][1 + p] * bgr[0];
                    color[1] += sharpen_filter[1 + k][1 + p] * bgr[1];
                    color[2] += sharpen_filter[1 + k][1 + p] * bgr[2];
                }
            }
            Vec3s bgr = {static_cast<short>(color[0]), static_cast<short>(color[1]), static_cast<short>(color[2])};
            dst.at<Vec3s>(i, j) = bgr;
        }
    }

    return 0;
}


int bigMess(Mat &src, Mat &dst, int quantize_lv, int threshold) {

    // Mat sharpened = dst.clone();
    // sharp(src, sharpened);
    Mat blurred = Mat::zeros(dst.rows, dst.cols, CV_8UC3);
    Mat cartoonized = dst.clone();
    Mat sharpened = dst.clone();
    // convertScaleAbs(dst, blurred_1);
    blur5x5(src, blurred);
    cartoon (blurred, cartoonized, quantize_lv, threshold);
    sharp(cartoonized, sharpened);
    convertScaleAbs(sharpened, sharpened);
    convertScaleAbs(dst, dst);
    blurQuantize(sharpened, dst, quantize_lv);

    return 0;
}