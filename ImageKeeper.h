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
	int symID;
	std::string name;
	std::vector<Feature> points;
	int rows,cols;
public:

	ImageKeeper(){
	}

	ImageKeeper(int id,int camid,std::string Name,cv::Mat image){

		ID = id;
		camID = camid;
		name = Name;
		img = image;
	}
	void setName(std::string in){
		name = in;
	}

	std::string getName(){
		return name;
	}

	int setIDbyName(){

       

	  size_t camIDpos1 = name.find("!");
	  size_t camIDpos2 = name.rfind("!");

	  size_t IDpos1 = camIDpos2;
	  size_t IDpos2 = name.find(".");

	  // std::cout << "camID:" << name.substr(camIDpos1+1,camIDpos2-camIDpos1-1) << std::endl;
	  //std::cout << "ID:" << name.substr(IDpos1+1,IDpos2-IDpos1-1) << std::endl;

	  camID = std::stoi(name.substr(camIDpos1+1,camIDpos2-camIDpos1-1));
	  ID = std::stoi(name.substr(IDpos1+1,IDpos2-IDpos1-1));
	  
	  return 0;

	}

	void setCamID(int in){
	  camID = in;
	}

	int getCamID(){
	  return camID;
	}

	void setID(int in){
		ID = in;
	}

	int getID(){
		return ID;
	}

	void setSymID(int in){
		symID = in;
	}

	int getSymID(){
		return symID;
	}

	void setIMG(cv::Mat in){
		in.copyTo(img);

		rows = img.rows;
		cols = img.cols;
	}

	cv::Mat getIMG(){
		return img;
	}

	void setFeature(int id,float x, float y){
		Feature p;

		p.FID=id;
		p.point.x = x;
		p.point.y = y;

		p.fixPoint(rows,cols);

		points.push_back(p);
	}

	int getPointsSize(){
		return (int)points.size();
	}

	cv::Point2f getPoint(int in){
		return points[in].point;
	}

	int getFeatureID(int in){
		return points[in].FID;
	}

	cv::Point2f getPointID(int in){
		for(int i=0;i<points.size();i++){
			if(points[i].FID == in){
				return points[i].point;
			}
		}

		return cv::Point2f(0.0,0.0);
	}

	bool isHaveFeature(int in){
		for(int i=0;i<points.size();i++){
			if(points[i].FID == in){
				
				//std::cout << "points[i].FID:" << points[i].FID << " in:" << in << std::endl;
				return true;
			}
		}
		return false;
	}

	int getFeatureFlags(ImageKeeper ik,std::vector<bool> &flags){
		for(int i=0;i<points.size();i++){
			for(int j=0;j<ik.getPointsSize();j++){
				if(isHaveFeature(ik.getFeatureID(j)) == true){
					flags[j] = true;
				}else{
					flags[j] = false;
				}
			}
		}

		return 1;
	}

	void showPointsSize(){
		std::cout  << "size:" << points.size() << std::endl;
	}

	cv::Mat showAllData(){
		std::cout << "No:" << ID << std::endl << "Name:" << name << std::endl<<std::endl;
		//showFeatures();

		return img;
	}

	void showFeatures(){
		std::vector<Feature>::iterator it = points.begin();

		for(int i=0;i<points.size();i++){
			std::cout << "FID;" << points[i].FID << " x:" << points[i].point.x << " y:" << points[i].point.y << std::endl; 
		}
	}

	void showFeatureIndex(){
		for(int i=0;i<points.size();i++){
			std::cout << "FeatureIndex:" << points[i].FeatureIndex << std::endl;
		}
	}

	void setFeatureIndex(int i,int input){
		points[i].FeatureIndex = input;
	}

	int getCols(){
		return cols;
	}

	int getRows(){
		return rows;
	}
};



#endif
