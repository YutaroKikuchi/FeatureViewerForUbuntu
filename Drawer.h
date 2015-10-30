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

public:
	cv::Mat DrawPoints(ImageKeeper img);
	cv::Mat DrawPoint(cv::Point2f point,cv::Mat img);
	void DrawRoute(std::vector<ImageKeeper> &imgs, cv::Mat out, std::vector<bool> &flags);

};

cv::Mat Drawer::DrawPoints(ImageKeeper img){		//入力画像に特徴点ごとに円を描写した画像を返す．
	cv::Mat out;

	int pointSize = img.getFeaturesSize();

	for(int i=0;i<pointSize;i++){
		out = DrawPoint(img.getFeature(i),img.getIMG());
	}

	return out;
}

cv::Mat Drawer::DrawPoint(cv::Point2f point,cv::Mat img){
		                                       
	cv::Mat out;

	cv::circle(img,point,15,cv::Scalar(0,0,200),2,4);

	out = img;

	return out;
}

void Drawer::DrawRoute(std::vector<ImageKeeper> &imgs, cv::Mat out, std::vector<bool> &flags){

	int currentID = imgs[0].getID(), curPSize = imgs[0].getFeaturesSize();

	for(int i=0;i<curPSize;i++){
		int toFID = imgs[0].getFeatureID(i);

		if(flags[i]==false){

			for(int j=0;imgs.begin()+j+1!=imgs.end();j++){
				cv::Point2f to = imgs[j].getFeaturebyID(toFID);
				cv::Point2f from = imgs[j+1].getFeaturebyID(toFID);
				if(from == cv::Point2f(0.0,0.0)){
					break;
				}else{
					cv::line(out,to,from,cv::Scalar(0,0,200),3,8);
				}
			}
		}
	}
}


#endif
