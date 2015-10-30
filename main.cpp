#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <string.h>

#include "ImageKeeper.h"
#include "Drawer.h"
#include "FeaturePoint.h"
//#include "Reader.h"
#include "LineReader.h"
#include "FeatureViewer.h"
#include "ImageViewer.h"

#define CAM 2

#define W 480
#define H 480


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

int readPath(char* in, std::string *imgpath, std::string *nvmpath,int *cam, int *lengh){

	std::ifstream ifs(in);
	std::string buff;

	if(ifs.fail()){
		std::cout << "File Not Found" << std::endl;
		return 0;
	}else{
		while(std::getline(ifs,buff)){
			if(strstr(buff.c_str(),"data_dir")){
				size_t pos1 = buff.find("\""), pos2 = buff.rfind("\"");
				*imgpath = buff.substr(pos1 + 1, pos2 - pos1 - 1);
			}

			if(strstr(buff.c_str(),"nvm_dir")){
				size_t pos1 = buff.find("\""), pos2 = buff.rfind("\"");
				*nvmpath = buff.substr(pos1 + 1, pos2 - pos1 - 1);
			}

			if(strstr(buff.c_str(),"pertition_number")){
				size_t pos1 = buff.find("\""), pos2 = buff.rfind("\"");
				*cam = std::stoi(buff.substr(pos1 + 1, pos2 - pos1 - 1));
			}

			if(strstr(buff.c_str(),"rought_lengh")){
				size_t pos1 = buff.find("\""), pos2 = buff.rfind("\"");
				*lengh = std::stoi(buff.substr(pos1 + 1, pos2 - pos1 - 1));
			}
		}
	}

	return 1;
}

/*
int main(){

	if(false){
		std::cout << "Invalid Argument !!!" << std::endl;
		return 0;
	}

	std::string nvmpath;
	std::string imgpath;
	int lengh,cam;

	if(readPath("./input.txt",&imgpath,&nvmpath,&cam,&lengh)==0){
		return 0;
	}

	int numpic=0;

	ImageViewer imgv(cam,lengh);
	Reader reader(cam,imgpath,nvmpath);

	Drawer drawer;

	LineReader lr(nvmpath);

	lr.setLineNo();
	lr.showLines();

	for(int i=0;i<lr.endFeature.size();i++){
		reader.setImg(lr.startImg[i],lr.endImg[i]);
		reader.setFeaturePoint(lr.startFeature[i],lr.endFeature[i]);
	}

	for(int i=0;i<lr.endFeature.size();i++){
		numpic += lr.endImg[i] - lr.startImg[i] + 1;
	}

	std::cout << "Num of Imgs:" << numpic << std::endl;

	cv::namedWindow("hoge",CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);

	for(int i=0;i<numpic;i++){
		
		std::cout << "ImageIndex:" <<i<< " name:" << reader.ik[i].getName() << std::endl;
		drawer.DrawPoints(reader.ik[i]);
		cv::imshow("hoge",reader.ik[i].getIMG());
		cv::waitKey(0);
	}	
}
*/

/*
int main(){
	ImageViewer imgv(12,4);

	std::vector<ImageKeeper> lena(12);
	for(int i=0;i<12;i++){
		ImageKeeper buff(0,i,"lena.jpg",cv::imread("./lena.jpg",1));
		lena[i] = buff;
	}

	cv::namedWindow("hoge",CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);
	imgv.showImgsTheta(lena.begin(),lena.end());
	cv::waitKey(0);
}*/

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

	if(argc != 2){
		std::cout << "Invalid Argument !!!" << std::endl;
		return 0;
	}

	std::string nvmpath;
	std::string imgpath;
	int lengh,cam;

	if(readPath(argv[1],&imgpath,&nvmpath,&cam,&lengh)==0){
		return 0;
	}

	int numpic=0;

	ImageViewer imgv(cam,lengh);
	Reader reader(cam,imgpath,nvmpath);

	Drawer drawer;

	LineReader lr(nvmpath);

	lr.setLineNo();
	lr.showLines();

	for(int i=0;i<lr.endFeature.size();i++){
		reader.setImg(lr.startImg[i],lr.endImg[i]);
		reader.setFeaturePoint(lr.startFeature[i],lr.endFeature[i]);
	}

	for(int i=0;i<lr.endFeature.size();i++){
		numpic += lr.endImg[i] - lr.startImg[i] + 1;
	}

	std::cout << "Num of Imgs:" << numpic << std::endl;

	std::cout << "Name:" << reader.ik[lengh].getName() << " Row:" << reader.ik[lengh].getRows() << " Col" << reader.ik[lengh].getCols() << std::endl;

	cv::namedWindow("hoge",CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);

	for(int i=lengh;i<(numpic/12+1);i++){

		std::vector<std::vector<ImageKeeper>> drawnCam(cam);

		for(int j=1;j<=cam;j++){
			for(int k=0 ;k<lengh;k++)
				drawnCam[j-1].push_back(reader.getIKbyID(j,i-k));
		}
		
		bool contflag = true;
		for(int j=0;j<cam;j++){
			contflag = contflag && (drawnCam[j][0].getID() == -1);
		}

		for(int j=0;j<cam;j++){
			if(drawnCam[j][0].getID() == -1){
				ImageKeeper buff(-1,0,"No Image",cv::Mat::zeros(H,W,CV_8UC3));
				drawnCam[j][0] = buff;
			}
		} 

		std::vector< std::vector<bool> > flagCam(cam);

		for(int j=0;j<cam;j++){

			drawer.DrawPoints(drawnCam[j][0]);
			flagCam[j].reserve(drawnCam[j][0].getFeaturesSize());
		}

		for(int j=0;j<cam;j++){
			for(int k=0;k<cam;k++){
               			if(j!=k){
                    			drawnCam[k][1].getFeatureFlags(drawnCam[j][0],flagCam[j]);
                		}
            		}
		}

		for(int j=0;j<cam;j++){
			drawer.DrawRoute(drawnCam[j],drawnCam[j][0].getIMG(),flagCam[j]);
		}

		std::vector<ImageKeeper> preimg;
		std::vector<ImageKeeper> viewedimg;

		for(int j=0;j<cam;j++){
			preimg.push_back(drawnCam[j][1]);
			viewedimg.push_back(drawnCam[j][0]);
		}
/*
	for(int j=0;j<cam;j++){
		std::cout << "Cam1.name:" << viewedimg[j].getName() << std::endl;
	}
*/



		imgv.showImgsTheta(viewedimg.begin(),viewedimg.end(),preimg.begin(),preimg.end());

		checkCommand(&i,cv::waitKey(0));
	}

	return 0;
}

/*
int main(int argc ,char* argv[]){
  
	int numpic;

	std::string nvmpath = NVM;
	std::string imgpath = IMG;

	ImageViewer imgv(CAM,LGH);
	Reader reader(CAM,IMG,NVM);

	Drawer drawer;

	LineReader 
lr(NVM);

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
/*
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
*/
