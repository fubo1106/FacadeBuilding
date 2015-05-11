#pragma once
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>

#include "Util\gcoUtil.h"

using namespace cv;
using namespace std;

class Segmentation
{
public:
	Segmentation(cv::Mat& src,string path);
	Segmentation(cv::Mat& src, cv::Mat& dst);
	~Segmentation();
	
	cv::Mat _src;
	cv::Mat _dst;

	void ImageSegmentation(Mat& src, Mat& res);
	int getBestSegAxis(char whichaxis, int userselected);
	void saveSegments(Mat& src, vector<pair<char, int>>& axis,string dir);

	void kmeans_seg_folder(string srcDir, string dstDir);

	/*src: input image
	  result: CV_8UC1 same size to input, result.at<int>(i,j) record cluster label for pixel(i,j)
	  center: CV_32FC3 size=(ncluster,input.channel()), record cluster centers
	  visual: src.clone() for visualization
	*/
	void kmeans_seg(Mat& src, Mat& result, Mat& centers, Mat& visual, int nclusters);
	void save_kmeans(string dir);

	gcoUtil* gcoSeg;
	bool init_gco = false;
	/*gco segmentation*/
	void gco_seg(double smooth_sym,double smooth_grid);

private:
	double* _rgradient = NULL;
	double* _cgradient = NULL;
	string _baseName;

	//k-means variables
	Mat _kmeans_res, _kmeans_centers;
	int _kmeans_ncluster;
	int _kmeans_nSamples;

	void calcAttribute(); //calc attributr based on _src
};

