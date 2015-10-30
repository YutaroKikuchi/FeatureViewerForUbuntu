#ifndef LINEREADER
#define LINEREADER

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>

class LineReader{
private:
	std::string nvmpath;
	bool isStart(int size,int presize);
	bool isEnd(int size,int presize);

public:
	std::vector<int> startImg,endImg,startFeature,endFeature;

	LineReader(std::string in){
		nvmpath = in;
	}

	void setLineNo();
	int getNumOfModel();
	void showLines();
};

void LineReader::setLineNo(){		//画像データと特徴点データの箇所を検出，各箇所の行数を格納

	std::ifstream ifs(nvmpath.c_str());
	std::string buff;

	if(ifs.fail()){
		std::cout << "file is failed" << std::endl;
	}else{
		int currentline = 0,presize,size;
		int areano = 0;

		while(std::getline(ifs,buff)){

			currentline++;
			size = (int)buff.size();
			size--;

			if(buff=="0"){
				break;
			}else if((currentline == 1)||(currentline == 2)){

			}else if((isStart(size,presize)==true) && (isEnd(size,presize)==true)){

			}else if((isStart(size,presize)==true) && !(isEnd(size,presize)==true) && (areano==0)){	//現在の文字列の文字数と１行前の文字列の文字数を比較し，各箇所の行数を検出する
				std::cout << "startImg set" << std::endl;
				startImg.push_back(currentline+1);
			}else if(!(isStart(size,presize)==true) && (isEnd(size,presize)==true) && (areano==0)){	
				std::cout << "endImg set" << std::endl;
				endImg.push_back(currentline-1);
				areano = 1;
			}else if((isStart(size,presize)==true) && !(isEnd(size,presize)==true) && (areano==1)){
				std::cout << "startFeature set" << std::endl;
				startFeature.push_back(currentline+1);
			}else if(!(isStart(size,presize)==true) && (isEnd(size,presize)==true) && (areano==1)){
				std::cout << "endFeature set" << std::endl;
				endFeature.push_back(currentline-1);
				areano = 0;
			}
			presize = size;
		}

		for(int i=0;i<endFeature.size();i++){
			if(startImg[i]>endImg[i]){		//特徴点データが存在しない場合，endFeatureの値がstartFeatureの値を下回り，不具合の原因になるので調整する
				startImg[i] = endImg[i];
			}

			if(startFeature[i] > endFeature[i]){	//画像データが存在しない場合，endImgの値がstartImgの値を下回り，不具合の原因になるので調整する
				startFeature[i] = endFeature[i];
			}
		}
	}
}

int LineReader::getNumOfModel(){		//分断されたモデルの数を返す
	return endFeature.size();
}

void LineReader::showLines(){
	for(int i=0;i<endFeature.size();i++){
		//std::cout << "No." << i << std::endl << "startImg:" << startImg[i] << "  endImg" << endImg[i] << std::endl < "startFeature:" << startFeature[i] << "  endFeature:" << endFeature[i] << std::endl << std::endl;
	}
}


bool LineReader::isStart(int size,int presize){		//現在の行が，画像もしくは特徴点データの開始位置であるかどうかの判定
	if(size!=0 && presize==0){
	  std::cout << "hoge" << std::endl;
		return true;
	}else{
		return false;
	}
}
bool LineReader::isEnd(int size,int presize){		//現在の行が，画像もしくは特徴点データの終了位置であるかどうかの判定
	if(size==0 && presize!=0){
	  std::cout << "hoge" << std::endl;
		return true;
	}else{
		return false;
	}
}

#endif LINEREADER
