
//OpenCV 
#include "cv.h"
#include "highgui.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

//std
#include <iostream>
#include <cstdlib>

//constants
const int GAUSSIAN_BLUR_SIZE = 7;
const double GAUSSIAN_BLUR_SIGMA = 2; 
const double CANNY_EDGE_TH = 150;
const double HOUGH_ACCUM_RESOLUTION = 2;
const double MIN_CIRCLE_DIST = 40;
const double HOUGH_ACCUM_TH = 70;
const int MIN_RADIUS = 20;
const int MAX_RADIUS = 100;

int main(int argc, char *argv[]) 
{
    cv::VideoCapture camera; //OpenCV video capture object
    cv::Mat image; //OpenCV image object
	int cam_id; //camera id . Associated to device number in /dev/videoX
    cv::Mat gray_image;
    cv::vector<cv::Vec3f> circles;
    cv::Point center;
    int radius;
    

// 	cv::Ptr<cv::ORB> orb_detector = new cv::ORB(MIN_NUM_FEATURES); //ORB point feature detector
//     cv::vector<cv::KeyPoint> point_set; //set of point features
//     cv::Ptr<cv::DescriptorExtractor> orb_descriptor; //ORB descriptor
//     orb_descriptor = cv::DescriptorExtractor::create("ORB"); //init the descriptor
//     cv::Mat descriptor_set; //set of descriptors, for each feature there is an associated descriptor 
	
	//check user args
	switch(argc)
	{
		case 1: //no argument provided, so try /dev/video0
			cam_id = 0;  
			break; 
		case 2: //an argument is provided. Get it and set cam_id
			cam_id = atoi(argv[1]);
			break; 
		default: 
			std::cout << "Invalid number of arguments. Call program as: webcam_capture [video_device_id]. " << std::endl; 
			std::cout << "EXIT program." << std::endl; 
			break; 
	}
	
	//advertising to the user 
	std::cout << "Opening video device " << cam_id << std::endl;

    //open the video stream and make sure it's opened
    if( !camera.open(cam_id) ) 
	{
        std::cout << "Error opening the camera. May be invalid device id. EXIT program." << std::endl;
        return -1;
    }

    //Process loop. Capture and point feature extraction. User can quit pressing a key
    while(1)
	{
		//Read image and check it. Blocking call up to a new image arrives from camera.
        if(!camera.read(image)) 
		{
            std::cout << "No image" << std::endl;
            cv::waitKey();
        }
        		
    //**************** Find circles in the image ****************************
        
        //clear previous circles
        circles.clear();

        // If input image is RGB, convert it to gray 
        cv::cvtColor(image, gray_image, CV_BGR2GRAY);

        //Reduce the noise so we avoid false circle detection
        cv::GaussianBlur( gray_image, gray_image, cv::Size(GAUSSIAN_BLUR_SIZE, GAUSSIAN_BLUR_SIZE), GAUSSIAN_BLUR_SIGMA );

        //Apply the Hough Transform to find the circles
        cv::HoughCircles( gray_image, circles, CV_HOUGH_GRADIENT, HOUGH_ACCUM_RESOLUTION, MIN_CIRCLE_DIST, CANNY_EDGE_TH, HOUGH_ACCUM_TH, MIN_RADIUS, MAX_RADIUS );
        
        //draw circles on the image      
        for(unsigned int ii = 0; ii < circles.size(); ii++ )
        {
            if ( circles[ii][0] != -1 )
            {
                    center = cv::Point(cvRound(circles[ii][0]), cvRound(circles[ii][1]));
                    radius = cvRound(circles[ii][2]);
                    cv::circle(image, center, 5, cv::Scalar(0,0,255), -1, 8, 0 );// circle center in green
                    cv::circle(image, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );// circle perimeter in red
            }
        }      
        
    //********************************************************************
    
        //show image
        cv::imshow("Output Window", image);

		//Waits 1 millisecond to check if a key has been pressed. If so, breaks the loop. Otherwise continues.
        if(cv::waitKey(1) >= 0) break;
    }   
}