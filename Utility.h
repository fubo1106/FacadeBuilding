#pragma once
#include <qimage.h>
#include <qdebug.h>

#include <time.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#ifndef printf
#define printf Utility::print
#endif

using namespace cv;
using namespace std;

#define DEPTH_0 30
#define DEPTH_1 100
#define DEPTH_2 200
#define DEPTH_3 230
#define DEPTH_4 170

namespace Utility
{
	QImage MatToQImage(const Mat& mat);
	
	QString getCommandLine(QString& plain);

	void testCommand(QString& command);

	string toString(float d);

	string toString(long d);

	string toString(double d);

	string toString(int d);

	void format(string& s);

	template<typename _Tp>
	string toString(_Tp d){
		return toString(d);
	}

	clock_t elapsed_time();

	string getTime();

	void print();
};

