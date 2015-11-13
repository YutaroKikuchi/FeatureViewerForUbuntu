#ifndef FEATURE
#define FEATURE

class Feature{

public:
	int FID;
	cv::Point2f point;

	void fixPoint(int rows,int cols);
};


void Feature::fixPoint(int rows,int cols){
	//point.x = point.x+cols/2;
	//point.y = rows/2-point.y;
}
#endif
