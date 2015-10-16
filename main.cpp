#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "ImageKeeper.h"
#include "Drawer.h"
#include "FeaturePoint.h"
#include "Reader.h"
#include "LineReader.h"
#include "FeatureViewer.h"
#include "ImageViewer.h"

#define LGH 4
#define CAM 2


#define NVM "./passage6_1980_f30_dist/nvm/passage6_param.nvm"
#define IMG "./passage6_1980_f30_dist/"


void checkCommand(int* output,char key){

	if(key == 'n'){
	}else if(key == 'p'){
		*output -=2;
	}else{
		*output -= 1;
	}
}



int main(){

	Drawer drawer;
	ImageViewer imgviewer(CAM,LGH);

	std::vector<ImageKeeper> cam1,cam2;
	int x = 30,y=30,length1 = 200, length2 =190;

	for(int i=0;i<15;i++){
		ImageKeeper buff,buff2;
		buff.setIMG(cv::Mat::zeros(256,256,CV_8UC3));
		buff2.setIMG(cv::Mat::zeros(256,256,CV_8UC3));
		buff2.setID(i);
		buff.setID(i);
		cam2.push_back(buff2);

		switch(i%4){
		case 0:
			y+=length1;
			break;
		case 1:
			x+=length1;
			length1 = length1*(1-0.03*((i+3)/4));
			break;
		case 2:
			y-=length2;
			break;
		case 3:
			x-=length2;
			length2 = length2*(1-0.03*((i+1)/4));
			break;
		default:
			break;
		}

		buff.setFeature(1,(float)x,(float)y);

		cam1.push_back(buff);
	}

	for(int i=15;i<30;i++){
		ImageKeeper buff,buff2;
		buff.setIMG(cv::Mat::zeros(256,256,CV_8UC3));
		buff2.setIMG(cv::Mat::zeros(256,256,CV_8UC3));
		buff.setID(i);
		buff2.setID(i);
		cam1.push_back(buff2);

		switch(i%4){
		case 0:
			y+=length1;
			break;
		case 1:
			x+=length1;
			length1 = length1*(1-0.03*((i+3)/4));
			break;
		case 2:
			y-=length2;
			break;
		case 3:
			x-=length2;
			length2 = length2*(1-0.03*((i+1)/4));
			break;
		default:
			break;
		}

		buff.setFeature(1,x,y);

		cam2.push_back(buff);
	}


	cv::namedWindow("hoge",CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);

	for(int i=LGH;i<30;i++){
		ImageKeeper shownImg1 = cam1[i],shownImg2 = cam2[i];

		drawer.DrawPoints(shownImg1);
		drawer.DrawPoints(shownImg2);

		std::vector<ImageKeeper> drawn1;
		std::vector<ImageKeeper> drawn2;

		for(int j=0;j<LGH;j++){
			drawn1.push_back(cam1[i-j]);
			drawn2.push_back(cam2[i-j]);
		}

		std::cout << "cam1[" << i << "]" << std::endl;
		cam1[i].showFeatures();
		std::cout << "cam2[" << i << "]" << std::endl;
		cam2[i].showFeatures();
		std::cout << "-----------------------------" << std::endl;

		drawer.DrawRoute(drawn1.begin(),drawn1.end(),cam1[i].getIMG());
		drawer.DrawRoute(drawn2.begin(),drawn2.end(),cam2[i].getIMG());

		std::vector<ImageKeeper> viewed;
		viewed.push_back(cam1[i]);
		viewed.push_back(cam2[i]);

		std::vector<ImageKeeper> preimg;
		preimg.push_back(cam1[i-1]);
		preimg.push_back(cam2[i-1]);

		imgviewer.showImgsbyLine(viewed.begin(),viewed.end(),preimg.begin(),preimg.end());

		checkCommand(&i,cv::waitKey(0));
	}
}

/*
int main(){

	int numpic;

	std::string nvmpath = NVM;
	std::string imgpath = IMG;

<<<<<<< HEAD

	ImageViewer imgv(12);
	Reader reader(IMG,NVM);
=======
	Reader reader(imgpath,nvmpath);
>>>>>>> master
	Drawer drawer;

	LineReader lr(NVM);

	lr.setLineNo();
	lr.showLines();

	reader.setImg(lr.startImg[0],lr.endImg[0]);
	reader.setFeaturePoint(lr.startFeature[0],lr.endFeature[0]);
	reader.setImg(lr.startImg[1],lr.endImg[1]);
	reader.setFeaturePoint(lr.startFeature[1],lr.endFeature[1]);

	cv::namedWindow("hoge",CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);

	for(int i=LGH;i<reader.ik.size();i++){
		ImageKeeper shownImg = reader.getIKbyID(i);

		std::vector<ImageKeeper> hoge;

		if(shownImg.getID() != -1){
			drawer.DrawPoints(shownImg);

			for(int j=0;j<LGH;j++){
				hoge.push_back(reader.getIKbyID(i-j));
			}
			drawer.DrawRoute(hoge.begin(),hoge.end(),reader.getIKbyID(i).getIMG());
			
			cv::vector<ImageKeeper> viewed;

			for(int k=0;k<12;k++){
				viewed.push_back(reader.getIKbyID(i));
			}

			imgv.showImgsbyRect(viewed.begin(),viewed.end());

			std::cout << "ID:" << reader.getIKIndexbyID(i) << " Name:" << shownImg.getName() << std::endl;
<<<<<<< HEAD
			checkCommand(&i,cv::waitKey(100));
		}
	}

	return 0;
}*/

