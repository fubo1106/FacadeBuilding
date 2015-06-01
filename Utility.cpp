#include "Utility.h"

QImage Utility::MatToQImage(const Mat& mat)
{
	// 8-bits unsigned, NO. OF CHANNELS=1
	if (mat.type() == CV_8UC1)
	{
		// Set the color table (used to translate colour indexes to qRgb values)
		QVector<QRgb> colorTable;
		for (int i = 0; i<256; i++)
			colorTable.push_back(qRgb(i, i, i));
		// Copy input Mat
		const uchar *qImageBuffer = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
		img.setColorTable(colorTable);
		return img;
	}
	// 8-bits unsigned, NO. OF CHANNELS=3
	if (mat.type() == CV_8UC3)
	{
		// Copy input Mat
		const uchar *qImageBuffer = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return img.rgbSwapped();
	}
	else
	{
		qDebug() << "ERROR: Mat could not be converted to QImage.";
		return QImage();
	}

}

QString Utility::getCommandLine(QString& plain){
	string str,cmdline;
	str = plain.toStdString();

	for (int i = str.length()-1; i >= 0; i--){

		if (str[i] == '\n'){
			cmdline = str.substr(i+1 , str.length());
			break;
		}
		if (i == 0){
			cmdline = str.substr(i, str.length());
			break;
		}

	}

	QString qstr = QString::fromStdString(cmdline);
	return qstr;

}

void Utility::testCommand(QString& command){
	string str;
	str = command.toStdString();
	qDebug("sss");
}

string Utility::toString(float d){
	char buffer[20];
	sprintf_s(buffer, "%f", d);
	string s = buffer;
	format(s);
	return s;
}

string Utility::toString(long d){
	char buffer[20];
	sprintf_s(buffer, "%d", d);
	string s = buffer;
	format(s);
	return s;
	
}

string Utility::toString(double d){
	char buffer[20];
	sprintf_s(buffer, "%f", d);
	string s = buffer;
	format(s);
	return s;
}

string Utility::toString(int d){
	char buffer[20];
	sprintf_s(buffer, "%d", d);
	string s = buffer;
	format(s);
	return s;
}

void Utility::format(string& s){
	if (s[1]=='.')
		s = s.substr(0, 4);
	return;
}

clock_t Utility::elapsed_time(){
	return clock() / CLOCKS_PER_SEC;
}

string Utility::getTime(){
	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "[%Y_%m_%d][%H_%M_%S]", localtime(&t));
	//puts(tmp);
	return tmp;
}

cv::Point Utility::tocvPoint(QPoint p){
	return cv::Point(p.x(), p.y());
}
vector<cv::Point> Utility::tocvPoints(vector<QPoint>& p){
	vector<cv::Point> points;
	for (int i = 0; i < p.size(); i++)
		points.push_back(tocvPoint(p[i]));
	return points;
}