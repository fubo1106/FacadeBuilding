#pragma once
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

class Segmentation
{
public:
	Segmentation();
	Segmentation(cv::Mat& src);
	Segmentation(cv::Mat& src, cv::Mat& dst);
	~Segmentation();
	
	cv::Mat _src;
	cv::Mat _dst;

	double* _rgradient;
	double* _cgradient;

	void ImageSegmentation(Mat& src, Mat& res);
	int getBestSegAxis(char whichaxis, int userselected);
	void saveSegments(Mat& src, vector<pair<char, int>>& axis,string dir);

private:
	void calcAttribute(); //calc attributr based on _src
};

