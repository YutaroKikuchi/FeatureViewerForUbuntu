#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "ImageKeeper.h"
#include "Drawer.h"
#include "FeaturePoint.h"
//#include "Reader.h"
#include "LineReader.h"
#include "FeatureViewer.h"
#include "ImageViewer.h"

#define LGH 4
#define CAM 2


#define NVM "./multicam/nvm/passage6.nvm"
#define IMG "./multicam/"


void checkCommand(int* output,char key){

	if(key == 'n'){
	}else if(key == 'p'){
		*output -=2;
	}else{
		*output -= 1;
	}
}


/*
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
}*/


int main(int argc ,char* argv[]){
  
	int numpic;

	std::string nvmpath = NVM;
	std::string imgpath = IMG;

	ImageViewer imgv(CAM,LGH);
	Reader reader(CAM,IMG,NVM);

	Drawer drawer;

	LineReader lr(NVM);

	lr.setLineNo();
	lr.showLines();

	reader.setImg(lr.startImg[0],lr.endImg[0]);
	reader.setFeaturePoint(lr.startFeature[0],lr.endFeature[0]);
	reader.setImg(lr.startImg[1],lr.endImg[1]);
	reader.setFeaturePoint(lr.startFeature[1],lr.endFeature[1]);

  
  

	numpic = lr.endImg[0] - lr.startImg[0]+1 + lr.endImg[1]-lr.startImg[1]+1;

	cv::namedWindow("hoge",CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);
	for(int i=LGH;i<numpic;i++){

		//std::cout << i <<std::endl;

		std::vector<ImageKeeper> drawnCam1;
		std::vector<ImageKeeper> drawnCam2;

		drawnCam1.push_back(reader.getIKbyID(1,i));
		drawnCam2.push_back(reader.getIKbyID(2,i));

/*
    std::cout <<"cam1.row="<<drawnCam1[0].getRows()<<" cam1.col="<< drawnCam1[0].getCols() <<" cam2.row=" << drawnCam2[0].getRows() << " cam2.col=" << drawnCam2[0].getCols()<<std::endl;
*/

		if(drawnCam1[0].getID() == -1 && drawnCam2[0].getID() == -1){
			continue;
	    	}else{
			ImageKeeper buff(-1,0,"black",cv::Mat::zeros(1080,1920,CV_8UC3));

			if(drawnCam1[0].getID()==-1){
				drawnCam1[0]=buff;
				drawnCam1[0].setIMG(cv::Mat::zeros(1080,1920,CV_8UC3));
	    		}

			if(drawnCam2[0].getID()==-1){
				drawnCam2[0]=buff;
				drawnCam2[0].setIMG(cv::Mat::zeros(1080,1920,CV_8UC3));
			}
		}

		for(int j=1;j<LGH;j++){
			drawnCam1.push_back(reader.getIKbyID(1,i-j));
			drawnCam2.push_back(reader.getIKbyID(2,i-j));
		}

		drawer.DrawPoints(drawnCam1[0]);
		drawer.DrawPoints(drawnCam2[0]);

		std::vector<ImageKeeper> preimg;
		preimg.push_back(drawnCam1[1]);
		preimg.push_back(drawnCam2[1]);

		std::vector<bool> flagCam1(drawnCam1[0].getPointsSize()),flagCam2(drawnCam2[0].getPointsSize());

		drawnCam2[1].getFeatureFlags(drawnCam1[0],flagCam1);
		drawnCam1[1].getFeatureFlags(drawnCam2[0],flagCam2);
	
		drawer.DrawRoute(drawnCam1.begin(),drawnCam1.end(),drawnCam1[0].getIMG(),flagCam1);
		drawer.DrawRoute(drawnCam2.begin(),drawnCam2.end(),drawnCam2[0].getIMG(),flagCam2);

		std::vector<ImageKeeper> viewedimg;
		viewedimg.push_back(drawnCam1[0]);
		viewedimg.push_back(drawnCam2[0]);

		imgv.showImgsbyLine(viewedimg.begin(),viewedimg.end(),preimg.begin(),preimg.end());

		std::cout << "Cam1:" << viewedimg[0].getName() << " Cam2:" << viewedimg[1].getName() << std::endl;

		checkCommand(&i,cv::waitKey(0));
    
	}
        
  return 0;
}
