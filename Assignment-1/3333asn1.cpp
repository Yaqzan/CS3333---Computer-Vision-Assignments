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

using namespace cv;
/* ***********************************************
Module Name: main
Author: Yaqzan Ali
Date: Oct 27, 2014
Purpose: The program takes 2 images, calculates the
optical flow, and uses that to reconstruct
a new image. THen finds the mean and standard
deviation of the 2 images.
Input:  the file paths of the 2 images to compare
*************************************************/
int main(int argc, char** argv)
{
	int count, sum, size = 5;

	//Create the images
	Mat img1, img2, flow, newimg;
	img1 = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	img2 = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);

	// Calculate optical flow
	calcOpticalFlowFarneback(img1, img2, flow, 1, 1, 15, 3, 5, 1.1, 0);

	// Reconstructrct image 2
	for (int i = 0; i < img1.cols; i++){
		for (int j = 0; j < img1.rows; j++){
			sum = 0; count = 0;
			for (int k = i - size; k < i + size; k++){
				if (k >= 0 && k <= img1.cols){
					for (int l = j - size; l < j + size; l++){
						if (l >= 0 && l <= img1.rows){
							int x = (int)(round(flow.at<cv::Point2f>(k, l).x));
							int y = (int)(round(flow.at<cv::Point2f>(k, l).y));
							// check if it satisfies the condition
							if (x + k == i && y + l == j){
								sum += img1.at<uchar>(k, l);
								count++;
							}
						}
					}
				}
			}
			if (count == 0){
				newimg.at<uchar>(i, j) = 0;
			}
			else // find sum, divide by average, set new value of image
				newimg.at<uchar>(i, j) = (int)(round(sum / count));
		}
	}


	// Find mean
	int mean = 0;
	count = 0;
	for (int i = 0; i < img1.cols; i++){
		for (int j = 0; j < img1.rows; j++){
			mean += (img2.at<uchar>(i, j) - newimg.at<uchar>(i, j));
			count++;
		}
	}
	mean = mean / count;

	double sdeviation;
	// Find standard deviation
	for (int i = 0; i < img1.cols; i++){
		for (int j = 0; j < img1.rows; j++){
			sdeviation += ((mean - newimg.at<uchar>(i, j)*(mean - newimg.at<uchar>(i, j))));
		}
	}
	sdeviation = sdeviation / count;
	sdeviation = sqrt(sdeviation);

	// Print results
	std::cout << "Average Difference: " << mean << "\n";
	std::cout << "Standard Deviation: " << sdeviation << "\n";

	waitKey(0);
	return 0;
}
