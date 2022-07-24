#include <stdio.h>
#include<iostream>
#include <string>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/opencv.hpp>

using namespace std; 
using namespace cv;


int main(){
        std::cout << "Test";

        std::string output = ""; 
        Mat hsv {};
        Mat inimg = imread("");                     // <-- CHANGE Image Path            
        Mat thres;
        if(!inimg.data)
        {
            std::cout<<"Could not find the image";
            //return -1;
        }

        
        namedWindow( "original", WINDOW_NORMAL );
        imshow( "original", inimg ); 

        cvtColor(inimg, hsv, COLOR_BGR2HSV);
        vector<Mat> channels;
        split(hsv, channels);
        Mat H = channels[0];
        Mat S = channels[1];
        Mat V = channels[2];   

        

        Mat shiftedH = H.clone();
        int shift = 90; // in openCV hue values go from 0 to 180 (so have to be doubled to get to 0 .. 360) because of byte range from 0 to 255    // <-- CHANGE hue spectrum
        for(int j=0; j<shiftedH.rows; ++j){
            for(int i=0; i<shiftedH.cols; ++i)
            {
                shiftedH.at<unsigned char>(j,i) = (shiftedH.at<unsigned char>(j,i) + shift)%180;
            }
        } 
        namedWindow( "shiftedH", WINDOW_NORMAL );
        imshow("shiftedH", shiftedH);
          

////////METHODE1/////////
        Mat cannyH;
        Canny(shiftedH, cannyH, 100, 50);
        Mat cannyS;
        Canny(S, cannyS, 200, 100);

    // extract contours of the canny image:
        std::vector<std::vector<Point> > contoursH;
        std::vector<Vec4i> hierarchyH;
        findContours(cannyH,contoursH, hierarchyH,  RETR_TREE , CHAIN_APPROX_SIMPLE);

        namedWindow( "cannyH", WINDOW_NORMAL );
        imshow("cannyH", cannyH);

    // draw the contours to a copy of the input image:
        Mat outputH = inimg.clone();
        for( int i = 0; i< contoursH.size(); i++ )
        {
            drawContours( outputH, contoursH, i, Scalar(0,0,255), 2, 8, hierarchyH, 0);                  // <-- CHANGE thickness of drawing (5)
        }
        dilate(cannyH, cannyH, Mat());
        dilate(cannyH, cannyH, Mat());
        dilate(cannyH, cannyH, Mat());

        for( int i = 0; i< contoursH.size(); i++ )
        {
            if(contourArea(contoursH[i]) < 20) continue; // ignore contours that are too small to be a patty
            if(hierarchyH[i][3] < 0) continue;  // ignore "outer" contours
            drawContours( outputH, contoursH, i, Scalar(0,0,255), 10 ,8 , hierarchyH, 0);               // <-- CHANGE thickness of drawing (5)

        }

    
        namedWindow( "final", WINDOW_NORMAL );
        imshow( "final", outputH); 


//-----------------------METHODE2-----------------------//
        inRange(shiftedH,Scalar(0,0,0),Scalar(60,0,0),thres);                                   // <-- CHANGE Max Black Color (Sensity) (3.1)

        namedWindow( "Range_Image", WINDOW_NORMAL );
        imshow( "Range_Image", thres ); 

    // extract contours of the canny image:
        std::vector<std::vector<Point> > contoursH_Range;
        std::vector<Vec4i> hierarchyH_Range;
        findContours(thres,contoursH_Range, hierarchyH_Range,  RETR_TREE , CHAIN_APPROX_SIMPLE);

        namedWindow( "cannyState_Range", WINDOW_NORMAL );
        imshow("cannyState_Range", thres);


       
    // draw the contours to a copy of the input image:
        Mat outputH_Range = inimg.clone();
            
        for( int i = 0; i< contoursH_Range.size(); i++ )
        {
            
            drawContours( outputH_Range, contoursH_Range, i, Scalar(0,0,255), 30, 8, hierarchyH_Range, 0);  // <-- CHANGE thickness of drawing (5)
        }
        dilate(thres, thres, Mat());
        dilate(thres, thres, Mat());
        dilate(thres, thres, Mat());


        for( int i = 0; i< contoursH_Range.size(); i++ )
        {
            if(contourArea(contoursH_Range[i]) < 20) continue; // ignore contours that are too small to be a patty
            if(hierarchyH_Range[i][3] < 0) continue;
        
            drawContours(thres,contoursH_Range,i, Scalar(0,0,255), 150, 8, hierarchyH_Range, 0);
        }

        namedWindow( "Range_final", WINDOW_NORMAL );
        imshow( "Range_final", outputH_Range ); 
        
        
    

    

    waitKey(0);  
    return 0;
}