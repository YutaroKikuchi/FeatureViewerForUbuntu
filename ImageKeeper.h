#ifndef IMAGEKEEPER
#define IMAGEKEEPER

#include<string>

class Feature{

public:
	int FID;
	int FeatureIndex;
	cv::Point2f point;

	void fixPoint(int rows,int cols){
		point.x = point.x+cols/2;
		point.y = rows/2-point.y;
	}
};

class ImageKeeper{

private:
	cv::Mat img;
	int camID;
	int ID;
	std::string name;
	std::vector<Feature> features;
	int rows,cols;
public:

	ImageKeeper(){
	}

	ImageKeeper(int id,int camid,std::string Name,cv::Mat image){

		ID = id;
		camID = camid;
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

	void setIMG(cv::Mat in);	// imgのsetter
	cv::Mat getIMG();		// imgのgetter

	void setFeature(int id,float x, float y);	//Featureのsetter
	cv::Point2f getFeature(int in);			//Featureの座標のgetter
	int getFeatureID(int in);			//FeatureのIDのgetter
	cv::Point2f getFeaturebyID(int in);		//FeatureのIDを使った座標のgetter
	int getFeaturesSize();				//Featureのvectorのサイズを返す．

/*
	bool isHaveFeature(int in){
		for(int i=0;i<features.size();i++){
			if(features[i].FID == in){
				
				//std::cout << "points[i].FID:" << points[i].FID << " in:" << in << std::endl;
				return true;
			}
		}
		return false;
	}
*/

	int getFeatureFlags(ImageKeeper ik,std::vector<bool> &flags){
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

	void showFeaturesSize(){
		std::cout  << "size:" << features.size() << std::endl;
	}

	int getCols(){
		return cols;
	}

	int getRows(){
		return rows;
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

cv::Point2f ImageKeeper::getFeaturebyID(int in){
	for(int i=0;i<features.size();i++){
		if(features[i].FID == in){
			return features[i].point;
		}
	}

	return cv::Point2f(0.0,0.0);
}


#endif
