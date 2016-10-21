
//OpenCV 
#include "cv.h"
#include "highgui.h"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"

//std
#include <iostream>
#include <cstdlib>

//consts
const unsigned int MIN_NUM_FEATURES = 300; //minimum number of point fetaures

int main(int argc, char *argv[]) 
{
    cv::VideoCapture camera; //OpenCV video capture object
    cv::Mat image; //OpenCV image object
	int cam_id; //camera id . Associated to device number in /dev/videoX
	cv::Ptr<cv::ORB> orb_detector = new cv::ORB(MIN_NUM_FEATURES); //ORB point feature detector
    cv::vector<cv::KeyPoint> point_set; //set of point features
    cv::Ptr<cv::DescriptorExtractor> orb_descriptor; //ORB descriptor
    orb_descriptor = cv::DescriptorExtractor::create("ORB"); //init the descriptor
    cv::Mat descriptor_set; //set of descriptors, for each feature there is an associated descriptor 
	
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
        		
    //**************** Find ORB point fetaures and descriptors ****************************
        
        //clear previous points
        point_set.clear(); 
        
        //detect point features
        orb_detector->detect(image, point_set);
        
        //extract descriptors
        orb_descriptor->compute(image,point_set,descriptor_set);
        
        //draw points on the image
        cv::drawKeypoints( image, point_set, image, 255, cv::DrawMatchesFlags::DEFAULT );      
                
    //********************************************************************
		
        //show image
        cv::imshow("Output Window", image);

		//Waits 1 millisecond to check if a key has been pressed. If so, breaks the loop. Otherwise continues.
        if(cv::waitKey(1) >= 0) break;
    }   
}