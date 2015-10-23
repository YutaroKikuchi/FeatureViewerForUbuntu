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

	void setLineNo(){

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
				}else if((isStart(size,presize)==true) && !(isEnd(size,presize)==true) && (areano==0)){
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
				if(startImg[i]>endImg[i])
					startImg[i] = endImg[i];
				if(startFeature[i] > endFeature[i])
					startFeature[i] = endFeature[i];
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
		for(int i=0;i<endFeature.size();i++){
			std::cout << "No." << i << std::endl << "startImg:" << startImg[i] << "  endImg" << endImg[i] << std::endl << "startFeature:" << startFeature[i] << "  endFeature" << endFeature[i] << std::endl << std::endl;
		}
	}


};

bool LineReader::isStart(int size,int presize){
	if(size!=0 && presize==0){
	  std::cout << "hoge" << std::endl;
		return true;
	}else{
		return false;
	}
}
bool LineReader::isEnd(int size,int presize){
	if(size==0 && presize!=0){
	  std::cout << "hoge" << std::endl;
		return true;
	}else{
		return false;
	}
}

#endif LINEREADER
