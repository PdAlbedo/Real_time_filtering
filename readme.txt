CS5330 Project 1-
Xichen Liu


Khoury wiki:
https://wiki.khoury.northeastern.edu/display/~xicliu/CS5330-Project-1-Report


filter.cpp: file that stores the manipulation functions
filter.h: header file of filter.cpp, included in vidDisplay.cpp
imgDisplay.cpp: contains a single program that takes an image and show it until user quit
	by pressing 'q'
vidDisplay.cpp: The program implements some filters that can be applied to the images or 
	each frame of a video.


Operating system: Windows 11
IDE: vscode
code-runner execute command: "cd $dir && g++ $fileName filter.cpp -o $fileNameWithoutExt -std=c++14 -I D:\\CodeAndTools\\OpenCV\\opencv\\build\\include -I D:\\CodeAndTools\\OpenCV\\opencv\\build\\include\\opencv2 -L D:\\CodeAndTools\\OpenCV\\opencv\\build\\x64\\MinGW\\lib -l opencv_core455 -l opencv_core455 -l opencv_imgproc455 -l opencv_imgcodecs455 -l opencv_video455 -l opencv_ml455 -l opencv_highgui455 -l opencv_objdetect455 -l opencv_flann455 -l opencv_imgcodecs455 -l opencv_photo455 -l opencv_videoio455 && $dir$fileNameWithoutExt"


Procedure of running vidDisplay.cpp:

1. select if user want to modify a image or modify the webCamera.
1.1 if select to edit image, type in the image name, then the supported operations 
	will be displayed in panel.
1.2 if select to edit video, the supported operations will be displayed in panel.
	There's one more operations supported by video edition than image edition, which 
	is recording.

Example of recorded video:
https://drive.google.com/file/d/1AKeAzD8AV5x4QKlFxD4GUQDHRpmNZOiv/view?usp=sharing
