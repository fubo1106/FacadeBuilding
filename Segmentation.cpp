#include "Segmentation.h"
#include "Basics.h"

Segmentation::Segmentation()
{
}

Segmentation::Segmentation(cv::Mat& src, cv::Mat& dst)
{
	_src = src.clone();
	_dst = dst.clone(); 
}

Segmentation::Segmentation(cv::Mat& src)
{
	_src = src.clone();
	calcAttribute();
}

Segmentation::~Segmentation()
{
	delete _rgradient;
	delete _cgradient;
}

void Segmentation::calcAttribute(){
	Mat gradSrc;
	//Mat visual = Mat::zeros(_src.rows, _src.cols, CV_8UC1);

	if (_src.channels() == 3)
		cvtColor(_src, gradSrc, CV_BGR2GRAY);

	//each col gradient 
	_cgradient = new double[_src.cols]; //record sum gray for each colume

	for (int i = 0; i < _src.cols; i++){
		_cgradient[i] = 0;
	}

	for (int j = 0; j < _src.cols; j++){
		for (int i = 0; i < _src.rows; i++)
			_cgradient[j] += _src.at<uchar>(i, j);
		_cgradient[j] /= _src.rows;
		if (j > 0){
			_cgradient[j - 1] = abs(_cgradient[j] - _cgradient[j - 1]);
		}
	}

	//each row gradient 
	_rgradient = new double[_src.rows]; //record sum gray for each row

	for (int i = 0; i < _src.rows; i++){
		_rgradient[i] = 0;
	}

	for (int i = 0; i < _src.rows; i++){
		for (int j = 0; j < _src.cols; j++)
			_rgradient[i] += _src.at<uchar>(i, j);
		_rgradient[i] /= _src.cols;
		if (i > 0){
			_rgradient[i - 1] = abs(_rgradient[i] - _rgradient[i - 1]);
		}
	}

	/*Mat visual = Mat::zeros(_src.rows, _src.cols, CV_8UC1);
	for (int j = 0; j < _src.cols; j++){
		for (int i = 0; i < _src.rows; i++){
			visual.at<uchar>(i, j) = _cgradient[j];
		}
	}
	imshow("visual", visual);
	waitKey(0);*/

}

void Segmentation::ImageSegmentation(Mat& src, Mat& res){
	//Canny(src, res, 50, 100); 
	Mat gradSrc;
	Mat visual = Mat::zeros(src.rows, src.cols, CV_8UC1);

	cvtColor(src, gradSrc, CV_BGR2GRAY);
	double* avggray;
	avggray = new double[src.cols]; //record sum gray for each colume

	vector<double> scale;
	scale.resize(src.rows);//record gray scale for rows

	for (int i = 0; i < src.cols; i++){
		avggray[i] = 0;
	}


	for (int j = 0; j < src.cols; j++){
		for (int i = 0; i < src.rows; i++)
			avggray[j] += src.at<uchar>(i, j);
		avggray[j] /= src.rows;
		if (j >= 2){
			double del1 = abs(avggray[j] - avggray[j - 1]);
			double del2 = abs(avggray[j-1] - avggray[j - 2]);
			if (del1 >= 10*del2)
				avggray[j - 1] = -1;//record segment line
		}
	}
	
	for (int j = 0; j < visual.cols; j++){
		if (avggray[j] == -1)
			line(src, Point(j, 0), Point(j, visual.rows - 1), Scalar(0, 0, 255), 2, CV_AA);
	}

	for (int j = 0; j < visual.cols; j++){
		for (int i = 0; i < visual.rows; i++){
			visual.at<uchar>(i, j) = avggray[j];
		}
	}
	//imshow("visualize", visual);
	res = visual;
	//waitKey(0);
	delete avggray;
	return;
}

int Segmentation::getBestSegAxis(char whichaxis, int userselected){
	/*coordinate: ------------->  x axis
				  |
			y axis|
				  |
	*/
	int l, r;
	if (whichaxis == 'x'){
		/*horizen axis: -------->*/
		for (int i = userselected; i < _src.rows; i++){
			if (i >= 2){
				double del1 = abs(_rgradient[i] - _rgradient[i - 1]);
				double del2 = abs(_rgradient[i - 1] - _rgradient[i - 2]);
				if (del1 >= 10 * del2){
					r = i;//record segment line
					break;
				}
			}
		}
		for (int i = userselected; i > 0; i--){
			if (i >= 2){
				double del1 = abs(_rgradient[i] - _rgradient[i - 1]);
				double del2 = abs(_rgradient[i - 1] - _rgradient[i - 2]);
				if (del2 >= 10 * del1){
					l = i;//record segment line
					break;
				}
			}
		}
	}
	else{/*vertical axis --------*/
		for (int i = userselected; i < _src.cols; i++){
			if (i >= 2){
				double del1 = abs(_cgradient[i] - _cgradient[i - 1]);
				double del2 = abs(_cgradient[i - 1] - _cgradient[i - 2]);
				if (del1 >= 10 * del2){
					r = i;//record segment line
					break;
				}
					
			}
		}
		for (int i = userselected; i > 0; i--){
			if (i >= 2){
				double del1 = abs(_cgradient[i] - _cgradient[i - 1]);
				double del2 = abs(_cgradient[i - 1] - _cgradient[i - 2]);
				if (del2 >= 10 * del1){
					l = i;//record segment line
					break;
				}
			}
		}
	}

	/*if (abs((l + r) / 2 - userselected) < 50)
		return (l + r) / 2;
	else*/
		return userselected;
}

bool smallerThan(int p1, int p2){
	return p1 < p2;
}

void Segmentation::saveSegments(Mat& src, vector<pair<char, int>>& axis, string dir){
	/*coordinate: ------------->  x axis
			  |
			  |
	 		  |  y axis
	 x:  horizen axis: -------->
	 y:  vertical axis
	*/
	Mat imageroi;

	vector<int> ax, ay;
	for (int i = 0; i < axis.size(); i++){
		if (axis[i].first == 'x')
			ax.push_back(axis[i].second);
		else
			ay.push_back(axis[i].second);
	}
	sort(ax,smallerThan); 
	sort(ay,smallerThan);

	for (int i = 0; i < ax.size()-1; i++){
		for (int j = 0; j < ay.size()-1; j++){
			Rect rect(ay[j], ax[i], ay[j + 1] - ay[j], ax[i + 1] - ax[i]);
			imageroi = src(rect);
			//imshow("roi", imageroi); waitKey(0);
			imwrite(dir+"\\part-[" + boost::lexical_cast<string>(i)+"," + boost::lexical_cast<string>(j)+"].jpg", imageroi);
		}
	}

	return;
}