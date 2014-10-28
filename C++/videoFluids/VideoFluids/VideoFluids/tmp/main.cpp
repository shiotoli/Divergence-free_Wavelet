#include "stdafx.h"
#include "VideoFluids.h"
VideoFluids vf;
int main(int argc,char* argv[])  

{  
	//cvNamedWindow("avi");
	CvCapture* capture = cvCreateFileCapture( "./test.avi");  
	cv::Mat frame,frame1;  
	int i = 0;
	while(1)  
	{  
		i++;
		frame = cv::Mat(cvQueryFrame(capture));
		frame1 = frame.clone();
		if(frame.empty()) break ;  
		//cv::imshow("avi",frame);  
		//bilateralFilter (frame1,frame ,5,3,3) ; 
		cv::cvtColor(frame,frame,CV_BGR2GRAY);
		GaussianBlur (frame,frame ,Size(5,5),3,3) ; 
		vf.addPic(frame);
		if (i>1)
			break;
		//cv::imwrite("test.bmp",frame);
        //char c = cvWaitKey(330);  
        //if(c == 27) break;  
    }  
	printf("read");
	vf.processSFS();
	printf("sfsdown");
	
	vf.filter();/*
	vf.calcFluid();
	cvReleaseCapture(&capture);  
	cvDestroyWindow( "avi");  */
	return 0;  

}  