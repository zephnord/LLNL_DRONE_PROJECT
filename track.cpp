#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>
using namespace std;
using namespace cv;
 
Mat frame;
Mat hsv;
Mat thresh;
Mat res;
vector<vector<Point> > contours;
vector<Vec4i> hierarchy;
float area;
int pic, desiredArea;

int main(){

  VideoCapture vid(1); 
  if(!vid.isOpened())
  {
    cout << "Can't Open Video...RIP" << endl; 
    return -1; 
  }

  while(1)
  {		  
    
    vid >> frame;
    if (frame.empty())
      break;   
    desiredArea = 500;
    line(frame, Point(frame.size().width/2, 0), Point(frame.size().width/2, frame.size().height), Scalar(110,220,0),2,8);
    line(frame, Point(0, frame.size().height/2), Point(frame.size().width, frame.size().height/2), Scalar(110,220,0),2,8);
    cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    inRange(hsv, Scalar(25,100,100),Scalar(70,255,255),thresh);
    dilate(thresh, thresh, getStructuringElement(MORPH_ELLIPSE, Size(5,5)));
    bitwise_and(thresh, thresh, res);
 
    findContours(thresh, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);   

    for(pic = 0; pic < contours.size(); pic++)
    {
        area = contourArea(contours[pic],false); 
        if (area > desiredArea)
        {      
            vector<Moments> mu(contours.size() );
            vector<Point2f> mc(contours.size() );
            mu[pic] = moments(contours[pic], false);
            mc[pic] = Point2f(mu[pic].m10/mu[pic].m00 , mu[pic].m01/mu[pic].m00);
            circle(frame, mc[pic], 4, Scalar(0,0,255), -1, 8, 0);
            char num[10];
            sprintf(num, "(%f, %f)",frame.size().width/2 - mc[pic].x, frame.size().height/2 - mc[pic].y);
            putText(frame, num, mc[pic], FONT_HERSHEY_PLAIN,2,Scalar(0,0,255,255));
        }
    }
	  
    imshow( "Captured Output", frame);
    imshow( "Thresholded Output", thresh);
    char c = (char)waitKey(1);
    if( c == 27 ) 
      break;
  }
  vid.release();
  destroyAllWindows();
  return 0;
}
