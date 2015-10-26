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

	void setImg(int startline, int endline){
	  std::ifstream ifs(nvmpass.c_str());
	  std::string buff;
	  ImageKeeper imbuff;
	  
	  prevID = currentID;

	  if(ifs.fail()){
	    std::cout << "ERROR" << std::endl;
	  }else{
	    int lineno = 0;
	    //cv::namedWindow("video",CV_WINDOW_AUTOSIZE);
	    while(std::getline(ifs,buff)){
	      lineno++;
	      if(lineno >= startline && lineno<=endline){
		std::string name = getfileName(buff);
		
		//std::cout << currentID << std::endl;
		
		//if(currentID % 1 == 0)
		  //std::cout << name << std::endl;

		imbuff.setName(name);
		imbuff.setIDbyName();
		ik.push_back(imbuff);
		ik[currentID].setIMG(cv::imread(imgpass+imbuff.getName(),1));
		imgLink.push_back(imbuff.getID());

		currentID++;
	      }
				
	    }
		
	  }

	  std::sort(imgLink.begin(),imgLink.end());

		/*
		if(ifs.fail()){
			std::cout << "ERROR" << std::endl;
		}else{
			int lineno = 0;
			int ID = 0;
			while(std::getline(ifs,buff)){
				lineno++;

				if(lineno >= startline && lineno<=endline){
					std::string name = getfileName(buff);
					int num = getImgNum(name);

					ik[ID].setID(num);
					ik[ID].setName(name);
					ik[ID].setIMG(cv::imread(imgpass+name,1));

					ID++;
				}
				
			}
		}*/

	}

	void setLineNo(){
	  std::ifstream ifs(nvmpass.c_str());
		std::string buff;

		if(ifs.fail()){
		}else{
			int currentline = 0,presize,size;
			int areano = 0;

			while(std::getline(ifs,buff)){
				currentline++;
				size = (int)buff.size();

				if((currentline == 1)||(currentline == 2)){
				}else if((isStart(size,presize)==true) && (isEnd(size,presize)==true)){
				}else if((isStart(size,presize)==true) && !(isEnd(size,presize)==true) && (areano==0)){
					startImg = currentline+1;
				}else if(!(isStart(size,presize)==true) && (isEnd(size,presize)==true) && (areano==0)){
					endImg = currentline-1;
					areano++;
				}else if((isStart(size,presize)==true) && !(isEnd(size,presize)==true) && (areano==1)){
					startFeature = currentline+1;
				}else if(!(isStart(size,presize)==true) && (isEnd(size,presize)==true) && (areano==1)){
					endFeature = currentline-1;
					areano++;
				}
				presize = size;
			}
		}
	}

	int setFeaturePoint(int startline,int endline){

		//featurePoints.reserve(endline-startline+1);

		if(endline - startline == 0)
			return 0;

	  std::ifstream ifs(nvmpass.c_str());
	  std::string buff;

	  std::cout << "setting FeaturePoints";

	  ImageKeeper imbuff;
	  if(ifs.fail()){
	    std::cout << "ERROR" << std::endl;
	  }else{
	    int lineno = 0;
	    while(std::getline(ifs,buff)){
	      lineno++;
	      if((lineno >= startline && lineno<=endline) && (lineno%FREQ == 0)){
		
		if(lineno % 1000 ==0)
		  std::cout << ".";
		//std::cout<< buff << std::endl;

		getnumofImage(buff," ");
		int NumImage = std::stoi(FeatureData[6]);
		FeaturePoint hoge(currentFeatureID);
		featurePoints.push_back(hoge);
		//std::cout << "*******************************" << std::endl <<"ID:"<< featureID << std::endl;
		//std::cout << "NumOfImage:"<<NumImage<<std::endl;
		for(int i=0;i<NumImage;i++){
		  int ImageIndex;
		  
		  if(prevID != 0){
		    ImageIndex = std::stoi(FeatureData[i*4+7]) + prevID;
		  }else{
		    ImageIndex = std::stoi(FeatureData[i*4+7]);
		  }
		  
		  int FeatureIndex = std::stoi(FeatureData[i*4+7+1]);
		  int FeatureID = currentFeatureID;
		  float xPoint = std::stof(FeatureData[i*4+7+2]);
		  float yPoint = std::stof(FeatureData[i*4+7+3]);
		  
		  if(ImageIndex == 0){
		    featureindex.push_back(FeatureIndex);
		  }

		  featurePoints[currentFeatureID].setimgID(ImageIndex);
		  
		  ik[ImageIndex].setFeature(FeatureID,xPoint,yPoint);
		}
		
		currentFeatureID++;
	      }
	      
	      FeatureData.clear();
	      
	    }
	    
	    std::cout << std::endl;
	  }
	}

	ImageKeeper getIKbyID(int in){
		for(int i=0;i<ik.size();i++){
			if(ik[i].getID() == in){
				return ik[i];
			}
		}

		ImageKeeper ret;
		ret.setID(-1);
		return ret;
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

	int getIKIndexbyID(int in){
		for(int i=0;i<ik.size();i++){
			if(ik[i].getID() == in){
				return i;
			}
		}

		return -1;
	}

	void showFeatureData(std::string in){

		getnumofImage(in," ");

		std::vector<std::string>::iterator it = FeatureData.begin();

		for(;it!=FeatureData.end();++it){
			std::cout << *it << std::endl;
		}
	}

	void testFeatureData(){

		for(int i=0;i<10;i++){
			std::cout << "ID:"<<featurePoints[i*100].getID() <<" imgID:" << featurePoints[i*100].getimgID(0)<<std::endl;
		}
	}

	void showImgIndex(int startline,int endline){

		featurePoints.reserve(endline-startline+1);

		std::ifstream ifs(nvmpass);
		std::string buff;

		std::ofstream ofs("./imageindex.txt");

		std::cout << "setting FeaturePoints";

		ImageKeeper imbuff;
		if(ifs.fail()){
			std::cout << "ERROR" << std::endl;
		}else{
			int lineno = 0;
			int featureID = 0;
			while(std::getline(ifs,buff)){
				lineno++;
				if((lineno >= startline && lineno<=endline) && (lineno%FREQ == 0)){

					if(lineno % 1000 ==0)
						std::cout << ".";

					getnumofImage(buff," ");
					int NumImage = std::stoi(FeatureData[6]);
					FeaturePoint hoge(featureID);
					featurePoints.push_back(hoge);
					for(int i=0;i<NumImage;i++){
						int ImageIndex = std::stoi(FeatureData[i*4+7]);
						int FeatureIndex = std::stoi(FeatureData[i*4+7+1]);
						float xPoint = std::stof(FeatureData[i*4+7+2]);
						float yPoint = std::stof(FeatureData[i*4+7+3]);

						if(FeatureIndex == 1){
							ofs << "image[" << ImageIndex << "]:{" << FeatureIndex << "}in " << lineno << std::endl;
						}
					}

					featureID++;
				}

				FeatureData.clear();
				
			}

			std::cout << std::endl;
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

void Reader::getnumofImage(std::string in,std::string delim){

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
