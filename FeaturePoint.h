#ifndef FEATUREPOINT
#define FEATUREPOINT
#include <opencv2/core/core.hpp>
#include <iostream>


class FeaturePoint{
private:
	unsigned int ID;
	std::vector<int> imgID;
public:

	FeaturePoint(unsigned int id){
		ID = id;
	}
	/*
	~FeaturePoint(){
		ID = NULL;
		delete &ID;
	}*/

	unsigned int getID(){
		return ID;
	}

	void setimgID(int in){
		imgID.push_back(in);
	}

	int getimgID(int in){
		return imgID[in];
	}

	int getSizeImgID(){
		return imgID.size();
	}

	bool tellImage(int imgid){

		for(int i=0;i<imgID.size();i++){
			if(imgID[i] == imgid){
				return true;
			}
		}

		return false;
	}

	void showFeaturePoint(){

		std::cout << "ID:" << ID <<std::endl;

		std::vector<int>::iterator it = imgID.begin();

		for(;it!=imgID.end();++it){
			std::cout << " imgID:" << *it;
		}
		std::cout << std::endl;
	}

};


#endif
