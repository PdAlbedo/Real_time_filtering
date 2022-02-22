/*
Xichen Liu

CS5330 CV Project-1
Create a file vidDisplay.cpp. Your main function should open a video channel, 
create a window, and then loop, capturing a new frame and displaying it each 
time through the loop. Have your program quit if the user types 'q'. Add the 
ability to save an image to a file if the user types 's'. The remaining tasks 
will involve adding capabilities to this program that are activated by different 
keystrokes. The OpenCV tutorials provide more information about how to capture 
live video from a camera. The following is skeleton code for capturing live 
from a camera.
*/

#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <videoio.hpp>
#include <time.h>
#include <string>
#include <filesystem>
#include "filter.h"

using namespace cv;
using namespace std;


int main(int argc, char *argv[]) {
        VideoCapture *capdev;

        // get times as file names
        time_t curr_time = time(NULL);
        struct tm *mt = localtime(&curr_time);
        char img_name[30];
        
        // flags that check whether the images need to be manipulated
        bool isManipulated = false;
        bool isGray = false;
        bool isGray_alt = false;
        bool isBlurred = false;
        bool isSobelX = false;
        bool isSobelY = false;
        bool isMagnitude = false;
        bool isQuantizes = false;
        bool isCartoonized = false;
        bool isBrightnContrast = false;
        bool isRecording = false;
        bool isSharpened = false;
        bool isBigMess = false;

        int quantizes_lv = 15;
        int threshold = 15;
        double alpha = 1.0;
        int beta = 0;

        int video_count = 1;
        VideoWriter video;

        string selection;
        string name_of_pic;

        while (true) {
                cout << "Select the one you want edit: video or picture: ";
                cin >> selection;
                cout << endl;

                // extension
                if (selection == "picture") {
                        cout << "Enter the name of picture: ";
                        cin >> name_of_pic;
                        cout << endl;

                        Mat image = imread(name_of_pic);
                        if(image.empty()) {
                                cout << "Could not read the image: " << name_of_pic << endl;
                                return 1;
                        }
                        
                        namedWindow("Image");
                        cout << "-----------------------------------------------------------------------" << endl
                                << "Press s to save the screenshot" << endl 
                                << "Press g or h to convert image to a grayscale and save the modified image" << endl
                                << "Press b to apply Gaussian Blur to image and save the modified image" << endl
                                << "Press x to apply Sobel X operator to image and save the modified image" << endl
                                << "Press y to apply Sobel Y operator to image and save the modified image" << endl
                                << "Press m to show magnitude image and save the modified image" << endl
                                << "Press l to show quantized image and save the modified image" << endl
                                << "Press c to cartoonize image and save the modified image" << endl
                                << "Press n to adjust brightness and constrast and save the modified image" << endl 
                                << "Press r to sharpen the image and save the modified image" << endl
                                << "Press q to quit" << endl
                                << "-----------------------------------------------------------------------" << endl;
                        
                        while (true) {

                                if (isManipulated) {
                                        
                                        if (isGray) {
                                                Mat gray_pic = Mat::zeros(image.rows, image.cols, CV_8UC1);
                                                cvtColor(image, gray_pic, COLOR_BGR2GRAY);
                                                imshow("Image", gray_pic);
                                        }
                                        else if (isGray_alt) {
                                                Mat gray_alt_pic = Mat::zeros(image.rows, image.cols, CV_8UC1);
                                                greyscale(image, gray_alt_pic);
                                                imshow("Image", gray_alt_pic);      
                                        }
                                        else if (isBlurred) {
                                                Mat blurred_pic = Mat::zeros(image.rows, image.cols, CV_8UC3);
                                                blur5x5(image, blurred_pic);
                                                imshow("Image", blurred_pic);
                                        }
                                        else if (isSobelX) {
                                                Mat sobelx_pic = Mat::zeros(image.rows, image.cols, CV_16SC3);
                                                sobelX3x3(image, sobelx_pic);
                                                convertScaleAbs(sobelx_pic, sobelx_pic);
                                                imshow("Image", sobelx_pic);
                                        }
                                        else if (isSobelY) {
                                                Mat sobely_pic = Mat::zeros(image.rows, image.cols, CV_16SC3);
                                                sobelY3x3(image, sobely_pic);
                                                convertScaleAbs(sobely_pic, sobely_pic);
                                                imshow("Image", sobely_pic);
                                        }
                                        else if (isMagnitude) {
                                                Mat sobelx_pic = Mat::zeros(image.rows, image.cols, CV_16SC3);
                                                sobelX3x3(image, sobelx_pic);
                                                Mat sobely_pic = Mat::zeros(image.rows, image.cols, CV_16SC3);
                                                sobelY3x3(image, sobely_pic);
                                                Mat magnitude_pic = Mat::zeros(image.rows, image.cols, CV_16SC3);
                                                magnitude(sobelx_pic, sobely_pic, magnitude_pic);
                                                convertScaleAbs(magnitude_pic, magnitude_pic);
                                                imshow("Image", magnitude_pic);
                                        }
                                        else if (isQuantizes) {
                                                Mat quantize_pic = Mat::zeros(image.rows, image.cols, CV_8UC3);
                                                blurQuantize(image, quantize_pic, quantizes_lv);
                                                imshow("Image", quantize_pic);
                                        }
                                        else if (isCartoonized) {
                                                Mat cartoonized_pic = Mat::zeros(image.rows, image.cols, CV_16SC3);
                                                cartoon(image, cartoonized_pic, quantizes_lv, threshold);
                                                convertScaleAbs(cartoonized_pic, cartoonized_pic);
                                                imshow("Image", cartoonized_pic);
                                        }
                                        else if (isBrightnContrast) {
                                                Mat bright_contrast_pic = Mat::zeros(image.rows, image.cols, CV_8UC3);
                                                brightnesscontrast(image, bright_contrast_pic, alpha, beta);
                                                imshow("Image", bright_contrast_pic);
                                        }
                                        else if (isSharpened) {
                                                Mat sharpened_pic = Mat::zeros(image.rows, image.cols, CV_16SC3);
                                                sharp(image, sharpened_pic);
                                                convertScaleAbs(sharpened_pic, sharpened_pic);
                                                imshow("Image", sharpened_pic);
                                        }
                                        else if (isBigMess) {
                                                Mat big_mess = Mat::zeros(image.rows, image.cols, CV_16SC3);
                                                bigMess(image, big_mess, quantizes_lv, threshold);
                                                imshow("Video", big_mess);
                                        }
                                }
                                else {
                                        imshow("Image", image);
                                }

                                // see if there is a waiting keystroke
                                char key = waitKey(10);
                                
                                curr_time = time(NULL);
                                mt = localtime(& curr_time);
                                
                                if( key == 'q') {
                                        break;
                                }
                                // save the frame when press 's'
                                else if (key == 's') {
                                        sprintf(img_name, "%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, image);
                                }
                                // convert image to a grayscale image when press 'g'
                                else if (key == 'g') {
                                        isGray = true;
                                        isManipulated = true;
                                        isGray_alt = false;
                                        isBlurred = false;
                                        isSobelX = false;
                                        isSobelY = false;
                                        isMagnitude = false;
                                        isQuantizes = false;
                                        isCartoonized = false;
                                        isBrightnContrast = false;

                                        Mat gray_pic = Mat::zeros(image.rows, image.cols, CV_8UC1);
                                        cvtColor(image, gray_pic, COLOR_BGR2GRAY);

                                        sprintf(img_name, "grayscale_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, gray_pic);
                                }
                                // convert image to a grayscale image without using build-in function when press 'h'
                                else if (key == 'h') {
                                        isGray_alt = true;
                                        isManipulated = true;
                                        isGray = false;
                                        isBlurred = false;
                                        isSobelX = false;
                                        isSobelY = false;
                                        isMagnitude = false;
                                        isQuantizes = false;
                                        isCartoonized = false;
                                        isBrightnContrast = false;

                                        Mat gray_alt_pic = Mat::zeros(image.rows, image.cols, CV_8UC1);
                                        greyscale(image, gray_alt_pic);

                                        sprintf(img_name, "grayscale_alt_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, gray_alt_pic);
                                }
                                // implement a 5x5 Gaussian filter as separable 1x5 filters when press 'b'
                                else if (key == 'b') {
                                        isBlurred = true;
                                        isManipulated = true;
                                        isGray = false;
                                        isGray_alt = false;
                                        isSobelX = false;
                                        isSobelY = false;
                                        isMagnitude = false;
                                        isQuantizes = false;
                                        isCartoonized = false;
                                        isBrightnContrast = false;

                                        Mat blurred_pic = Mat::zeros(image.rows, image.cols, CV_8UC3);
                                        blur5x5(image, blurred_pic);

                                        sprintf(img_name, "GaussianBlurred_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, blurred_pic);
                                }
                                // implement a 3x3 Sobel X as separable 1x3 filters when press 'x'
                                else if (key == 'x') {
                                        isSobelX = true;
                                        isManipulated = true;
                                        isGray = false;
                                        isGray_alt = false;
                                        isBlurred = false;
                                        isSobelY = false;
                                        isMagnitude = false;
                                        isQuantizes = false;
                                        isCartoonized = false;
                                        isBrightnContrast = false;

                                        Mat sobelx_pic = Mat::zeros(image.rows, image.cols, CV_16SC3);
                                        sobelX3x3(image, sobelx_pic);

                                        convertScaleAbs(sobelx_pic, sobelx_pic);

                                        sprintf(img_name, "sobelx_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, sobelx_pic);
                                }
                                // implement a 3x3 Sobel Y as separable 1x3 filters when press 'y'
                                else if (key == 'y') {
                                        isSobelY = true;
                                        isManipulated = true;
                                        isGray = false;
                                        isGray_alt = false;
                                        isBlurred = false;
                                        isSobelX = false;
                                        isMagnitude = false;
                                        isQuantizes = false;
                                        isCartoonized = false;
                                        isBrightnContrast = false;

                                        Mat sobely_pic = Mat::zeros(image.rows, image.cols, CV_16SC3);
                                        sobelY3x3(image, sobely_pic);

                                        convertScaleAbs(sobely_pic, sobely_pic);

                                        sprintf(img_name, "sobely_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, sobely_pic);
                                }
                                // implement a function that generates a gradient magnitude image from the X and Y Sobel images when press 'm'
                                else if (key == 'm') {
                                        isMagnitude = true;
                                        isManipulated = true;
                                        isGray = false;
                                        isGray_alt = false;
                                        isBlurred = false;
                                        isSobelX = false;
                                        isSobelY = false;
                                        isQuantizes = false;
                                        isCartoonized = false;
                                        isBrightnContrast = false;

                                        Mat sobelx_pic = Mat::zeros(image.rows, image.cols, CV_16SC3);
                                        sobelX3x3(image, sobelx_pic);

                                        Mat sobely_pic = Mat::zeros(image.rows, image.cols, CV_16SC3);
                                        sobelY3x3(image, sobely_pic);

                                        Mat magnitude_pic = Mat::zeros(image.rows, image.cols, CV_16SC3);
                                        magnitude(sobelx_pic, sobely_pic, magnitude_pic);

                                        convertScaleAbs(magnitude_pic, magnitude_pic);

                                        sprintf(img_name, "magnitude_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, magnitude_pic);
                                }
                                // implement a function that blurs and quantizes a color image when press 'l'
                                else if (key == 'l') {
                                        isQuantizes = true;
                                        isManipulated = true;
                                        isGray = false;
                                        isGray_alt = false;
                                        isBlurred = false;
                                        isSobelX = false;
                                        isSobelY = false;
                                        isMagnitude = false;
                                        isCartoonized = false;
                                        isBrightnContrast = false;

                                        while(true) {
                                                cout << "Enter a quantize level > 0: ";
                                                cin >> quantizes_lv;
                                                cout << "-----------------------------------------------------------------------" << endl;;
                                                if (quantizes_lv > 0) {
                                                        break;
                                                }
                                                cout << "Invalid" << endl;
                                        }

                                        Mat quantize_pic = Mat::zeros(image.rows, image.cols, CV_8UC3);
                                        blurQuantize(image, quantize_pic, quantizes_lv);

                                        sprintf(img_name, "quantized_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, quantize_pic);
                                }
                                else if (key == 'c') {
                                        isCartoonized = true;
                                        isManipulated = true;
                                        isGray = false;
                                        isGray_alt = false;
                                        isBlurred = false;
                                        isSobelX = false;
                                        isSobelY = false;
                                        isMagnitude = false;
                                        isQuantizes = false;
                                        isBrightnContrast = false;

                                        while(true) {
                                                cout << "Enter a quantize level > 0: ";
                                                cin >> quantizes_lv;
                                                cout << endl;
                                                cout << "Enter a threshold > 0: ";
                                                cin >> threshold;
                                                cout << "-----------------------------------------------------------------------" << endl;;
                                                if (quantizes_lv > 0 && threshold > 0) {
                                                        break;
                                                }
                                                cout << "Invalid" << endl;
                                        }

                                        Mat cartoonized_pic = Mat::zeros(image.rows, image.cols, CV_16SC3);
                                        cartoon(image, cartoonized_pic, quantizes_lv, threshold);

                                        sprintf(img_name, "cartoonized_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, cartoonized_pic);
                                }
                                // modify the brightness and contrast upon untered value when press 'n'
                                else if (key == 'n') {
                                        isBrightnContrast = true;
                                        isManipulated = true;
                                        isGray = false;
                                        isGray_alt = false;
                                        isBlurred = false;
                                        isSobelX = false;
                                        isSobelY = false;
                                        isMagnitude = false;
                                        isQuantizes = false;
                                        isCartoonized = false;

                                        while(true) {
                                                cout << "Enter a double type brightness level (between 0 and 3, 1 means no change): ";
                                                cin >> alpha;
                                                cout << endl;
                                                cout << "Enter an integer contrast level (between -127 and 127): ";
                                                cin >> beta;
                                                cout << "-----------------------------------------------------------------------" << endl;;
                                                if (alpha >= 0 && alpha <= 3 && beta >= -127 && beta <= 127) {
                                                        break;
                                                }
                                                cout << "Invalid" << endl;
                                        }
                                        
                                        Mat bright_contrast_pic = Mat::zeros(image.rows, image.cols, CV_8UC3);
                                        brightnesscontrast(image, bright_contrast_pic, alpha, beta);

                                        sprintf(img_name, "bright_contrast_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, bright_contrast_pic);
                                }
                                // extension
                                // apply sharpen filter to image when press 's'
                                else if (key == 'r') {
                                        isSharpened = true;
                                        isManipulated = true;
                                        isGray = false;
                                        isGray_alt = false;
                                        isBlurred = false;
                                        isSobelX = false;
                                        isSobelY = false;
                                        isMagnitude = false;
                                        isQuantizes = false;
                                        isCartoonized = false;
                                        isBrightnContrast = false;
                                        Mat sharpened_pic = Mat::zeros(image.rows, image.cols, CV_16SC3);
                                        sharp(image, sharpened_pic);

                                        sprintf(img_name, "sharpened_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, sharpened_pic);
                                }
                                // extension
                                // apply combination of some filters to image
                                else if (key == 'z') {
                                        isBigMess = true;
                                        isManipulated = true;
                                        isGray = false;
                                        isGray_alt = false;
                                        isBlurred = false;
                                        isSobelX = false;
                                        isSobelY = false;
                                        isMagnitude = false;
                                        isQuantizes = false;
                                        isCartoonized = false;
                                        isBrightnContrast = false;

                                        while(true) {
                                                cout << "Enter a quantize level > 0: ";
                                                cin >> quantizes_lv;
                                                cout << endl;
                                                cout << "Enter a threshold > 0: ";
                                                cin >> threshold;
                                                cout << "-----------------------------------------------------------------------" << endl;;
                                                if (quantizes_lv > 0 && threshold > 0) {
                                                        break;
                                                }
                                                cout << "Invalid" << endl;
                                        }

                                        Mat big_mess = Mat::zeros(image.rows, image.cols, CV_16SC3);
                                        bigMess(image, big_mess, quantizes_lv, threshold);

                                        sprintf(img_name, "big_mess_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, big_mess);
                                }

                        }


                        return 0;
                }
                else if (selection == "video") {

                        // open the video device
                        capdev = new VideoCapture(0);
                        if( !capdev->isOpened() ) {
                                printf("Unable to open video device\n");
                                return(-1);
                        }


                        // get some properties of the image
                        Size refS( (int) capdev->get(CAP_PROP_FRAME_WIDTH ),
                                (int) capdev->get(CAP_PROP_FRAME_HEIGHT));
                        printf("Expected size: %d %d\n", refS.width, refS.height);

                        namedWindow("Video", 1); // identifies a window
                        Mat frame;
                        cout << "-----------------------------------------------------------------------" << endl
                                << "Press s to save the screenshot" << endl 
                                << "Press g or h to convert video to a grayscale and take screenshot" << endl
                                << "Press b to apply Gaussian Blur to video and take screenshot" << endl
                                << "Press x to apply Sobel X operator to video and take screenshot" << endl
                                << "Press y to apply Sobel Y operator to video and take screenshot" << endl
                                << "Press m to show magnitude image of video and take screenshot" << endl
                                << "Press l to show quantized image of video and take screenshot" << endl
                                << "Press c to cartoonize video and take screenshot" << endl
                                << "Press n to adjust brightness and constrast" << endl 
                                << "Press v to start recording the video, press v again to stop recoring" << endl
                                << "Press r to sharpen the video" << endl
                                << "Press z to apply a big-mess filter to the video" << endl
                                << "Press q to quit" << endl
                                << "-----------------------------------------------------------------------" << endl;

                        for(;;) {
                                *capdev >> frame; // get a new frame from the camera, treat as a stream
                                if( frame.empty() ) {
                                        printf("frame is empty\n");
                                        break;
                                }

                                if (isRecording) {

                                        if (isManipulated) {
                                                
                                                if (isGray) {
                                                        Mat gray_pic = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
                                                        cvtColor(frame, gray_pic, COLOR_BGR2GRAY);
                                                        video.write(gray_pic);
                                                        imshow("Video", gray_pic);
                                                }
                                                else if (isGray_alt) {
                                                        Mat gray_alt_pic = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
                                                        greyscale(frame, gray_alt_pic);
                                                        video.write(gray_alt_pic);
                                                        imshow("Video", gray_alt_pic);      
                                                }
                                                else if (isBlurred) {
                                                        Mat blurred_pic = Mat::zeros(frame.rows, frame.cols, CV_8UC3);
                                                        blur5x5(frame, blurred_pic);
                                                        video.write(blurred_pic);
                                                        imshow("Video", blurred_pic);
                                                }
                                                else if (isSobelX) {
                                                        Mat sobelx_pic = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                                        sobelX3x3(frame, sobelx_pic);
                                                        convertScaleAbs(sobelx_pic, sobelx_pic);
                                                        video.write(sobelx_pic);
                                                        imshow("Video", sobelx_pic);
                                                }
                                                else if (isSobelY) {
                                                        Mat sobely_pic = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                                        sobelY3x3(frame, sobely_pic);
                                                        convertScaleAbs(sobely_pic, sobely_pic);
                                                        video.write(sobely_pic);
                                                        imshow("Video", sobely_pic);
                                                }
                                                else if (isMagnitude) {
                                                        Mat sobelx_pic = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                                        sobelX3x3(frame, sobelx_pic);
                                                        Mat sobely_pic = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                                        sobelY3x3(frame, sobely_pic);
                                                        Mat magnitude_pic = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                                        magnitude(sobelx_pic, sobely_pic, magnitude_pic);
                                                        convertScaleAbs(magnitude_pic, magnitude_pic);
                                                        video.write(magnitude_pic);
                                                        imshow("Video", magnitude_pic);
                                                }
                                                else if (isQuantizes) {
                                                        Mat quantize_pic = Mat::zeros(frame.rows, frame.cols, CV_8UC3);
                                                        blurQuantize(frame, quantize_pic, quantizes_lv);
                                                        video.write(quantize_pic);
                                                        imshow("Video", quantize_pic);
                                                }
                                                else if (isCartoonized) {
                                                        Mat cartoonized_pic = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                                        cartoon(frame, cartoonized_pic, quantizes_lv, threshold);
                                                        convertScaleAbs(cartoonized_pic, cartoonized_pic);
                                                        video.write(cartoonized_pic);
                                                        imshow("Video", cartoonized_pic);
                                                }
                                                else if (isBrightnContrast) {
                                                        Mat bright_contrast_pic = Mat::zeros(frame.rows, frame.cols, CV_8UC3);
                                                        brightnesscontrast(frame, bright_contrast_pic, alpha, beta);
                                                        video.write(bright_contrast_pic);
                                                        imshow("Video", bright_contrast_pic);
                                                }
                                                else if (isSharpened) {
                                                        Mat sharpened_pic = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                                        sharp(frame, sharpened_pic);
                                                        convertScaleAbs(sharpened_pic, sharpened_pic);
                                                        video.write(sharpened_pic);
                                                        imshow("Video", sharpened_pic);
                                                }
                                                else if (isBigMess) {
                                                        Mat big_mess = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                                        bigMess(frame, big_mess, quantizes_lv, threshold);
                                                        video.write(big_mess);
                                                        imshow("Video", big_mess);
                                                }
                                        }
                                        else {
                                                video.write(frame);
                                                imshow("Video", frame);
                                        }
                                }
                                else {

                                        if (isManipulated) {
                                                
                                                if (isGray) {
                                                        Mat gray_pic = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
                                                        cvtColor(frame, gray_pic, COLOR_BGR2GRAY);
                                                        imshow("Video", gray_pic);
                                                }
                                                else if (isGray_alt) {
                                                        Mat gray_alt_pic = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
                                                        greyscale(frame, gray_alt_pic);
                                                        imshow("Video", gray_alt_pic);      
                                                }
                                                else if (isBlurred) {
                                                        Mat blurred_pic = Mat::zeros(frame.rows, frame.cols, CV_8UC3);
                                                        blur5x5(frame, blurred_pic);
                                                        imshow("Video", blurred_pic);
                                                }
                                                else if (isSobelX) {
                                                        Mat sobelx_pic = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                                        sobelX3x3(frame, sobelx_pic);
                                                        convertScaleAbs(sobelx_pic, sobelx_pic);
                                                        imshow("Video", sobelx_pic);
                                                }
                                                else if (isSobelY) {
                                                        Mat sobely_pic = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                                        sobelY3x3(frame, sobely_pic);
                                                        convertScaleAbs(sobely_pic, sobely_pic);
                                                        imshow("Video", sobely_pic);
                                                }
                                                else if (isMagnitude) {
                                                        Mat sobelx_pic = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                                        sobelX3x3(frame, sobelx_pic);
                                                        Mat sobely_pic = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                                        sobelY3x3(frame, sobely_pic);
                                                        Mat magnitude_pic = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                                        magnitude(sobelx_pic, sobely_pic, magnitude_pic);
                                                        convertScaleAbs(magnitude_pic, magnitude_pic);
                                                        imshow("Video", magnitude_pic);
                                                }
                                                else if (isQuantizes) {
                                                        Mat quantize_pic = Mat::zeros(frame.rows, frame.cols, CV_8UC3);
                                                        blurQuantize(frame, quantize_pic, quantizes_lv);
                                                        imshow("Video", quantize_pic);
                                                }
                                                else if (isCartoonized) {
                                                        Mat cartoonized_pic = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                                        cartoon(frame, cartoonized_pic, quantizes_lv, threshold);
                                                        convertScaleAbs(cartoonized_pic, cartoonized_pic);
                                                        imshow("Video", cartoonized_pic);
                                                }
                                                else if (isBrightnContrast) {
                                                        Mat bright_contrast_pic = Mat::zeros(frame.rows, frame.cols, CV_8UC3);
                                                        brightnesscontrast(frame, bright_contrast_pic, alpha, beta);
                                                        imshow("Video", bright_contrast_pic);
                                                }
                                                else if (isSharpened) {
                                                        Mat sharpened_pic = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                                        sharp(frame, sharpened_pic);
                                                        convertScaleAbs(sharpened_pic, sharpened_pic);
                                                        imshow("Video", sharpened_pic);
                                                }
                                                else if (isBigMess) {
                                                        Mat big_mess = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                                        bigMess(frame, big_mess, quantizes_lv, threshold);
                                                        imshow("Video", big_mess);
                                                }
                                        }
                                        else {
                                                imshow("Video", frame);
                                        }
                                }

                                // see if there is a waiting keystroke
                                char key = waitKey(10);
                                
                                curr_time = time(NULL);
                                mt = localtime(& curr_time);
                                
                                if( key == 'q') {
                                        break;
                                }
                                // save the frame when press 's'
                                else if (key == 's') {
                                        sprintf(img_name, "%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, frame);
                                }
                                // convert image to a grayscale image when press 'g'
                                else if (key == 'g') {
                                        isGray = true;
                                        isManipulated = true;
                                        isGray_alt = false;
                                        isBlurred = false;
                                        isSobelX = false;
                                        isSobelY = false;
                                        isMagnitude = false;
                                        isQuantizes = false;
                                        isCartoonized = false;
                                        isBrightnContrast = false;

                                        Mat gray_pic = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
                                        cvtColor(frame, gray_pic, COLOR_BGR2GRAY);

                                        sprintf(img_name, "grayscale_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, gray_pic);
                                }
                                // convert image to a grayscale image without using build-in function when press 'h'
                                else if (key == 'h') {
                                        isGray_alt = true;
                                        isManipulated = true;
                                        isGray = false;
                                        isBlurred = false;
                                        isSobelX = false;
                                        isSobelY = false;
                                        isMagnitude = false;
                                        isQuantizes = false;
                                        isCartoonized = false;
                                        isBrightnContrast = false;

                                        Mat gray_alt_pic = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
                                        greyscale(frame, gray_alt_pic);

                                        sprintf(img_name, "grayscale_alt_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, gray_alt_pic);
                                }
                                // implement a 5x5 Gaussian filter as separable 1x5 filters when press 'b'
                                else if (key == 'b') {
                                        isBlurred = true;
                                        isManipulated = true;
                                        isGray = false;
                                        isGray_alt = false;
                                        isSobelX = false;
                                        isSobelY = false;
                                        isMagnitude = false;
                                        isQuantizes = false;
                                        isCartoonized = false;
                                        isBrightnContrast = false;

                                        Mat blurred_pic = Mat::zeros(frame.rows, frame.cols, CV_8UC3);
                                        blur5x5(frame, blurred_pic);

                                        sprintf(img_name, "GaussianBlurred_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, blurred_pic);
                                }
                                // implement a 3x3 Sobel X as separable 1x3 filters when press 'x'
                                else if (key == 'x') {
                                        isSobelX = true;
                                        isManipulated = true;
                                        isGray = false;
                                        isGray_alt = false;
                                        isBlurred = false;
                                        isSobelY = false;
                                        isMagnitude = false;
                                        isQuantizes = false;
                                        isCartoonized = false;
                                        isBrightnContrast = false;

                                        Mat sobelx_pic = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                        sobelX3x3(frame, sobelx_pic);

                                        convertScaleAbs(sobelx_pic, sobelx_pic);

                                        sprintf(img_name, "sobelx_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, sobelx_pic);
                                }
                                // implement a 3x3 Sobel Y as separable 1x3 filters when press 'y'
                                else if (key == 'y') {
                                        isSobelY = true;
                                        isManipulated = true;
                                        isGray = false;
                                        isGray_alt = false;
                                        isBlurred = false;
                                        isSobelX = false;
                                        isMagnitude = false;
                                        isQuantizes = false;
                                        isCartoonized = false;
                                        isBrightnContrast = false;

                                        Mat sobely_pic = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                        sobelY3x3(frame, sobely_pic);

                                        convertScaleAbs(sobely_pic, sobely_pic);

                                        sprintf(img_name, "sobely_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, sobely_pic);
                                }
                                // implement a function that generates a gradient magnitude image from the X and Y Sobel images when press 'm'
                                else if (key == 'm') {
                                        isMagnitude = true;
                                        isManipulated = true;
                                        isGray = false;
                                        isGray_alt = false;
                                        isBlurred = false;
                                        isSobelX = false;
                                        isSobelY = false;
                                        isQuantizes = false;
                                        isCartoonized = false;
                                        isBrightnContrast = false;

                                        Mat sobelx_pic = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                        sobelX3x3(frame, sobelx_pic);

                                        Mat sobely_pic = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                        sobelY3x3(frame, sobely_pic);

                                        Mat magnitude_pic = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                        magnitude(sobelx_pic, sobely_pic, magnitude_pic);

                                        convertScaleAbs(magnitude_pic, magnitude_pic);

                                        sprintf(img_name, "magnitude_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, magnitude_pic);
                                }
                                // implement a function that blurs and quantizes a color image when press 'l'
                                else if (key == 'l') {
                                        isQuantizes = true;
                                        isManipulated = true;
                                        isGray = false;
                                        isGray_alt = false;
                                        isBlurred = false;
                                        isSobelX = false;
                                        isSobelY = false;
                                        isMagnitude = false;
                                        isCartoonized = false;
                                        isBrightnContrast = false;

                                        while(true) {
                                                cout << "Enter a quantize level > 0: ";
                                                cin >> quantizes_lv;
                                                cout << "-----------------------------------------------------------------------" << endl;;
                                                if (quantizes_lv > 0) {
                                                        break;
                                                }
                                                cout << "Invalid" << endl;
                                        }

                                        Mat quantize_pic = Mat::zeros(frame.rows, frame.cols, CV_8UC3);
                                        blurQuantize(frame, quantize_pic, quantizes_lv);

                                        sprintf(img_name, "quantized_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, quantize_pic);
                                }
                                // implement a live video cartoonization function using the gradient magnitude and blur/quantize filters when press 'c'
                                else if (key == 'c') {
                                        isCartoonized = true;
                                        isManipulated = true;
                                        isGray = false;
                                        isGray_alt = false;
                                        isBlurred = false;
                                        isSobelX = false;
                                        isSobelY = false;
                                        isMagnitude = false;
                                        isQuantizes = false;
                                        isBrightnContrast = false;

                                        while(true) {
                                                cout << "Enter a quantize level > 0: ";
                                                cin >> quantizes_lv;
                                                cout << endl;
                                                cout << "Enter a threshold > 0: ";
                                                cin >> threshold;
                                                cout << "-----------------------------------------------------------------------" << endl;;
                                                if (quantizes_lv > 0 && threshold > 0) {
                                                        break;
                                                }
                                                cout << "Invalid" << endl;
                                        }

                                        Mat cartoonized_pic = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                        cartoon(frame, cartoonized_pic, quantizes_lv, threshold);

                                        sprintf(img_name, "cartoonized_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, cartoonized_pic);
                                }
                                // modify the brightness and contrast upon untered value when press 'n'
                                else if (key == 'n') {
                                        isBrightnContrast = true;
                                        isManipulated = true;
                                        isGray = false;
                                        isGray_alt = false;
                                        isBlurred = false;
                                        isSobelX = false;
                                        isSobelY = false;
                                        isMagnitude = false;
                                        isQuantizes = false;
                                        isCartoonized = false;

                                        while(true) {
                                                cout << "Enter a double type brightness level (between 0 and 3, 1 means no change): ";
                                                cin >> alpha;
                                                cout << endl;
                                                cout << "Enter an integer contrast level (between -127 and 127): ";
                                                cin >> beta;
                                                cout << "-----------------------------------------------------------------------" << endl;;
                                                if (alpha >= 0 && alpha <= 3 && beta >= -127 && beta <= 127) {
                                                        break;
                                                }
                                                cout << "Invalid" << endl;
                                        }
                                        
                                        Mat bright_contrast_pic = Mat::zeros(frame.rows, frame.cols, CV_8UC3);
                                        brightnesscontrast(frame, bright_contrast_pic, alpha, beta);

                                        sprintf(img_name, "bright_contrast_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, bright_contrast_pic);
                                }
                                // extension
                                // start recording the video when press 'v', stops when press 'v' again
                                else if (key == 'v') {
                                        if (isRecording) {
                                                isRecording = false;
                                                cout << "Recoring End" << endl;
                                        }
                                        else {
                                                isRecording = true;
                                                sprintf(img_name, "video_%d.avi", video_count++);
                                                video.open(img_name, VideoWriter::fourcc('M','J','P','G'), 10, Size(refS.width, refS.height), true);
                                                cout << "Recoring Start" << endl;
                                        }
                                }
                                // extension
                                // apply sharpen filter to video when press 's'
                                else if (key == 'r') {
                                        isSharpened = true;
                                        isManipulated = true;
                                        isGray = false;
                                        isGray_alt = false;
                                        isBlurred = false;
                                        isSobelX = false;
                                        isSobelY = false;
                                        isMagnitude = false;
                                        isQuantizes = false;
                                        isCartoonized = false;
                                        isBrightnContrast = false;
                                        Mat sharpened_pic = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                        sharp(frame, sharpened_pic);

                                        sprintf(img_name, "sharpened_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, sharpened_pic);
                                }
                                // extension
                                // apply combination of some filters to video
                                else if (key == 'z') {
                                        isBigMess = true;
                                        isManipulated = true;
                                        isGray = false;
                                        isGray_alt = false;
                                        isBlurred = false;
                                        isSobelX = false;
                                        isSobelY = false;
                                        isMagnitude = false;
                                        isQuantizes = false;
                                        isCartoonized = false;
                                        isBrightnContrast = false;

                                        while(true) {
                                                cout << "Enter a quantize level > 0: ";
                                                cin >> quantizes_lv;
                                                cout << endl;
                                                cout << "Enter a threshold > 0: ";
                                                cin >> threshold;
                                                cout << "-----------------------------------------------------------------------" << endl;;
                                                if (quantizes_lv > 0 && threshold > 0) {
                                                        break;
                                                }
                                                cout << "Invalid" << endl;
                                        }

                                        Mat big_mess = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
                                        bigMess(frame, big_mess, quantizes_lv, threshold);

                                        sprintf(img_name, "big_mess_%d%02d%02d%02d%02d%02d.jpg", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec);
                                        imwrite(img_name, big_mess);
                                }

                        }

                        delete capdev;
                        return(0);
                }
                else {
                        cout << "Invalid" << endl;
                }
        
        }
        return 0;
}
