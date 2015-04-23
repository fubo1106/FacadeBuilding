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