#ifndef READER
#define READER

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
	int currentLine;
	std::string imgpass;
	std::string nvmpass;
	std::string getfileName(std::string in);
	bool isStart(int size,int presize);
	bool isEnd(int size,int presize);
	void getPoints();
	void getnumofImage(std::string in, std::string delim);
	void getFeatureData(std::string in,std::string delim, std::vector<std::string> &FeatureData);
	std::vector<std::string> FeatureData;
	int getImgNum(std::string in);
public:
	std::vector<ImageKeeper> ik;
	std::vector<FeaturePoint> featurePoints;
	int startImg,endImg,startFeature,endFeature;
	std::vector<int> imgLink;

	std::vector<int> featureindex;

	Reader(){}

	Reader(int cam,std::string img,std::string nvm){
		imgpass = img;
		nvmpass = nvm;
		currentID =0;
		currentLine = 0;
		currentFeatureID = 0;
		prevID = 0;
	}

	void setImg(int startline, int endline){	//引数で指定したnvmファイルの部分を読み込む
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
					ik.push_back(imbuff);
					ik[currentID].setIMG(cv::imread(imgpass+imbuff.getName(),1));
					imgLink.push_back(imbuff.getID());	//imgLinkにIDを登録

					currentID++;
		      		}
				
			}
		}

		std::sort(imgLink.begin(),imgLink.end());
	}

	int setFeaturePoint(int startline,int endline){		//nvmファイルを指定した領域だけ読み込み，特徴点のデータとして格納する．

		if(startline == endline){			//startline と endlineが等しい（指定された領域が存在しない）場合，関数を抜ける．
			return -1;
		}

		std::ifstream ifs(nvmpass.c_str());
		std::string buff;

		std::cout << "setting FeaturePoints";
		
		if(ifs.fail()){
			std::cout << "ERROR" << std::endl;
		}else{
			int lineno = 0;
			while(std::getline(ifs,buff)){		//最初の行から1行ずつ文字列を読み込む
				lineno++;			//読み込むごとに行数をカウントする
				if((lineno >= startline && lineno<=endline) && (lineno%FREQ == 0)){ //行数が引数で定義した領域内にあったら，特徴点データを読み込む
		
					if(lineno % 1000 ==0){
						std::cout << ".";
					}

					std::vector<std::string> FeatureData;

					getFeatureData(buff, " ", FeatureData);			//特徴点データを取得
					int NumImage = std::stoi(FeatureData[6]);	//int型として格納
					//FeaturePoint hoge(currentFeatureID);
					//featurePoints.push_back(hoge);

					for(int i=0;i<NumImage;i++){
						int ImageIndex;
		  
						if(prevID != 0){
							ImageIndex = std::stoi(FeatureData[i*4+7]) + prevID;
						}else{
							ImageIndex = std::stoi(FeatureData[i*4+7]);
						}

						//int FeatureIndex = std::stoi(FeatureData[i*4+7+1]);
						int FeatureID = currentFeatureID;	//特徴点にIDを割り振る
						float xPoint = std::stof(FeatureData[i*4+7+2]);	//対象画像上のx座標を取得
						float yPoint = std::stof(FeatureData[i*4+7+3]);	//対象画像上のy座標を取得
		  
						if(ImageIndex == 0){
							//featureindex.push_back(FeatureIndex);
						}

						//featurePoints[currentFeatureID].setimgID(ImageIndex);
		  
						ik[ImageIndex].setFeature(FeatureID,xPoint,yPoint);	//対象画像に特徴点のIDとx,y座標を格納する
					}
		
					currentFeatureID++;
				}
			}

			std::cout << std::endl;
		}
	}

	ImageKeeper getIKbyID(int cam,int in){
	  for(int i=0;i<ik.size();i++){
	    if(ik[i].getID()==in && ik[i].getCamID()==cam){
	      return ik[i];
	    }
	  }

	  ImageKeeper ret;
	  ret.setID(-1);
	  return ret;

	}

	void showFeatureData(std::string in){

		getnumofImage(in," ");

		std::vector<std::string>::iterator it = FeatureData.begin();

		for(;it!=FeatureData.end();++it){
			std::cout << *it << std::endl;
		}
	}

};

std::string Reader::getfileName(std::string in){

	//size_t pos1 = in.find("\\");
	size_t pos1 = 0;
	size_t pos2 = in.find(".")+4;

	std::string name = in.substr(pos1,pos2-pos1);

	return name;
}

bool Reader::isStart(int size,int presize){
	if(size!=0 && presize==0){
		return true;
	}else{
		return false;
	}
}
bool Reader::isEnd(int size,int presize){
	if(size==0 && presize!=0){
		return true;
	}else{
		return false;
	}
}

void Reader::getFeatureData(std::string in,std::string delim, std::vector<std::string> &FeatureData){

	size_t current = 0, found, delimlen = delim.size();

	while((found = in.find(delim, current)) != std::string::npos){
		FeatureData.push_back(std::string(in, current, found - current));
		current = found + delimlen;
	}
	FeatureData.push_back(std::string(in, current, in.size() - current));
}

int Reader::getImgNum(std::string in){

	size_t pos1 = in.find("!");
	size_t pos2 = in.find(".");

	std::string num = in.substr(pos1+1,pos2-pos1-1);

	return std::stoi(num);
}


#endif
