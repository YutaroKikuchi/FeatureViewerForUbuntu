#ifndef READER
#define READER

#include <sys/resource.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "./ImageKeeper.h"
#include "./FeaturePoint.h"

#define FREQ 1

class Reader{

private:
	unsigned long int currentID;
	unsigned long int prevID;
	unsigned long int currentFeatureID;
	int modelID;

	std::string imgpass;
	std::string nvmpass;

	std::vector<std::string> FeatureData;

	std::string getfileName(std::string in);
	void splitFeatureData(std::string in, std::vector<std::string> &FeatureData);
	void separateFeatureData(std::vector<std::string> &FeatureData, int *numimage, std::vector<int> &imageIndex, std::vector<int> &camID, std::vector<float> &xpoint, std::vector<float> &ypoint);
public:
	std::vector<ImageKeeper> ik;
	std::vector<int> keyFrameID;

	Reader(){}

	Reader(int cam,std::string img,std::string nvm){
		imgpass = img;
		nvmpass = nvm;
		currentID =0;
		currentFeatureID = 0;
		prevID = 0;
		modelID = 0;
	}

	void setImg(int numofCam,int startImg, int endImg);	//nvmファイルを指定した領域だけ読み込み，画像のデータとして格納する．
	int setFeaturePoint(int startline,int endline);	//nvmファイルを指定した領域だけ読み込み，特徴点のデータとして格納する．
	ImageKeeper getIKbyID(int cam,int in);
	void showFeatureData(std::string in);
	void sortKeyFrameID();

};

/*
void Reader::setImg(int startline, int endline){	//引数で指定したnvmファイルの部分を読み込む
	std::ifstream ifs(nvmpass.c_str());
	std::string buff;
	ImageKeeper imbuff;

	prevID = currentID;
	if(ifs.fail()){
		std::cout << "ERROR" << std::endl;
	}else{
		int lineno = 0;

		while(std::getline(ifs,buff)){		//最初の行から1行ずつ文字列を読み込む
			lineno++;			//読み込むごとに行数をカウントする

			if(lineno >= startline && lineno <= endline){	//行数が引数で定義した領域内にあったら，画像ファイルを読み込む
				std::string name = getfileName(buff);	//画像ファイルの名前を取得
				imbuff.setName(name);
				imbuff.setIDbyName();
				imbuff.setModelID(modelID);
				ik.push_back(imbuff);
				ik[currentID].setIMG(cv::imread(imgpass+imbuff.getName(),1));
				
				currentID++;
	      		}
				
		}

		modelID++;
	}
}*/


void Reader::setImg(int numofCam,int startImg, int endImg){


	for(int i=startImg; i <= endImg; i++){
		for(int j=0; j<numofCam; j++){
			for(int k=0; k<ik.size(); k++){
				if(ik[k].getID() == i && ik[k].getCamID() == j){

					std::string imgID;
					if(i>=0 && i<10){
						imgID = "000" + std::to_string(i);
					}else if(i>=10 && i<100){
						imgID = "00"+ std::to_string(i);
					}else if(i>=100 && i<1000){
						imgID = "0"+ std::to_string(i);
					}else if(i>=1000 && i<10000){
						imgID = std::to_string(i);
					}else{
						imgID = "xx";
					}


					std::string camID;
					if(j>=0 && j<10){
						camID = "0" + std::to_string(j);
					}else if(j>=10 && j<100){
						camID = std::to_string(j);
					}else{
						imgID = "xx";
					}

					std::string filename = imgID + "-" + camID + ".jpg";
					//std::cout << imgpass+filename << std::endl;

					cv::Mat imgbuff = cv::imread(imgpass+filename,1);

					ik[k].setName(filename);
					ik[k].setIMG(imgbuff);
				}
			}
		}
	}
}


/*
void Reader::setImg(int numofCam,int startImg, int endImg){

	clock_t start,end;

	std::ofstream ofs("./test.txt");

	int hoge = 0;
	ik.resize(numofCam*(endImg-startImg));

	for(int i=startImg; i < endImg; i++){

		for(int j = 0; j < numofCam; j++){

			std::string imgID;
			if(i>=0 && i<10){
				imgID = "000" + std::to_string(i);
			}else if(i>=10 && i<100){
				imgID = "00"+ std::to_string(i);
			}else if(i>=100 && i<1000){
				imgID = "0"+ std::to_string(i);
			}else if(i>=1000 && i<10000){
				imgID = std::to_string(i);
			}else{
				imgID = "xx";
			}


			std::string camID;
			if(j>=0 && j<10){
				camID = "0" + std::to_string(j);
			}else if(j>=10 && j<100){
				camID = std::to_string(j);
			}else{
				imgID = "xx";
			}

			std::string filename = imgID + "-" + camID + ".jpg";
			std::cout << imgpass+filename << std::endl;

			cv::Mat imgbuff = cv::imread(imgpass+filename,1);

			start = clock();
			if(imgbuff.rows > 0 && imgbuff.cols > 0){

				ik[hoge] = ImageKeeper(i,j,0,filename,imgbuff);
			}else{

				ik[hoge] = ImageKeeper(i, j, 0, "XX", cv::Mat::zeros(480, 480, CV_8UC3));
			}
			end = clock();
			//ofs << end-start << ",";

			hoge++;
		}
		//ofs << std::endl;
	}
	
}*/


int Reader::setFeaturePoint(int startline,int endline){		//nvmファイルを指定した領域だけ読み込み，特徴点のデータとして格納する．

	if(startline == endline){			//startline と endlineが等しい（指定された領域が存在しない）場合，関数を抜ける．
		return -1;
	}

	std::ifstream ifs(nvmpass.c_str());
	std::string buff;

	std::cout << "setting FeaturePoints";
		
	if(ifs.fail()){
		std::cout << "ERROR" << std::endl;
		return -1;
	}else{
		int lineno = 0;
		while(std::getline(ifs,buff)){		//最初の行から1行ずつ文字列を読み込む
			lineno++;			//読み込むごとに行数をカウントする
			if((lineno >= startline && lineno<=endline) && (lineno%FREQ == 0)){ //行数が引数で定義した領域内にあったら，特徴点データを読み込む
	
				if(lineno % 1000 ==0){
					std::cout << ".";
				}

				std::vector<std::string> FeatureData;
				int NumImage;
				std::vector<int> ImageIndex,CamIndex;
				std::vector<float> xPoint,yPoint;

				splitFeatureData(buff, FeatureData);		//特徴点データを取得
				
				separateFeatureData(FeatureData, &NumImage, ImageIndex, CamIndex, xPoint, yPoint);	//トラジェクトリのデータを取得

				for(int i=0; i<NumImage; i++){
					bool flag = true;

					for(int j=0; j<ik.size(); j++){
						if(ik[j].getID() == ImageIndex[i] && ik[j].getCamID() == CamIndex[i]){
							ik[j].setFeature(currentFeatureID,xPoint[i],yPoint[i]);
							flag = false;
							break;
						}
					}

					if(flag == true){
						ik.push_back(ImageKeeper(ImageIndex[i], CamIndex[i], 0, "XX", cv::Mat::zeros(480, 480, CV_8UC3)));
						ik[ik.size()-1].setFeature(currentFeatureID, xPoint[i], yPoint[i]);
					}
				}

				//FeaturePoint hoge(currentFeatureID);
				//featurePoints.push_back(hoge);

/*
				for(int i=0;i<NumImage;i++){
				
					ImageIndex[i] += prevID;


					for(int j=0;j<ik.size();j++){
						if(ik[j].getID() == ImageIndex[i] && ik[j].getCamID() == CamIndex[i]){
							ik[j].setFeature(currentFeatureID, xPoint[i], yPoint[i]);
						}
					}
					//ik[ImageIndex[i]].setFeature(currentFeatureID,xPoint[i],yPoint[i]);	//対象画像に特徴点のIDとx,y座標を格納する
				}*/
		
				currentFeatureID++;
			}
		}

		std::cout << std::endl;
	}
	return 0;
}

void Reader::sortKeyFrameID(){

	keyFrameID.resize(ik.size());

	for(int i=0; i<keyFrameID.size(); i++){
		keyFrameID[i] = ik[i].getID();
	}

	std::sort(keyFrameID.begin(), keyFrameID.end());

	keyFrameID.erase(std::unique(keyFrameID.begin(), keyFrameID.end()), keyFrameID.end());

/*
	std::vector<int> buff(ik.size());

	for(int i=0; i<ik.size();i++){
		buff[i] = ik[i].getID();
	}

	std::sort(buff.begin(), buff.end());

	keyFrameID.resize(std::unique(buff.begin(), buff.end())-buff.begin());
	std::cout << "mohimohi" << std::endl;

	for(int i=0; i<keyFrameID.size(); i++){
		std::cout << buff[i] << std::endl;
		keyFrameID[i] = buff[i];
	}*/
}

ImageKeeper Reader::getIKbyID(int cam,int in){
	for(int i=0;i<ik.size();i++){
		if(ik[i].getID()==in && ik[i].getCamID()==cam){
			return ik[i];
		}
	}

	return ImageKeeper(-1,-1,-1,"NoImage",cv::Mat::zeros(480, 480, CV_8UC3));
}


std::string Reader::getfileName(std::string in){

	//size_t pos1 = in.find("\\");
	size_t pos1 = 0;
	size_t pos2 = in.find(".")+4;

	std::string name = in.substr(pos1,pos2-pos1);

	return name;
}

void Reader::splitFeatureData(std::string in, std::vector<std::string> &FeatureData){	//特徴点のデータをstringで取得
											//FeatureData[0],[1],[2]はX,Y,Z [3]は画像の数
											//[4]はFrameID [5]はCameraID [6],[7]はx,y座標

	std::string delim = " ";
	size_t current = 0, found, delimlen = delim.size();

	while((found = in.find(delim, current)) != std::string::npos){
		FeatureData.push_back(std::string(in, current, found - current));
		current = found + delimlen;
	}
	FeatureData.push_back(std::string(in, current, in.size() - current));
}

void Reader::separateFeatureData(std::vector<std::string> &FeatureData, int *numimage, std::vector<int> &imageIndex, std::vector<int> &camID, std::vector<float> &xpoint, std::vector<float> &ypoint){

	*numimage = std::stoi(FeatureData[4]);
	for(int i=0; i < *numimage; i++){
		imageIndex.push_back(std::stoi(FeatureData[4 * i + 5]));
		camID.push_back(std::stoi(FeatureData[4 * i + 6]));
		xpoint.push_back(std::stoi(FeatureData[4 * i + 7]));
		ypoint.push_back(std::stoi(FeatureData[4 * i + 8]));
	}
}

#endif
