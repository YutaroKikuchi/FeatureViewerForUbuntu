#ifndef IMAGEKEEPER
#define IMAGEKEEPER

#include<string>
#include "Feature.h"

class ImageKeeper{

private:
	cv::Mat img;
	int camID;
	int ID;
	int ModelID;
	std::string name;
	std::vector<Feature> features;
	int rows,cols;
public:

	ImageKeeper(){
	}

	ImageKeeper(int id,int camid,int modelid,std::string Name,cv::Mat image){

		ID = id;
		camID = camid;
		ModelID = modelid;
		name = Name;
		img = image;

		rows = image.rows;
		cols = image.cols;
	}

	
	void setName(std::string in);	// nameのsetter
	std::string getName();		// nameのgetter

	void setCamID(int in);		// camIDのsetter
	int getCamID();			// camIDのgetter

	void setID(int in);		// IDのsetter
	int setIDbyName();		// nameを用いたIDのsetter
	int getID();			// IDのgetter

	void setModelID(int modelid);	//ModelIDのsetter
	int getModelID();		//ModelIDのgetter

	void setIMG(cv::Mat in);	// imgのsetter
	cv::Mat getIMG();		// imgのgetter

	void setFeature(int id,float x, float y);	//Featureのsetter
	cv::Point2f getFeature(int in);			//Featureの座標のgetter
	int getFeatureID(int in);			//FeatureのIDのgetter
	cv::Point2f getFeaturebyID(int in);		//FeatureのIDを使った座標のgetter
	int getFeaturesSize();				//Featureのvectorのサイズを返す．

	int getCols();			//Colsのgetter
	int getRows();			//Rowsのgetter


	int getFeatureFlags(ImageKeeper ik,std::vector<bool> &flags);

	void showFeaturesSize(){
		std::cout  << "size:" << features.size() << std::endl;
	}
};

int ImageKeeper::setIDbyName(){
	size_t camIDpos1 = name.find("-");
	size_t camIDpos2 = name.rfind(".");
	size_t IDpos1 = 0;
	size_t IDpos2 = camIDpos1;

	camID = std::stoi(name.substr(camIDpos1+1,camIDpos2-camIDpos1-1));
	ID = std::stoi(name.substr(IDpos1,IDpos2-IDpos1));
	  
	return 0;
}

void ImageKeeper::setName(std::string in){
	name = in;
}

std::string ImageKeeper::getName(){
	return name;
}

void ImageKeeper::setCamID(int in){
	camID = in;
}

int ImageKeeper::getCamID(){
	return camID;
}

void ImageKeeper::setID(int in){
	ID = in;
}

int ImageKeeper::getID(){
	return ID;
}

void ImageKeeper::setModelID(int modelid){
	ModelID = modelid;
}

int ImageKeeper::getModelID(){
	return ModelID;
}

void ImageKeeper::setIMG(cv::Mat in){
	in.copyTo(img);

	rows = img.rows;
	cols = img.cols;
}

cv::Mat ImageKeeper::getIMG(){
	return img;
}

void ImageKeeper::setFeature(int id,float x, float y){
	Feature p;

	p.FID=id;
	p.point.x = x;
	p.point.y = y;

	p.fixPoint(rows,cols);
	features.push_back(p);
}

int ImageKeeper::getFeaturesSize(){
	return (int)features.size();
}

cv::Point2f ImageKeeper::getFeature(int in){
	return features[in].point;
}

int ImageKeeper::getFeatureID(int in){
	return features[in].FID;
}

int ImageKeeper::getCols(){
	return cols;
}

int ImageKeeper::getRows(){
	return rows;
}

cv::Point2f ImageKeeper::getFeaturebyID(int in){
	for(int i=0;i<features.size();i++){
		if(features[i].FID == in){
			return features[i].point;
		}
	}

	return cv::Point2f(0.0,0.0);
}

int ImageKeeper::getFeatureFlags(ImageKeeper ik,std::vector<bool> &flags){
	for(int j=0;j<ik.getFeaturesSize();j++){
		if(getFeaturebyID(ik.getFeatureID(j)) == cv::Point2f(0.0,0.0)){
			flags[j] = true;
		}else{
			if(flags[j] != true)
				flags[j] = false;
		}
	}
	return 0;
}
#endif
