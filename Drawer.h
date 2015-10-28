#ifndef DRAWER
#define DRAWER

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "FeaturePoint.h"
#include "ImageKeeper.h"

#define POINT 2;

class Drawer{

private:
	void DrawLine(cv::Mat src,cv::Point2f from,cv::Point2f to, float alpha);
	void DrawLine(cv::Mat src,cv::Point2f from,cv::Point2f to);
	void CombineImg(cv::Mat right,cv::Mat left,cv::Mat out);
	int findID(std::vector<int>::iterator begin,std::vector<int>::iterator end,int targetID,int diff);

public:
	cv::Mat DrawPoint(cv::Point2f point,cv::Mat img){
		                                       
		cv::Mat out;

		cv::circle(img,point,15,cv::Scalar(0,0,200),2,4);

		out = img;

		return out;
	}
	
	cv::Mat DrawPoints(ImageKeeper img){
		cv::Mat out;

		int pointSize = img.getFeaturesSize();

		for(int i=0;i<pointSize;i++){
			out = DrawPoint(img.getFeature(i),img.getIMG());
		}

		return out;
	}

	void DrawRoute(ImageKeeper current,ImageKeeper prev, cv::Mat out){

		int currentID = current.getID(), curPSize = current.getFeaturesSize();
		int prevID = prev.getID(),prevPSize = prev.getFeaturesSize();

		for(int i=0;i<curPSize;i++){
			for(int j=0;j<prevPSize;j++){
				if(current.getFeatureID(i) == prev.getFeatureID(j)){
					cv::line(out,current.getFeature(i),prev.getFeature(j),cv::Scalar(0,0,200),2,8);
				}
			}
		}
	}


	void DrawRoute(std::vector<ImageKeeper>::iterator iksBegin,std::vector<ImageKeeper>::iterator iksEnd,cv::Mat out){

		int currentID = iksBegin[0].getID(), curPSize = iksBegin[0].getFeaturesSize();

		for(int i=0;i<curPSize;i++){
			int toFID = iksBegin[0].getFeatureID(i);

			for(int j=0;iksBegin+j+1!=iksEnd;j++){
				cv::Point2f to = iksBegin[j].getFeaturebyID(toFID);
				cv::Point2f from = iksBegin[j+1].getFeaturebyID(toFID);
				if(from == cv::Point2f(0.0,0.0)){
					break;
				}else{
					cv::line(out,to,from,cv::Scalar(0,0,200),3,8);
				}
			}
		}
	}


	void DrawRoute(std::vector<ImageKeeper>::iterator iksBegin,std::vector<ImageKeeper>::iterator iksEnd,cv::Mat out,std::vector<bool> &flags){

		int currentID = iksBegin[0].getID(), curPSize = iksBegin[0].getFeaturesSize();

		for(int i=0;i<curPSize;i++){
			int toFID = iksBegin[0].getFeatureID(i);

			if(flags[i]==false){

				for(int j=0;iksBegin+j+1!=iksEnd;j++){
					cv::Point2f to = iksBegin[j].getFeaturebyID(toFID);
					cv::Point2f from = iksBegin[j+1].getFeaturebyID(toFID);
					if(from == cv::Point2f(0.0,0.0)){
						break;
					}else{
						cv::line(out,to,from,cv::Scalar(0,0,200),3,8);
					}
				}
			}
		}
	}

	cv::Mat DrawLines(ImageKeeper prev,ImageKeeper current){
		int currentID = current.getID(),curPSize = current.getFeaturesSize();
		int prevID = prev.getID(),prevPSize = prev.getFeaturesSize();

		cv::Mat combineImg(cv::Size(current.getIMG().cols*2,current.getIMG().rows),CV_8UC3);

		CombineImg(current.getIMG(),prev.getIMG(),combineImg);

		cv::Point2f offset(current.getIMG().cols,0.0);
		std::cout << "previousname:" << prev.getName() << std::endl << "prevPSize:" << prevPSize << std::endl <<std::endl;
		std::cout << "previousname:" << current.getName() << std::endl << "curPSize:" << curPSize << std::endl ;

		for(int i=0;i<curPSize;i++){
			for(int j=0;j<prevPSize;j++){
				if(current.getFeatureID(i)==prev.getFeatureID(j)){
					DrawLine(combineImg,current.getFeature(i)+offset,prev.getFeature(j));
				}
			}
		}

		return combineImg;
	}

	cv::Mat TestCombine(ImageKeeper prev,ImageKeeper current){
		int currentID = current.getID(),curPSize = current.getFeaturesSize();
		int prevID = prev.getID(),prevPSize = prev.getFeaturesSize();

		cv::Mat combineImg(cv::Size(current.getIMG().cols*2,current.getIMG().rows),CV_8UC3);

		CombineImg(current.getIMG(),prev.getIMG(),combineImg);
		cv::Point2f offset(current.getIMG().cols,0.0);
		std::cout << "previousname:" << prev.getName() << std::endl << "prevPSize:" << prevPSize << std::endl <<std::endl;
		std::cout << "previousname:" << current.getName() << std::endl << "curPSize:" << curPSize << std::endl ;

		return combineImg;
	}

	void TestDrawLine(cv::Mat src){
		DrawLine(src,cv::Point2f(30.0,40.0),cv::Point2f(100.0,100.0),1);

		cv::namedWindow("video",CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);

		cv::imshow("video",src);
		cv::waitKey(0);
	}

	void TestCombine(){
		cv::Mat hoge1 = cv::imread("./lena.jpg",1);
		cv::Mat hoge2 = hoge1;

		cv::Mat out(cv::Size(hoge1.cols*2,hoge1.rows),CV_8UC3);

		CombineImg(hoge1,hoge2,out);

		cv::namedWindow("video",CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);

		cv::imshow("video",out);
		cv::waitKey(0);

	}

};

void Drawer::DrawLine(cv::Mat src,cv::Point2f from,cv::Point2f to, float alpha){

	cv::line(src,from,to,cv::Scalar(0,0,200),3,8);
}

void Drawer::DrawLine(cv::Mat src,cv::Point2f from,cv::Point2f to){
	cv::line(src,from,to,cv::Scalar(0,0,200),2,4);
}

void Drawer::CombineImg(cv::Mat right,cv::Mat left,cv::Mat out){

	cv::Rect rect;

	rect.width = left.cols;
	rect.height = left.rows;

	rect.x = 0;
	rect.y = 0;

	cv::Mat roiLeft(out,rect);

	rect.width = right.cols;
	rect.height = right.rows;

	rect.x = right.cols;
	rect.y = 0;

	cv::Mat roiRight(out,rect);

	right.copyTo(roiRight);
	left.copyTo(roiLeft);	
}

int Drawer::findID(std::vector<int>::iterator begin,std::vector<int>::iterator end,int targetID,int diff){

	for(int i=0;begin!=end;begin++,i++){
		if(*begin == targetID){
			return *(begin+=diff);
		}
	}
}

#endif
