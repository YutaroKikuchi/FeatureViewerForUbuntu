#ifndef IMGVIEWER
#define IMGVIEWER

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include "ImageKeeper.h"

class ImageViewer{
private:
	int numCam;
	int combineImgbyLine(cv::vector<ImageKeeper>::iterator begin,cv::vector<ImageKeeper>::iterator end,cv::Mat out);
	int combineImgbyRect(cv::vector<ImageKeeper>::iterator begin,cv::vector<ImageKeeper>::iterator end,cv::Mat out);
public:
	ImageViewer(int numcam){
		numCam = numcam;
	}

	int setWindow(){
		cv::namedWindow("hoge",CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);
	}

	int showImgsbyLine(cv::vector<ImageKeeper>::iterator begin,cv::vector<ImageKeeper>::iterator end){

		cv::Mat output(cv::Size(begin[0].getIMG().cols*numCam,begin[0].getIMG().rows),CV_8UC3);

		combineImgbyLine(begin,end,output);

		cv::imshow("hoge",output);

		return 0;
	}

	int showImgsbyRect(cv::vector<ImageKeeper>::iterator begin,cv::vector<ImageKeeper>::iterator end){

		cv::Mat output(cv::Size(begin[0].getIMG().cols*3,begin[0].getIMG().rows*(int)(numCam/3)),CV_8UC3);

		combineImgbyRect(begin,end,output);

		cv::imshow("hoge",output);

		cv::waitKey(0);
		return 0;
	}

};

int ImageViewer::combineImgbyLine(cv::vector<ImageKeeper>::iterator begin,cv::vector<ImageKeeper>::iterator end,cv::Mat out){
		
	if(end - begin !=numCam)
		return -1;

	cv::Rect rect;
	rect.width = begin[0].getIMG().cols;
	rect.height = begin[0].getIMG().rows;

	for(int i=0;i<end-begin;i++){
		rect.x = i*rect.width;
		rect.y = 0;

		cv::Mat roi(out,rect);
		begin[i].getIMG().copyTo(roi);
	}

	return 0;
}

int ImageViewer::combineImgbyRect(cv::vector<ImageKeeper>::iterator begin,cv::vector<ImageKeeper>::iterator end,cv::Mat out){

	if(end-begin!=numCam){
		return -1;
	}

	cv::Rect rect;
	rect.width = begin[0].getIMG().cols;
	rect.height = begin[0].getIMG().rows;

	for(int i=0;i<end-begin;i++){
		rect.x = (i%3)*rect.width;
		rect.y = (int)(i/3)*rect.height;

		cv::Mat roi(out,rect);
		begin[i].getIMG().copyTo(roi);		
	}
}
#endif