/*
Yaqzan Ali
3333 Asignment 4
Nov 30, 2014
This program takes images as input and combines them into a panorama
*/

#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define HESSIAN 500
#define MINDIS 4
#define SIZE 4

using namespace cv;

Mat warp(Mat img1, Mat img2)
{
    //Creates a SurfFeatureDetector object 
    SurfFeatureDetector surfDetector(HESSIAN);
    //Creates two KeyPoint objects that will hold the key points of both images
    vector<KeyPoint> img1KepPoints, img2KepPoints;
    //Creates a SurfDescriptorExtractor object 
    SurfDescriptorExtractor surfExtractor;
    //creates 2 Mat objects that will hold the image discriptors
    //will contain the homography matrix
    Mat img1Descriptor, img2Descriptor, homography, result;
    //creates a FlannBasedMatcher object
    FlannBasedMatcher flannMatcher;
    //creates a DMatch vector that will contain all the discriptor matches btween the two images
    //creates a DMatch vector that will contain the good discriptor matches btween the two images
    vector<DMatch> allMatches, goodMatches;
    //will hold the min distance and distance between matches
    double min = 2147483647, distance;
    //will contain the good key points
    vector<Point2f> img1GoodKeyPoints, img2GoodKeyPoints;
    int i;

    //populates the KeyPoint vectors with the key features
    surfDetector.detect(img1, img1KepPoints);
    surfDetector.detect(img2, img2KepPoints);

    //extracts the descripotrs from the two images the populates img1Descriptor and img2Descriptor with these values
    surfExtractor.compute(img1, img1KepPoints, img1Descriptor);
    surfExtractor.compute(img2, img2KepPoints, img2Descriptor);

    //finds all the matches between the images descriptors
    flannMatcher.match(img1Descriptor, img2Descriptor, allMatches);

    //looks for the smallest distance between the matches
    for(i = 0; i < img1Descriptor.rows; i++)
    { 
        distance = allMatches[i].distance;
        if(distance < min)
        {
            min = distance;
        }
    }

    //finds the good matches
    for(i = 0; i < img1Descriptor.rows; i++)
    {
        if(allMatches[i].distance < MINDIS*min)
        {
            goodMatches.push_back(allMatches[i]);
        }
    }

    //iterate through the good matches finds the key points
    for(i = 0; i < goodMatches.size(); i++)
    {
        img1GoodKeyPoints.push_back(img1KepPoints[goodMatches[i].queryIdx].pt);
        img2GoodKeyPoints.push_back(img2KepPoints[goodMatches[i].trainIdx].pt);
    }

    //calculates the homography matrix
    homography = findHomography(img1GoodKeyPoints, img2GoodKeyPoints, CV_RANSAC);

    //warps the image based on the output of the homography matrix
    warpPerspective(img1, result, homography, Size(img2.cols, img2.rows));

    return result;
}

int main(int argc, char** argv)
{
    int i;
    //will contain the list of input images
    //will contain the warrped image
    Mat inputImages[argc - 1], warppedImg;

    //reads in the images
    for(i = 0; i < argc - 1; i++)
    {                                                           
        inputImages[i] = imread(argv[i + 1], CV_LOAD_IMAGE_COLOR);
    }

    //creates the result image
    Mat result(inputImages[0].rows*SIZE,inputImages[0].cols*SIZE,inputImages[0].type());
    //copies the first image to the result Mat
    inputImages[0].copyTo(result(Rect((double)inputImages[0].cols, (double)inputImages[0].rows, inputImages[0].cols, inputImages[0].rows)));

    //iterates through the image list and sitiches images
    for(i = 1; i < argc - 1; i++)
    {
        warppedImg = warp(inputImages[i], result);
        warppedImg.copyTo(result, warppedImg);
    }

    //writes the images to the desk
    imwrite("panoramic.jpg",result);
}
