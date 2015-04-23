#ifndef FACBUILDING_H
#define FACBUILDING_H

#define WINDOW_W 1000
#define WINDOW_H 800

#include <QtWidgets/QMainWindow>
#include "ui_facbuilding.h"
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <qtextedit.h>

#include "CommandTextEdit.h"
#include "Utility.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

class FacBuilding : public QMainWindow
{
	Q_OBJECT

public:
	FacBuilding(QWidget *parent = 0);
	~FacBuilding();

	void myLayout(); //setup my own layout

	//UI design
	QWidget* window;
	QGridLayout* gLayout; //8*9 layout in this UI
	QLabel* testLabel;
	QLabel* resultLabel;
	QPushButton* startButton;
	QPushButton* button1;
	QPushButton* button2;
	QPushButton* button3;
	QPushButton* button4;
	QPushButton* button5;
	QPushButton* button6;
	QPushButton* button7;

	CommandTextEdit* command;
	QPushButton* cmdButton1;
	QPushButton* cmdButton2; 

	Mat src;
	Mat dst;

private:
	Ui::FacBuildingClass ui;

private slots:
	void on_startButton_clicked();
	void on_cmdButton1_clicked();
	void on_cmdButton2_clicked();

	void on_CommandEdit_Entered();
};

#endif // FACBUILDING_H
