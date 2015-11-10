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
	std::string windowName;

	int combineImgTheta(std::vector<ImageKeeper> &imgs, cv::Mat out);	//imgsの画像を指定された形式に結合し，１つの画像とする
	int DrawLinesTheta(std::vector<ImageKeeper> &imgs, std::vector<ImageKeeper> &preimgs, cv::Mat out);	//imgs と preimgs間に同じIDを持つ特徴点が存在したら，その座標を結ぶ線分を描写する
	int matchPoint(ImageKeeper from, ImageKeeper to, std::vector<cv::Point2f> &frompoints, std::vector<cv::Point2f> &topoints);	//２つの画像を比較し，同じIDを持つ特徴点が存在した場合，それぞれの座標を配列で返す．

public:
	ImageViewer(int numcam,int len, std::string windowname){
		numCam = numcam;
		lengh = len;
		windowName = windowname;
	}

	void setWindow();	//ウィンドウの設定を行う
	int showImgsTheta(std::vector<ImageKeeper> &imgs, std::vector<ImageKeeper> &preimgs);	//ベクトルimgsに格納されている画像すべてを表示する．
};



void ImageViewer::setWindow(){
	cv::namedWindow(windowName,CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);
}

int ImageViewer::showImgsTheta(std::vector<ImageKeeper> &imgs, std::vector<ImageKeeper> &preimgs){

	//入力画像を格納するための画像
	cv::Mat output(cv::Size(imgs[0].getIMG().cols*6,imgs[0].getIMG().rows*3),CV_8UC3);

	//入力画像を結びつける
	combineImgTheta(imgs, output);

	DrawLinesTheta(imgs, preimgs, output);

	cv::imshow(windowName,output);
	return 0;
}

int ImageViewer::combineImgTheta(std::vector<ImageKeeper> &imgs, cv::Mat out){

	if(imgs.size() !=numCam)
		return -1;

	cv::Rect rect;
	rect.width = imgs[0].getIMG().cols;
	rect.height = imgs[0].getIMG().rows;

	//表示のしかたが特殊なため，画像の位置に対応させたcase文. iが行でjが列の座標を表している
	for(int i=0; i < 3; i++){

		for(int j = 0; j < 6; j++){
			rect.x = j*rect.width;
			rect.y = i*rect.height;

			cv::Mat roi(out,rect);

			switch(i){
				case 0:
					switch(j){
						case 0:
							imgs[5].getIMG().copyTo(roi);
							break;
						case 1:
							imgs[4].getIMG().copyTo(roi);
							break;
						case 3:
							imgs[10].getIMG().copyTo(roi);
							break;
						case 4:
							imgs[11].getIMG().copyTo(roi);
							break;
						default:
							//画像を割り当てられない位置には黒色の画像を割り当てる
							roi = cv::Mat::zeros(rect.width,rect.height,CV_8UC3);;
					}
					break;
				case 1:
					switch(j){
						case 0:
							imgs[3].getIMG().copyTo(roi);
							break;
						case 1:
							imgs[0].getIMG().copyTo(roi);
							break;
						case 2:
							imgs[1].getIMG().copyTo(roi);
							break;
						case 3:
							imgs[9].getIMG().copyTo(roi);
							break;
						case 4:
							imgs[6].getIMG().copyTo(roi);
							break;
						case 5:
							imgs[7].getIMG().copyTo(roi);
							break;
						default:
							roi = cv::Mat::zeros(rect.width,rect.height,CV_8UC3);;
					}
					break;
				case 2:
					switch(j){
						case 1:
							imgs[2].getIMG().copyTo(roi);
							break;
						case 4:
							imgs[8].getIMG().copyTo(roi);
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

int ImageViewer::DrawLinesTheta(std::vector<ImageKeeper> &imgs, std::vector<ImageKeeper> &preimgs, cv::Mat out){

	float col = (float)imgs[0].getCols(); float row = (float)imgs[0].getRows();

	for(int i = 0;i < imgs.size(); i++){

		for(int j = 0; j < imgs.size(); j++){
			if(i == j){
				continue;
			}

			std::vector<cv::Point2f> frompoints,topoints;
			matchPoint(preimgs[i], imgs[j], frompoints, topoints);

		
			for(int k = 0; k < frompoints.size(); k++){
				cv::Point2f offsetFrom,offsetTo;

				//各画像が表示される位置に合わせて，調整する
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
		    

				if(rand()%3 == 0){	//線の色が１色だとわかりづらいので，ランダムに色を変化させる
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

	for(int i=0;i<from.getFeaturesSize();i++){
		int fromFID = from.getFeatureID(i);
		cv::Point2f point = to.getFeaturebyID(fromFID);

		//std::cout << "  getPointID(" << fromFID << ");" << std::endl;
		//std::cout << "  point = " << point << std::endl;

		//変数pointが(0.0,0.0)（つまり変数fromFIDと同じIDをもつ特徴点が存在しない）である場合は実行されない
		if(point != cv::Point2f(0.0,0.0)){
			frompoints.push_back(from.getFeaturebyID(fromFID));	//fromの座標はfrompointtsに
			topoints.push_back(point);	//toの座標はtopointsにプッシュされる
		}
	}

	return 0;
}
#endif
