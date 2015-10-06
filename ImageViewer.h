#ifndef IMGVIEWER
#define IMGVIEWER

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>

class ImageViewer{
private:
	int numCam;
public:
	ImagViewer(int numcam){
		numCam = numcam;
	}
}
#endif