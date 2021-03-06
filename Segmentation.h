#pragma once
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>

#include "Util/gcoUtil.h"
#include "QtAPI.h"
#include "Utility.h"

#include <fstream>
#include <iostream>
using namespace cv;
using namespace std;
using namespace Utility;
//extern QtAPI QtApi;

class Segmentation
{
public:
	Segmentation();
	Segmentation(cv::Mat& src,string path);
	Segmentation(cv::Mat& src, cv::Mat& dst);
	~Segmentation();
	
	cv::Mat _src;
	cv::Mat _dst;
	string _baseName;

	void ImageSegmentation(Mat& src, Mat& res);
	int getBestSegAxis(char whichaxis, int userselected);
	void saveSegments(Mat& src, vector<pair<char, int>>& axis,string dir);

	bool _segFolder = false;
	void kmeans_seg_folder(string srcDir, string dstDir);

	/*src: input image
	  result: CV_8UC1 same size to input, result.at<int>(i,j) record cluster label for pixel(i,j)
	  center: CV_32FC3 size=(ncluster,input.channel()), record cluster centers
	  visual: src.clone() for visualization
	*/
	void kmeans_seg(Mat& src, Mat& result, Mat& centers, Mat& visual, int nclusters);
	void save_kmeans(string dir);

	static const Scalar colorTab[5];

	gcoUtil* gcoSeg;
	bool init_gco = false;
	/*gco segmentation*/
	void gco_seg(double smooth_sym, double smooth_grid);
	void gco_segAll(string inputDir, string outDir, double smooth_sym, double smooth_grid);


//////////////////////////////////////////////////////////////////
	/*user labeling*/
	int depth=0;
	bool _withHint = false;
	void loadImages(string srcDir);
	void userLabel(QPoint tl, QPoint br);
	bool labelNext();
	bool labelPrevious();
	void saveLabels(string labelDir);
	vector<string> imagePath;
	vector<string> baseNames;
	int **_label_marked = NULL;
	int cursor = -1;//current image position

	typedef vector<Point> OneHint;
	typedef vector<vector<Point>> UserHint;
	//struct UserHint{
	//	vector<vector<Point>> hints;
	//
	//	void init(vector<vector<Point>>& points){
	//		hints = points;
	//	}
	//	void addHint(vector<Point>& hint){ 
	//		hints.push_back(hint); 
	//		_withHint = true; 
	//	}
	//	void removeHint(){
	//		if (hints.size() > 0)
	//			hints.pop_back();
	//		else
	//			_withHint = false;
	//	}
	//	UserHint(){}
	//	~UserHint(){}
	//};

	UserHint userhint;
	void addUserHint(OneHint& hint){
		userhint.push_back(hint);
		_withHint = true;
	}

	void refine_with_user();
	void refine_with_onehint(MatrixXi& init, OneHint& hint);
	void gco_seg_interactive(double smooth_sym, double smooth_grid,UserHint* userhint);
	void gco_seg_auto(double smooth_sym, double smooth_grid);
	void save_gco(string dir = "data\\result\\");
	//////////////////////////////////////////////////////
private:
	double* _rgradient = NULL;
	double* _cgradient = NULL;
	string _imagePath;

	//k-means variables
	Mat _kmeans_res;
	Mat _kmeans_centers;
	int _kmeans_ncluster;
	int _kmeans_nSamples;

	void calcAttribute(); //calc attributr based on _src
	void reinitialize();
};

