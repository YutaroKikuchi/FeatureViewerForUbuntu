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
	cv::vector<int> startImg,endImg,startFeature,endFeature;

	LineReader(std::string in){
		nvmpath = in;
	}


	void setLineNo(){
		std::ifstream ifs(nvmpath);
		std::string buff;

		if(ifs.fail()){
		}else{
			int currentline = 0,presize,size;
			int areano = 0;

			while(std::getline(ifs,buff)){
				currentline++;
				size = (int)buff.size();

				if(buff=="0"){
					break;
				}else if((currentline == 1)||(currentline == 2)){
				}else if((isStart(size,presize)==true) && (isEnd(size,presize)==true)){
				}else if((isStart(size,presize)==true) && !(isEnd(size,presize)==true) && (areano==0)){
					startImg.push_back(currentline+1);
				}else if(!(isStart(size,presize)==true) && (isEnd(size,presize)==true) && (areano==0)){
					endImg.push_back(currentline-1);
					areano = 1;
				}else if((isStart(size,presize)==true) && !(isEnd(size,presize)==true) && (areano==1)){
					startFeature.push_back(currentline+1);
				}else if(!(isStart(size,presize)==true) && (isEnd(size,presize)==true) && (areano==1)){
					endFeature.push_back(currentline-1);
					areano = 0;
				}
				presize = size;
			}
		}
	}

	int getNumOfModel(){
		return endFeature.size();
	}

	void showStr(int line){
		std::ifstream ifs(nvmpath);
		std::string buff;
		int lineno=0;

		while(std::getline(ifs,buff)){
			lineno++;
			if(lineno==line)
				std::cout << buff << std::endl;
		}
	}

	void showLines(){
		for(int i=0;i<startImg.size();i++){
			std::cout << "No." << i << std::endl << "startImg:" << startImg[i] << "  endImg" << endImg[i] << std::endl << "startFeature:" << startFeature[i] << "  endFeature" << endFeature[i] << std::endl << std::endl;
		}
	}


};

bool LineReader::isStart(int size,int presize){
	if(size!=0 && presize==0){
		return true;
	}else{
		return false;
	}
}
bool LineReader::isEnd(int size,int presize){
	if(size==0 && presize!=0){
		return true;
	}else{
		return false;
	}
}

#endif LINEREADER