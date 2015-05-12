#ifndef FACBUILDING_H
#define FACBUILDING_H

#define WINDOW_W 1000
#define WINDOW_H 800

#include <QtWidgets/QMainWindow>
#include "ui_facbuilding.h"
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <qtextedit.h>
#include <qslider.h>
#include <qaction.h>
#include <qdebug.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ImageLabel.h"
#include "CommandTextEdit.h"
#include "Utility.h"
#include "Segmentation.h"

//previous graph method
#include "../3rdParty/graph/FacBuilder.h"
#include "../3rdParty/graph/Basic_File.h"
#include "../3rdParty/graph/Basics.h"
#include "../3rdParty/graph/GraphMatching.h"
#include "../3rdParty/graph/DP.h"

using namespace cv;
using namespace std;

class FacBuilding : public QMainWindow
{
	Q_OBJECT

signals:
	void sendCommand(QString cmd);

public:
	FacBuilding(QWidget *parent = 0);
	~FacBuilding();

	void myLayout(); //setup my own layout
	void keyPressEvent(QKeyEvent* e);
	//UI design
	QWidget* window;
	QGridLayout* gLayout; //10*20 layout in this UI
	ImageLabel* testLabel;
	ImageLabel* tempLabel; 
	ImageLabel* resultLabel;
	QPushButton* startButton;

	QSlider* slider1;
	QSlider* slider2;

	QPushButton* selectFolder;
	QPushButton* button4;
	QPushButton* button5;
	QPushButton* button6;
	QPushButton* button7;


	QLabel *symAttri = new QLabel("sym:");//=string"sym:"
	QLabel *edgeAttri = new QLabel("edge:");
	QLabel *symvalue, *gridvalue;

	CommandTextEdit* command;
	QPushButton* cmdButton1;
	QPushButton* cmdButton2; 

	Mat src,src_temp;
	Mat dst,dst_temp;
	Mat tmp;

	Segmentation* seg = NULL;

private:
	Ui::FacBuildingClass ui;
	QAction* openAction;
	Size labelsize;
	vector<pair<char, int>> _axis; //the segment axis

	QLabel *_srcLabel = new QLabel("src"), 
		   *_tmpLabel = new QLabel("kmeans"), 
		   *_dstLabel = new QLabel("gco segmentation");

	void repaint();
	void showSegment(Mat& src, vector<pair<char, int>>& axis);

private slots:
	void on_startButton_clicked();
	void on_cmdButton1_clicked();
	void on_cmdButton2_clicked();
	void on_selectFolderButton_clicked();

	void on_CommandEdit_Entered();

	void on_ImageLabel_rolling(QPoint angle);
	void on_this_sendCmd(QString cmd);

	void on_ImageLabel_mouseClick(QPoint pos);
	void on_ImageLabel_mouseDrag(QPoint start, QPoint end);

	void on_sym_valuechanged(int v);
	void on_grid_valuechanged(int v);
	void on_sym_released();
};

#endif // FACBUILDING_H
