#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "ImageKeeper.h"
#include "Drawer.h"
#include "FeaturePoint.h"
#include "Reader.h"
#include "LineReader.h"
#include "FeatureViewer.h"

#define LGH 5


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

	int numpic;

	std::string nvmpath = NVM;
	std::string imgpath = IMG;

	Reader reader(imgpath,nvmpath);
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
			

			std::cout << "ID:" << reader.getIKIndexbyID(i) << " Name:" << shownImg.getName() << std::endl;
			cv::imshow("hoge",shownImg.getIMG());
			checkCommand(&i,(char)cv::waitKey(0));
		}
	}
}
