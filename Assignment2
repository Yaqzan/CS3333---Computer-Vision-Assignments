/* This program was made using Microsoft Visual Studio 2013. For compiler reasons, you may need to 
	#include "stdafx.h"
	Make sure openCV is installed in the workspace environment. Compile and run the program with g++.
    At the command line, include the path names for the two images to be compared
    Example: g++ main.cpp -o run
			run 1.png 2.png 	
*/
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
/* ***********************************************
Module Name: doPryDown
Author: Yaqzan Ali
Date: Oct 3, 2014
Purpose: Takes an image and applies a guassian pyramid reduction
		 Also applies a filter
Input:  
	IplImage in - the image to work on
	int filter - the filter to be applied
Output: 
	IplImage - Return the resulting image
*************************************************/
IplImage* doPyrDown(IplImage* in,int filter = IPL_GAUSSIAN_5x5){
	// Make sure input image is divisible by two.
	assert(in->width % 2 == 0 && in->height % 2 == 0);
	// Create the output image
	IplImage* out = cvCreateImage(cvSize(in->width / 2, in->height / 2),in->depth,in->nChannels);
	// Apply the guassian pyramid reduction
	cvPyrDown(in, out);
	return(out);
};
/* ***********************************************
Module Name: main
Author: Yaqzan Ali
Date: Oct 3, 2014
Purpose: The main function that takes two images of the 
		 same size, and finds the motion differences using
		 a 4th level guassian pyrammid, absolute differences, 
		 and binary thresholding.
Input:  the file paths of the 2 images to compare
*************************************************/
int main(int argc, char** argv)
{
	int pyramid = 3; // The Guassian pyramid level

	//Load the images
	IplImage* img1 = cvLoadImage(argv[1]);
	IplImage* img2 = cvLoadImage(argv[2]);
	
	// Downscale the images
	for (int i = 0; i < pyramid; i++){
		img2 = doPyrDown(img2);
		img1 = doPyrDown(img1);
	}
	
	// Convert the images to grayscale
	IplImage *img1Grey = cvCreateImage(cvGetSize(img1), IPL_DEPTH_8U, 1);
	cvCvtColor(img1, img1Grey, CV_RGB2GRAY);
	IplImage *img2Grey = cvCreateImage(cvGetSize(img2), IPL_DEPTH_8U, 1);
	cvCvtColor(img2, img2Grey, CV_RGB2GRAY);

	// Computes absolute difference image 
	IplImage *imgDiff = cvCreateImage(cvGetSize(img1Grey), IPL_DEPTH_8U, 1);
	cvAbsDiff(img1Grey, img2Grey, imgDiff);

	// Threshold the image into binary black and white image
	IplImage *imgThres = cvCreateImage(cvGetSize(img2Grey), IPL_DEPTH_8U, 1);
	cvThreshold(imgDiff, imgThres, 75, 255, CV_THRESH_BINARY);
	
	//Resize to original image size using nearest neighbor
	img1 = cvLoadImage("2.png");
	IplImage *resize = cvCreateImage(cvGetSize(img1), IPL_DEPTH_8U, 1);
	cvResize(imgThres, resize, CV_INTER_NN);
	
	//Dilate the image to create the mask
	IplImage *dilated = cvCreateImage(cvGetSize(img1), IPL_DEPTH_8U, 1);
	cvDilate(resize, dilated, NULL, 25);
	
	// Create the final image frame
	IplImage *imgFinal = cvCreateImage(cvGetSize(img1), img1->depth, img1->nChannels);;
	IplImage* refImg = cvCreateImage(cvGetSize(img1), img1->depth, img1->nChannels);
	cvZero(refImg);
	
	// Apply the diluted mask to the original image
	cvOr(img1, refImg, imgFinal, dilated);
	
	// Show the image
	cvNamedWindow("Window", CV_WINDOW_AUTOSIZE);
	cvShowImage("Window", imgFinal);

	// Wait for keypress, then close and exit
	cvWaitKey(0);
	cvReleaseImage(&imgFinal);
	cvDestroyWindow("Window");
}
