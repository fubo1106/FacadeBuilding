#pragma once
#include <qimage.h>
#include <qdebug.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace cv;

namespace Utility
{
	QImage MatToQImage(const Mat& mat);
	
	QString getCommandLine(QString& plain);

	void testCommand(QString& command);

};

