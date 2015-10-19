#ifndef FEATUREVIEWER
#define FEATUREVIEWER

#include <iostream>
#include <opencv2/core/core.hpp>
#include "Drawer.h"
#include "Reader.h"

#define LGH 3
#define INTER 1

/*
class FeatureViewer{
private:
	std::string windowname;
	Drawer drawer;
	Reader reader;
public:
	FeatureViewer(std::string npath,std::string ppath,std::string wname){ 
		Reader hoge(ppath,npath);
		reader = hoge;
		windowname = wname;
	}

	FeatureViewer(std::string npath,std::string ppath){
		Reader hoge(ppath,npath);
		reader=hoge;
		windowname = "noname";
	}

	void setWindowName(std::string in){
		windowname = in;
	}

	void setImgs(int startImg,int endImg,int startFeature,int endFeature){
		reader.setImg(startImg,endImg);
		reader.setFeaturePoint(startFeature,endFeature);

		int hoge;
		std::cin >> hoge;

		int numpic = reader.ik.size();
		
		for(int i=0;i<numpic;i++){
			ImageKeeper drawn = reader.getIKbyID(i);
			if(drawn.getID() != -1){
				drawer.DrawPoints(reader.getIKbyID(i));
			}
		}

		for(int i=5;i<numpic;i++){
			std::vector<ImageKeeper> iks;
			for(int j=0;j<LGH;j++){
				iks.reserve(LGH);
				iks.push_back(reader.getIKbyID(reader.imgLink[i-j]));
			}
			drawer.DrawRoute(iks.begin(),iks.end(),reader.getIKbyID(i).getIMG());
			iks.clear();
		}
	}

	void setWindow(){
		cv::namedWindow(windowname,CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);
	}

	void showImgs(int i){
		imshow(windowname,reader.getIKbyID(i).getIMG());
	}

	int getNumOfImg(){
		return reader.ik.size();
	}

	void hogehoge(){

		std::sort(reader.featureindex.begin(),reader.featureindex.end());
		std::ofstream ofs("./hogehoge.txt");

		for(int i=0;i<reader.featureindex.size();i++){
			ofs << "featureindex:" << reader.featureindex[i] << std::endl;
		}
	}
	};*/

#endif
