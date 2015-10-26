#ifndef IMGVIEWER
#define IMGVIEWER

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <stdio.h>
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
	int DrawLinesTheta(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end,std::vector<ImageKeeper>::iterator prebegin,std::vector<ImageKeeper>::iterator preend,cv::Mat out);

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

	int showImgsTheta(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end,std::vector<ImageKeeper>::iterator prebegin,std::vector<ImageKeeper>::iterator preend){

		cv::Mat output(cv::Size(begin[0].getIMG().cols*6,begin[0].getIMG().rows*3),CV_8UC3);

		combineImgTheta(begin,end,output);

		DrawLinesTheta(begin,end,prebegin,preend,output);

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
							roi = cv::Mat::zeros(rect.width,rect.height,CV_8UC3);;
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
							roi = cv::Mat::zeros(rect.width,rect.height,CV_8UC3);;
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
							roi = cv::Mat::zeros(rect.width,rect.height,CV_8UC3);;
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

int ImageViewer::DrawLinesTheta(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end,std::vector<ImageKeeper>::iterator prebegin,std::vector<ImageKeeper>::iterator preend,cv::Mat out){

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
		    cv::Point2f offsetFrom,offsetTo;
		    
		    switch(i){
			case 0:
				offsetFrom = cv::Point2f(col,row);
				break;
			case 1:
				offsetFrom = cv::Point2f(col*2,row);
				break;
			case 2:
				offsetFrom = cv::Point2f(col,row*2);
				break;
			case 3:
				offsetFrom = cv::Point2f(0,row);
				break;
			case 4:
				offsetFrom = cv::Point2f(col,0);
				break;
			case 5:
				offsetFrom = cv::Point2f(0,0);
				break;
			case 6:
				offsetFrom = cv::Point2f(col*4,row);
				break;
			case 7:
				offsetFrom = cv::Point2f(col*5,row);
				break;
			case 8:
				offsetFrom = cv::Point2f(col*4,row*2);
				break;
			case 9:
				offsetFrom = cv::Point2f(col*3,row);
				break;
			case 10:
				offsetFrom = cv::Point2f(col*3,0);
				break;
			case 11:
				offsetFrom = cv::Point2f(col*4,0);
				break;
			default:
				break;
		    }

		    switch(j){
			case 0:
				offsetTo = cv::Point2f(col,row);
				break;
			case 1:
				offsetTo = cv::Point2f(col*2,row);
				break;
			case 2:
				offsetTo = cv::Point2f(col,row*2);
				break;
			case 3:
				offsetTo = cv::Point2f(0,row);
				break;
			case 4:
				offsetTo = cv::Point2f(col,0);
				break;
			case 5:
				offsetTo = cv::Point2f(0,0);
				break;
			case 6:
				offsetTo = cv::Point2f(col*4,row);
				break;
			case 7:
				offsetTo = cv::Point2f(col*5,row);
				break;
			case 8:
				offsetTo = cv::Point2f(col*4,row*2);
				break;
			case 9:
				offsetTo = cv::Point2f(col*3,row);
				break;
			case 10:
				offsetTo = cv::Point2f(col*3,0);
				break;
			case 11:
				offsetTo = cv::Point2f(col*4,0);
				break;
			default:
				break;
		    }
		    cv::Point2f from = frompoints[k]+offsetFrom;
		    cv::Point2f to = topoints[k]+offsetTo;
		    

		    if(rand()%3 == 0){
		    	cv::line(out,from,to,cv::Scalar(0.0,0.0,200.0),2,CV_AA);
		    }else if(rand()%3 == 1){
			cv::line(out,from,to,cv::Scalar(0.0,200.0,0.0),2,CV_AA);
		    }else{
			cv::line(out,from,to,cv::Scalar(200.0,0.0,0.0),2,CV_AA);
		    }


			
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
