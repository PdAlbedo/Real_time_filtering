/*
Xichen Liu

CS5330 CV Project-1

Create a file imgDisplay.cpp. Your main function should read an image file and display it in a window.
Then your program should enter a loop, checking for a keypress. If the user types 'q', the program should quit
*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int main() {

    Mat img = imread("Lenna.png");

    namedWindow("Lenna");
    
    while(true) {
        imshow("Lenna", img);
        if (waitKey(1) == 'q') {
            break;
        }
    }
    
    return 0;
}
