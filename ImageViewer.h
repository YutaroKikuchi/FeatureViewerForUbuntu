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
	int lengh;
	int combineImgbyLine(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end,cv::Mat out);
	int combineImgbyRect(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end,cv::Mat out);
	int DrawLine(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end,cv::Mat out);
	int DrawLinesinLine(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end,std::vector<ImageKeeper>::iterator prebegin,std::vector<ImageKeeper>::iterator preend,cv::Mat out);
	int matchPoint(ImageKeeper from,ImageKeeper to,std::vector<cv::Point2f> &frompoints,std::vector<cv::Point2f> &topoints);
	int combineImgTheta(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end,cv::Mat out);

public:
	ImageViewer(int numcam,int len){
		numCam = numcam;
		lengh = len;
	}

	int setWindow(){
		cv::namedWindow("hoge",CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);
	}

	int showImgsbyLine(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end,std::vector<ImageKeeper>::iterator prebegin,std::vector<ImageKeeper>::iterator preend){

		cv::Mat output(cv::Size(begin[0].getIMG().cols*numCam,begin[0].getIMG().rows),CV_8UC3);

		combineImgbyLine(begin,end,output);

		DrawLinesinLine(begin,end,prebegin,preend,output);

		int hoge;
		//std::cout << "width;" << output.cols << " height:" << output.rows << std::endl;
		//std::cin >> hoge;
		
		cv::imshow("hoge",output);
		//cv::waitKey(0);

		return 0;
	}

	int showImgsbyRect(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end){

		cv::Mat output(cv::Size(begin[0].getIMG().cols*3,begin[0].getIMG().rows*(int)(numCam/3)),CV_8UC3);

		combineImgbyRect(begin,end,output);
		cv::imshow("hoge",output);

		cv::waitKey(0);
		return 0;
	}

	int showImgsTheta(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end){

		cv::Mat output(cv::Size(begin[0].getIMG().cols*6,begin[0].getIMG().rows*3),CV_8UC3);

		combineImgTheta(begin,end,output);

		cv::imshow("hoge",output);
		return 1;
	}

};

int ImageViewer::combineImgbyLine(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end,cv::Mat out){
		
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

int ImageViewer::combineImgbyRect(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end,cv::Mat out){

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

int ImageViewer::combineImgTheta(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end,cv::Mat out){

	if(end - begin !=numCam)
		return -1;

	cv::Rect rect;
	rect.width = begin[0].getIMG().cols;
	rect.height = begin[0].getIMG().rows;

	cv::Mat black = cv::Mat::zeros(rect.width,rect.height,CV_8UC3);

	for(int i=0;i<3;i++){

		for(int j=0;j<6;j++){
			rect.x = j*rect.width;
			rect.y = i*rect.height;

			cv::Mat roi(out,rect);

			switch(i){
				case 0:
					switch(j){
						case 0:
							begin[5].getIMG().copyTo(roi);
							break;
						case 1:
							begin[4].getIMG().copyTo(roi);
							break;
						case 3:
							begin[10].getIMG().copyTo(roi);
							break;
						case 4:
							begin[11].getIMG().copyTo(roi);
							break;
						default:
							roi = black;
					}
					break;
				case 1:
					switch(j){
						case 0:
							begin[3].getIMG().copyTo(roi);
							break;
						case 1:
							begin[0].getIMG().copyTo(roi);
							break;
						case 2:
							begin[1].getIMG().copyTo(roi);
							break;
						case 3:
							begin[9].getIMG().copyTo(roi);
							break;
						case 4:
							begin[6].getIMG().copyTo(roi);
							break;
						case 5:
							begin[7].getIMG().copyTo(roi);
							break;
						default:
							roi = black;
					}
					break;
				case 2:
					switch(j){
						case 1:
							begin[2].getIMG().copyTo(roi);
							break;
						case 4:
							begin[8].getIMG().copyTo(roi);
							break;
						default:
							roi = black;
					}
					break;
				default:
					break;
			}

		}

	}
}

int ImageViewer::DrawLinesinLine(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end,std::vector<ImageKeeper>::iterator prebegin,std::vector<ImageKeeper>::iterator preend,cv::Mat out){

	float col = (float)begin[0].getCols(); float row = (float)begin[0].getRows();

	for(int i=0;i<end-begin;i++){

		for(int j=0;j<end-begin;j++){
		  if(i==j){
		    continue;
		  }
		  std::vector<cv::Point2f> frompoints,topoints;
		  matchPoint(prebegin[i],begin[j],frompoints,topoints);
		 
		/* 
		  if(frompoints.size()>0 && topoints.size()>0){
		    std::cout << "Match" << std::endl;
		  }else{
		    std::cout << "no Match" << std::endl;
		  }

		  std::cout << "begin[" << i << "]  " << "begin[" << j << "]" << std::endl;*/

		  for(int k=0;k<frompoints.size();k++){
		    cv::Point2f from = frompoints[k]+cv::Point2f(col*i,0.0);
		    cv::Point2f to = topoints[k]+cv::Point2f(col*j,0.0);
		    
		    cv::line(out,from,to,cv::Scalar(0.0,0.0,200.0),2,CV_AA);
		  }
		}

	}
	return 0;
}

int ImageViewer::matchPoint(ImageKeeper from,ImageKeeper to,std::vector<cv::Point2f> &frompoints,std::vector<cv::Point2f> &topoints){

  //std::cout << " start MatchPoint" << std::endl;

	for(int i=0;i<from.getPointsSize();i++){
		int fromFID = from.getFeatureID(i);
		cv::Point2f point = to.getPointID(fromFID);

		//std::cout << "  getPointID(" << fromFID << ");" << std::endl;
		//std::cout << "  point = " << point << std::endl;

		if(point != cv::Point2f(0.0,0.0)){
			frompoints.push_back(from.getPointID(fromFID));
			topoints.push_back(point);
		}
	}

	return 0;
}
#endif
