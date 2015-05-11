#include "facbuilding.h"
#include <qstring.h>
#include <qfiledialog.h>
#include <qmovie.h>
#include <qpixmap.h>
#include <qmenu.h>

#include <sstream>

FacBuilding::FacBuilding(QWidget *parent)
	: QMainWindow(parent)
{
	//ui.setupUi(this);
	window = new QWidget;
	gLayout = new QGridLayout;

	testLabel = new ImageLabel();
	resultLabel = new ImageLabel();
	tempLabel = new ImageLabel();

	startButton = new QPushButton("start");
	startButton->setShortcut(tr("ctrl+o"));

	slider1 = new QSlider(Qt::Horizontal);
	slider2 = new QSlider(Qt::Horizontal);

	button3 = new QPushButton("start3");
	button4 = new QPushButton("start4");
	button5 = new QPushButton("start5");
	button6 = new QPushButton("start6");
	button7 = new QPushButton("start7");

	cmdButton1 = new QPushButton("excute1");
	cmdButton2 = new QPushButton("clear");
	
	button3->setEnabled(false);
	button4->setEnabled(false);
	button5->setEnabled(false);
	button6->setEnabled(false);
	button7->setEnabled(false);

	command = new CommandTextEdit;
//	command->setFocusPolicy(Qt::StrongFocus);
	command->setPlainText("start...\n");

	symvalue = new QLabel(tr("11"));
	gridvalue = new QLabel(tr("11"));

	connect(startButton, SIGNAL(clicked()), this, SLOT(on_startButton_clicked()));
	connect(cmdButton1, SIGNAL(clicked()), this, SLOT(on_cmdButton1_clicked()));
	connect(cmdButton2, SIGNAL(clicked()), this, SLOT(on_cmdButton2_clicked()));
	connect(command, SIGNAL(pressEnter()), this, SLOT(on_CommandEdit_Entered()));
	//connect(command, SIGNAL(pressEnter()), command, SLOT(executeCommand()));
	connect(testLabel, SIGNAL(wheelrolling(QPoint)), this, SLOT(on_ImageLabel_rolling(QPoint)));
	connect(this, SIGNAL(sendCommand(QString)), this, SLOT(on_this_sendCmd(QString)));

	connect(slider1, SIGNAL(valueChanged(int)), this, SLOT(on_sym_valuechanged(int)));
	connect(slider2, SIGNAL(valueChanged(int)), this, SLOT(on_grid_valuechanged(int)));

	ui.setupUi(this);
}

FacBuilding::~FacBuilding()
{
	if (seg != NULL)
		delete seg;
}

void FacBuilding::myLayout(){
	
	window->setWindowTitle("FacBuilding");
	//window->setWindowState(Qt::WindowMaximized);
	window->setGeometry(150, 100, WINDOW_W, WINDOW_H);

	testLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	testLabel->setFrameShape(QFrame::Box);
	testLabel->setAutoFillBackground(true);
	testLabel->setScaledContents(true);
	testLabel->setFixedWidth(WINDOW_W / 3);
	testLabel->setText("testImage");
	testLabel->setFocusPolicy(Qt::ClickFocus);

	tempLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	tempLabel->setFrameShape(QFrame::Box);
	tempLabel->setAutoFillBackground(true);
	tempLabel->setScaledContents(true);
	tempLabel->setFixedWidth(WINDOW_W / 3);
	tempLabel->setText("tmpImage");
	tempLabel->setFocusPolicy(Qt::ClickFocus);

	resultLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	resultLabel->setFrameShape(QFrame::Box);
	resultLabel->setScaledContents(true);
	resultLabel->setFixedWidth(WINDOW_W / 3);
	resultLabel->setText("resultImage");
	resultLabel->setFocusPolicy(Qt::ClickFocus);

	_srcLabel->setFixedHeight(WINDOW_H / 50);
	_tmpLabel->setFixedHeight(WINDOW_H / 50);
	_dstLabel->setFixedHeight(WINDOW_H / 50);

	startButton->setText("start");
	
	slider1->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
	slider1->setMinimum(0);
	slider1->setMaximum(100);
	slider1->setTickInterval(1);
	slider1->setInvertedAppearance(true);
	slider1->setValue(5);
	slider1->setEnabled(false);

	slider2->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
	slider2->setMinimum(0);
	slider2->setMaximum(100);
	slider2->setTickInterval(1);
	slider2->setInvertedAppearance(true);
	slider2->setValue(10);
	slider2->setEnabled(false);

	button3->setText("start");
	button4->setText("start4");
	button5->setText("start5");
	button6->setText("start6");
	button7->setText("start7");

	command->setAutoFillBackground(true);
	command->setMaximumHeight(WINDOW_H / 3);
	//command->setMinimumHeight(WINDOW_H / 5);

	//gLayout->addWidget(toolBar, 0, 0, 1, 1);
	gLayout->addWidget(testLabel, 0, 0, 5, 6);
	gLayout->addWidget(tempLabel, 0, 6, 5, 6);
	gLayout->addWidget(resultLabel, 0, 13, 5, 6);

	//labels
	gLayout->addWidget(_srcLabel, 5, 2, 1, 1);
	gLayout->addWidget(_tmpLabel, 5, 8, 1, 1);
	gLayout->addWidget(_dstLabel, 5, 15, 1, 1);

	gLayout->addWidget(startButton, 6, 0, 1, 1);
	gLayout->addWidget(symAttri, 6, 1, 1, 1);
	gLayout->addWidget(slider1, 6, 2, 1, 1);
	gLayout->addWidget(symvalue, 6, 3, 1, 1);
	gLayout->addWidget(edgeAttri, 6, 4, 1, 1);
	gLayout->addWidget(slider2, 6, 5, 1, 1);
	gLayout->addWidget(gridvalue, 6, 6, 1, 1);

	//gLayout->addWidget(blank, 5, 5, 1, 15);
	//gLayout->addWidget(button3, 4, 3, 1, 1);
	//gLayout->addWidget(button4, 4, 5, 1, 1);
	//gLayout->addWidget(button5, 4, 6, 1, 1);
	//gLayout->addWidget(button6, 4, 7, 1, 1);
	//gLayout->addWidget(button7, 4, 8, 1, 1);
	
	gLayout->addWidget(command, 7, 0, 3, 8);
	
	//gLayout->addWidget(cmdButton1, 7, 8, 1, 1);
	//gLayout->addWidget(cmdButton2, 8, 8, 1, 1);

	window->setLayout(gLayout);
	window->setWindowModified(true);
	window->show();
}

void FacBuilding::on_startButton_clicked(){

	QString fileName = QFileDialog::getOpenFileName(NULL, tr("Open Image"), "data\\", tr("ImageFile(*.jpg *.bmp *.png)"));
	
	if (fileName.length() != 0){
		src = cv::imread(fileName.toStdString(),1);
		//const uchar *qImageBuffer = (const uchar*)src.data;
		//imshow("src", src);
		/*QMovie *mov = new QMovie(fileName);
		mov->start();
		testLabel->setMovie(mov);*/
		
		//auto resize label 
		if (2 * WINDOW_H*src.cols >= WINDOW_W*src.rows) //max height 2/3 WINDOW_H
			labelsize = Size(WINDOW_W / 3, src.rows * WINDOW_W / (3 * src.cols));
		else
			labelsize = Size(WINDOW_H * src.cols / (3 * src.rows), WINDOW_H / 3);

		Mat tempS = Mat(labelsize, CV_8UC3);	
		testLabel->setFixedSize(QSize(tempS.cols, tempS.rows));
		tempLabel->setFixedSize(QSize(tempS.cols, tempS.rows));
		resultLabel->setFixedSize(QSize(tempS.cols, tempS.rows));
		cv::resize(src, tempS, labelsize);
		QImage simage = Utility::MatToQImage(tempS);
		testLabel->setPixmap(QPixmap::fromImage(simage));

		this->command->append("load image:" + fileName +"\n");

		//init
		src_temp = src.clone();
		dst_temp = dst.clone();
		seg = new Segmentation(src, fileName.toStdString());
		slider1->setEnabled(true);
		slider2->setEnabled(true);
	}
	else
		this->command->append("load image failed...");
	
	fileName = "";
	return;
}

void FacBuilding::on_cmdButton1_clicked(){
	command->toPlainText();
	command->setReadOnly(true);
	return;
}

void FacBuilding::on_cmdButton2_clicked(){
	command->clear();
	command->setReadOnly(false);
	return; 
}

void FacBuilding::on_CommandEdit_Entered(){
	int a = 0;

	QString current = Utility::getCommandLine(command->toPlainText());
	//Utility::testCommand(current);
	//command->append("Current Command Line: "+current);
	emit sendCommand(current);
	return;
}

void FacBuilding::on_ImageLabel_rolling(QPoint angle){
		
	int y = angle.y();
}

void FacBuilding::on_this_sendCmd(QString commd){
	//command parser
	string cmd = commd.toStdString();
	if (cmd == "segment"){
		if (src.rows == 0){
			this->command->append("error load source image.. Please reload image!!");
			return;
		}

		//seg->ImageSegmentation(src, dst);

		Mat tempD = Mat(labelsize, CV_8UC3);
		resultLabel->setFixedSize(QSize(tempD.cols, tempD.rows));
		cv::resize(dst, tempD, labelsize);
		QImage dimage = Utility::MatToQImage(tempD);
		resultLabel->setPixmap(QPixmap::fromImage(dimage));

		this->command->append("segmentation done!!");	
		//delete seg;
	}
	if (cmd == "kmeans"){
		//if (src.rows != 0){
			Mat result, centers, visual;
			connect(slider1, SIGNAL(mouseReleaseEvent(QMouseEvent*)), this, SLOT(on_sym_released(int)));
			seg->kmeans_seg(src,result,centers,visual,5);
			tmp = visual.clone();
			//seg->save_kmeans("K-means");
			//seg->kmeans_seg_folder("data\\", "K-means\\");
			this->command->append("K-means done..");
			repaint();
		//}
	
	}
	if (cmd == "gco"){
		seg->gco_seg((double)slider1->value() / 100, (double)slider2->value() / 10);
		dst = seg->_dst.clone();
		repaint();
	}
	if (cmd == "s"){
		
		repaint();
		if (src.rows != 0){
			//connect(testLabel, SIGNAL(mousepress(QPoint)), this, SLOT(on_ImageLabel_mouseClick(QPoint)));//connect user select}
			connect(testLabel, SIGNAL(mousemove(QPoint, QPoint)), this, SLOT(on_ImageLabel_mouseDrag(QPoint, QPoint)));//connect user select
			this->command->append(tr("please drag some segments...."));
		}
		else{
			this->command->append("Please reload image!!");
		}
		//delete seg;
	}

	if (cmd == "d"){

		//disconnect(testLabel, SIGNAL(mousepress(QPoint)), 0,0);//connect user select}
		disconnect(testLabel, SIGNAL(mousemove(QPoint, QPoint)), 0, 0);//connect user select
		this->command->append(tr("segmenting... done!"));
		showSegment(src, _axis);
	}		

	if (cmd == "b"){
		/*test code for previous graph matching method*/
		/*if (src.rows == 0){
			this->command->append("error load source image.. Please reload image!!");
			return;
		}*/
		Basic_File fileop;
		src = imread("FacGraph-src\\10.jpg");
		Mat region;
		int flag = fileop.LoadData("FacGraph-region\\10-label.txt", region, src.rows, src.cols);
		GraphMatching gMatch;
		gMatch.doMatching(dst, "10", "FacGraph-src", "FacGraph-region", "FacGraph-depth");

		this->command->append("building done!!");

		Mat tempD = Mat(labelsize, CV_8UC3);
		resultLabel->setFixedSize(QSize(tempD.cols, tempD.rows));
		cv::resize(dst, tempD, labelsize);
		QImage dimage = Utility::MatToQImage(tempD);
		resultLabel->setPixmap(QPixmap::fromImage(dimage));	
	}

	if (cmd == "save"){//save segment result
		Mat s = src.clone();
		if (_axis.size() != 0){
			for (int i = 0; i < _axis.size(); i++){
				if (_axis[i].first == 'x'){
					line(s, Point(0, _axis[i].second), Point(s.cols, _axis[i].second), Scalar(0, 0, 255), 3, CV_AA);
				}
				else{
					line(s, Point(_axis[i].second, 0), Point(_axis[i].second, s.rows), Scalar(0, 0, 255), 3, CV_AA);
				}
			}
		}
		seg->saveSegments(src, _axis,"SegmentParts");
		this->command->append(tr("saved segment parts to directory: SegmentParts\\..."));
	}

	if (cmd == "clear"){ //clear
		this->command->clear();
	}
	
	if (cmd == "size" || cmd == "image size"){
		int rows = src.rows;
		int cols = src.cols; 
		string r, c;
		stringstream sr,sc;
		sr << rows;
		sr >> r;
		sc << cols;
		sc >> c;
		string infor = "rows:" + r +"   cols:" + c;
		this->command->append(QString::fromStdString(infor));
	}
	return;
}

void FacBuilding::keyPressEvent(QKeyEvent* e){
	/*switch (e->key()){
	case Qt::Key_Escape:
		this->close();
		break;
	default:
		QMainWindow::keyPressEvent(e);
	}*/
	return;
}

void FacBuilding::on_ImageLabel_mouseClick(QPoint pos){
	int x = pos.x();
}

void FacBuilding::on_ImageLabel_mouseDrag(QPoint start, QPoint end){

	double scale = (double)src.rows / (double)labelsize.height; //scale
	QPoint s = scale*start;
	QPoint e = scale*end;

	if (abs(s.x() - e.x()) >= 5 * abs(s.y() - e.y()) || s.y() == e.y()){
		int ax = seg->getBestSegAxis('x', (s.y() + e.y()) / 2);
		_axis.push_back(std::pair<char, int>('x', ax));
	}
		
	else{
		int ax = seg->getBestSegAxis('y', (s.x() + e.x()) / 2);
		_axis.push_back(std::pair<char, int>('y',ax));
	}
		
	line(src_temp, Point(s.x(), s.y()), Point(e.x(), e.y()), Scalar(0, 0, 255), 4, CV_AA);
	repaint();
}

void FacBuilding::repaint(){
	if (src.rows > 0 && src.cols > 0){
		Mat tempS = Mat(labelsize, CV_8UC3);
		cv::resize(src, tempS, labelsize);
		QImage simage = Utility::MatToQImage(tempS);
		testLabel->setPixmap(QPixmap::fromImage(simage));
	}
	if (tmp.rows > 0 && tmp.cols > 0){
		Mat tempT = Mat(labelsize, CV_8UC3);
		cv::resize(tmp, tempT, labelsize);
		QImage dimage = Utility::MatToQImage(tempT);
		tempLabel->setPixmap(QPixmap::fromImage(dimage));
	}
	

	if (dst.rows > 0 && dst.cols > 0){
		
		Mat tempD = Mat(labelsize, CV_8UC3);
		cv::resize(dst, tempD, labelsize);
		QImage dimage = Utility::MatToQImage(tempD);
		resultLabel->setPixmap(QPixmap::fromImage(dimage));
	}
		
	return;
}

void FacBuilding::showSegment(Mat& src, vector<pair<char, int>>& axis){
	Mat show = src.clone();
	for (int i = 0; i < axis.size(); i++){
		if (axis[i].first == 'x'){
			line(show, Point(0, axis[i].second), Point(show.cols, axis[i].second), Scalar(0, 0, 255), 3, CV_AA);
		}
		else{
			line(show, Point(axis[i].second, 0), Point(axis[i].second, show.rows), Scalar(0, 0, 255), 3, CV_AA);
		}
	}
	//imshow("show", show); waitKey(0);

	Mat tempS = Mat(labelsize, CV_8UC3);
	cv::resize(show, tempS, labelsize);
	QImage simage = Utility::MatToQImage(tempS);
	testLabel->setPixmap(QPixmap::fromImage(simage));
}

void FacBuilding::on_sym_valuechanged(int v){
	double pos = (double)slider1->value()/100;
	QString str = QString("%1").arg(pos);
	symvalue->setText(str);
}

void FacBuilding::on_sym_released(){
	double pos = (double)slider1->value() / 100;
	double pos2 = (double)slider2->value() / 100;
	if (seg != NULL){
		slider1->setEnabled(true);
		seg->gco_seg(pos,pos2);
		dst = seg->_dst.clone();
		repaint();
	}
}

void FacBuilding::on_grid_valuechanged(int v){
	double pos = (double)slider2->value() / 100;
	QString str = QString("%1").arg(pos);
	gridvalue->setText(str);
}