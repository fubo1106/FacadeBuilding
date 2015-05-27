#include "facbuilding.h"
#include <qstring.h>
#include <qfiledialog.h>
#include <qmovie.h>
#include <qpixmap.h>
#include <qmenu.h>
#include <sstream>

extern QtAPI QtApi;

FacBuilding::FacBuilding(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);
	//window = new QWidget;
	gLayout = new QGridLayout;

	testLabel = new ImageLabel();
	resultLabel = new ImageLabel();
	tempLabel = new ImageLabel();

	testArea = new QScrollArea();
	tempArea = new QScrollArea();
	resultArea = new QScrollArea();

	startButton = new QPushButton;
	startButton->setShortcut(tr("ctrl+o"));
	
	slider1 = new QSlider(Qt::Horizontal);
	slider2 = new QSlider(Qt::Horizontal);

	selectFolder = new QPushButton;
	button4 = new QPushButton("start4");
	button5 = new QPushButton("start5");
	button6 = new QPushButton("start6");
	button7 = new QPushButton("start7");

	cmdButton1 = new QPushButton("excute1");
	cmdButton2 = new QPushButton("clear");
	
	selectFolder->setEnabled(true);
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
	connect(selectFolder, SIGNAL(clicked()), this, SLOT(on_selectFolderButton_clicked()));
	connect(cmdButton1, SIGNAL(clicked()), this, SLOT(on_cmdButton1_clicked()));
	connect(cmdButton2, SIGNAL(clicked()), this, SLOT(on_cmdButton2_clicked()));
	connect(command, SIGNAL(pressEnter()), this, SLOT(on_CommandEdit_Entered()));
	//connect(command, SIGNAL(pressEnter()), command, SLOT(executeCommand()));
	connect(testLabel, SIGNAL(wheelrolling(QPoint)), this, SLOT(on_ImageLabel_rolling(QPoint)));
	connect(this, SIGNAL(sendCommand(QString)), this, SLOT(on_this_sendCmd(QString)));

	connect(slider1, SIGNAL(valueChanged(int)), this, SLOT(on_sym_valuechanged(int)));
	connect(slider2, SIGNAL(valueChanged(int)), this, SLOT(on_grid_valuechanged(int)));
	connect(&QtApi, SIGNAL(message(string)), this, SLOT(showMsg(string)));

	log.open("../log/" + getTime() + ".log");

//	this->se.setupUi(this);
}

FacBuilding::~FacBuilding()
{
	if (seg != NULL)
		delete seg;
	log.close();
}

void FacBuilding::myLayout(){
	
	this->setWindowTitle("FacBuilding");
	//window->setWindowState(Qt::WindowMaximized);
	this->setGeometry(150, 100, WINDOW_W, WINDOW_H);
	//window->showMaximized();
	//window->setFixedSize(window->width(),window->height());

	testLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	testLabel->setFrameShape(QFrame::Box);
	testLabel->setAutoFillBackground(true);
	testLabel->setScaledContents(true);
	//testLabel->setFixedWidth(WINDOW_W / 3);
	//testLabel->setMaximumHeight(WINDOW_H / 2);
	testLabel->setText("testImage");
	testLabel->setFocusPolicy(Qt::ClickFocus);
	//testLabel->setFixedSize(WINDOW_W / 3, WINDOW_H);

	tempLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	tempLabel->setFrameShape(QFrame::Box);
	tempLabel->setAutoFillBackground(true);
	tempLabel->setScaledContents(true);
//	tempLabel->setFixedWidth(WINDOW_W / 3);
	//tempLabel->setMaximumHeight(WINDOW_H / 2);
	tempLabel->setText("tmpImage");
	tempLabel->setFocusPolicy(Qt::ClickFocus);
	//tempLabel->setFixedSize(WINDOW_W / 3, WINDOW_H);

	resultLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	resultLabel->setFrameShape(QFrame::Box);
	resultLabel->setScaledContents(true);
	//resultLabel->setFixedWidth(WINDOW_W / 3);
	//resultLabel->setMaximumHeight(WINDOW_H / 2);
	resultLabel->setText("resultImage");
	resultLabel->setFocusPolicy(Qt::ClickFocus);
	//resultLabel->setFixedSize(WINDOW_W / 3, WINDOW_H);

	testArea->setBackgroundRole(QPalette::Light);
	tempArea->setBackgroundRole(QPalette::Light);
	resultArea->setBackgroundRole(QPalette::Light);

	testArea->setWidget(testLabel);
	tempArea->setWidget(tempLabel);
	resultArea->setWidget(resultLabel);

	_srcLabel->setFixedHeight(WINDOW_H / 50);
	_tmpLabel->setFixedHeight(WINDOW_H / 50);
	_dstLabel->setFixedHeight(WINDOW_H / 50);

	startButton->setText("open Image");
	
	slider1->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
	slider1->setMinimum(0);
	slider1->setMaximum(100);
	slider1->setTickInterval(1);
	slider1->setInvertedAppearance(true);
	slider1->setValue(5);
	slider1->setEnabled(true);

	slider2->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
	slider2->setMinimum(0);
	slider2->setMaximum(100);
	slider2->setTickInterval(1);
	slider2->setInvertedAppearance(true);
	slider2->setValue(10);
	slider2->setEnabled(true);

	selectFolder->setText("select Folder");
	button4->setText("start4");
	button5->setText("start5");
	button6->setText("start6");
	button7->setText("start7");

	command->setAutoFillBackground(true);
	command->setMaximumHeight(WINDOW_H / 3);
	//command->setMinimumHeight(WINDOW_H / 5);

	//gLayout->addWidget(toolBar, 0, 0, 1, 1);
	gLayout->addWidget(testArea, 0, 0, 4, 6);
	gLayout->addWidget(tempArea, 0, 7, 4, 6);
	gLayout->addWidget(resultArea, 0, 14, 4, 6);

	//labels
	//gLayout->addWidget(_srcLabel, 5, 2, 1, 1);
	//gLayout->addWidget(_tmpLabel, 5, 8, 1, 1);
	//gLayout->addWidget(_dstLabel, 5, 15, 1, 1);

	gLayout->addWidget(startButton, 6, 0, 1, 1);
	gLayout->addWidget(selectFolder, 6, 1, 1, 1);
	gLayout->addWidget(symAttri, 6, 2, 1, 1);
	gLayout->addWidget(slider1, 6, 3, 1, 2);
	gLayout->addWidget(symvalue, 6, 5, 1, 1);
	gLayout->addWidget(edgeAttri, 6, 6, 1, 1);
	gLayout->addWidget(slider2, 6, 7, 1, 2);
	gLayout->addWidget(gridvalue, 6, 9, 1, 1);
	gLayout->addWidget(_blankLabel, 6, 10, 1, 9);
	//gLayout->addWidget(blank, 5, 5, 1, 15);
	//gLayout->addWidget(button3, 4, 3, 1, 1);
	//gLayout->addWidget(button4, 4, 5, 1, 1);
	//gLayout->addWidget(button5, 4, 6, 1, 1);
	//gLayout->addWidget(button6, 4, 7, 1, 1);
	//gLayout->addWidget(button7, 4, 8, 1, 1);
	
	//gLayout->addWidget(_blankLabel, 6, 0, 3, 8);
	
	gLayout->addWidget(command, 7, 0, 2, 20);
	//gLayout->addWidget(cmdButton1, 7, 8, 1, 1);
	//gLayout->addWidget(cmdButton2, 8, 8, 1, 1);

	this->setLayout(gLayout);
	this->setWindowModified(true);
	this->show();
}

void FacBuilding::on_startButton_clicked(){

	QString fileName = QFileDialog::getOpenFileName(NULL, tr("Open Image"), "data\\", tr("ImageFile(*.jpg *.bmp *.png)"));
	
	if (fileName.length() != 0){
		src = cv::imread(fileName.toStdString(),1);
		tmp = src.clone();
		dst = src.clone();
		//const uchar *qImageBuffer = (const uchar*)src.data;
		//imshow("src", src);
		/*QMovie *mov = new QMovie(fileName);
		mov->start();
		testLabel->setMovie(mov);*/
		
		//auto resize label 
		//if (2 * WINDOW_H*src.cols >= WINDOW_W*src.rows) //max height 2/3 WINDOW_H
		//	labelsize = Size(WINDOW_W / 3, src.rows * WINDOW_W / (3 * src.cols));
		//else
		//	labelsize = Size(WINDOW_H * src.cols / (3 * src.rows), WINDOW_H / 3);

		//Mat tempS = Mat(labelsize, CV_8UC3);	
		//testLabel->setFixedSize(QSize(tempS.cols, tempS.rows));
		//tempLabel->setFixedSize(QSize(tempS.cols, tempS.rows));
		//resultLabel->setFixedSize(QSize(tempS.cols, tempS.rows));
		//cv::resize(src, tempS, labelsize);
		//QImage simage = Utility::MatToQImage(tempS);
		//testLabel->setPixmap(QPixmap::fromImage(simage));
		testLabel->resize(src.cols, src.rows);
		QImage simage = Utility::MatToQImage(src);
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

void FacBuilding::on_selectFolderButton_clicked(){
	QString fileName = QFileDialog::getOpenFileName(NULL, tr("Open Image"), "data\\training\\", tr("ImageFile(*.jpg *.bmp *.png)"));
	string str = fileName.toStdString();

	int s=0, e=0;
	for (int i = 0; i < str.length(); i++){
		if (str.at(i) == '/'){
			s = e;
			e = i;
		}
	}
	string currentDir = str.substr(s + 1, e - s - 1);

	if (seg != NULL)
		delete seg;
	seg = new Segmentation();
	seg->_segFolder = true;
	
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
			
		if (seg->_segFolder)
			seg->kmeans_seg_folder("data\\training\\", "K-means\\");
		else{
			Mat result, centers, visual;
			connect(slider1, SIGNAL(mouseReleaseEvent(QMouseEvent*)), this, SLOT(on_sym_released(int)));
			seg->kmeans_seg(src, result, centers, visual, 5);
			dst_temp = visual.clone();
			seg->save_kmeans("K-means\\");
			//seg->kmeans_seg_folder("data\\", "K-means\\");
			this->command->append("K-means done..");
		}	
			repaint();
		//}
	
	}
	if (cmd == "gco"){
		seg->gco_seg((double)slider1->value() / 100, (double)slider2->value() / 10);
		dst = seg->_dst.clone();
		repaint();
	}
	if (cmd == "gco-all"){
		
		//training folders images
		if (seg == NULL)
			seg = new Segmentation();
		seg->kmeans_seg_folder("data\\training\\", "K-means\\");
		seg->gco_segAll("data\\training\\","output",(double)slider1->value() / 100, (double)slider2->value() / 10);
		/*dst = seg->_dst.clone();
		repaint();*/
	}
	if (cmd == "s"){
		
		repaint();
		if (src.rows != 0){
			//connect(testLabel, SIGNAL(mousepress(QPoint)), this, SLOT(on_ImageLabel_mouseClick(QPoint)));//connect user select}
			connect(testLabel, SIGNAL(mouserelease(vector<QPoint>&)), this, SLOT(on_ImageLabel_mouseRelease(vector<QPoint>&)));//connect user stroke
			connect(testLabel, SIGNAL(mousemove(QPoint,QPoint)), this, SLOT(on_ImageLabel_mouseDrag(QPoint, QPoint)));//connect user stroke
			this->command->append(tr("please drag some segments...."));
		}
		else{
			this->command->append("Please reload image!!");
		}
		//delete seg;
	}

	if (cmd == "d"){

		//disconnect(testLabel, SIGNAL(mousepress(QPoint)), 0,0);//connect user select}
		disconnect(testLabel, SIGNAL(mouserelease(vector<QPoint>&)), 0, 0);//connect user select
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

	if (cmd == "label"){
		this->command->append(tr("User stroke for labeling each region.\n")+
								 tr("1 => depth:30			\n")+
								 tr("2 => depth:100			\n") +
								 tr("3 => depth:170			\n") +
								 tr("4 => depth:200			\n") +
								 tr("5 => depth:230			\n") +
								 tr("Causion!! your following stroke will not overlap previous label.") );

		if (seg == NULL)
			seg = new Segmentation();
		else{
			this->command->append(tr("segmentation already exist"));
			return;
		}
		
		seg->loadImages("data\\training\\");
		connect(testLabel, SIGNAL(userstroke(QPoint, QPoint)), this, SLOT(on_user_selected(QPoint, QPoint)));
		connect(testLabel, SIGNAL(mouserect(QPoint, QPoint)), this, SLOT(on_rect_changed(QPoint, QPoint)));
	}

	if (cmd == "test"){ //clear
		string time = Utility::getTime();
	}
	return;
}

void FacBuilding::keyPressEvent(QKeyEvent* e){
	switch (e->key()){
	case Qt::Key_Escape:
		this->close();
		break;
	case Qt::Key_Space:
		if (seg == NULL){
			return;
		}
		if (seg->labelNext()){
			src_temp = seg->_src;
			src = seg->_src;
			repaint();
		}
		break;

	case Qt::Key_0:
		seg->depth = DEPTH_0;
		seg->userLabel(testLabel->user_tl, testLabel->user_br);
		src_temp = seg->_src;
		repaint();
		break;

	case Qt::Key_1:
		seg->depth = DEPTH_1;
		seg->userLabel(testLabel->user_tl, testLabel->user_br);
		src_temp = seg->_src;
		repaint();
		break;

	case Qt::Key_2:
		seg->depth = DEPTH_2;
		seg->userLabel(testLabel->user_tl, testLabel->user_br);
		src_temp = seg->_src;
		repaint();
		break;

	case Qt::Key_3:
		seg->depth = DEPTH_3;
		seg->userLabel(testLabel->user_tl, testLabel->user_br);
		src_temp = seg->_src;
		repaint();
		break;

	case Qt::Key_4:
		seg->depth = DEPTH_4;
		seg->userLabel(testLabel->user_tl, testLabel->user_br);
		src_temp = seg->_src;
		repaint();
		break;
	case Qt::Key_Enter:
		seg->saveLabels("data\\training\\label\\");
		src_temp = seg->_src;
		repaint();
		QtApi.sendMsg("save labels to data\\training\\label\\ \n");
		break;
	case Qt::Key::Key_Backspace:
		if (seg == NULL){
			return;
		}
		if (seg->labelPrevious()){
			src_temp = seg->_src;
			src = seg->_src;
			repaint();
		}
		break;

	default:
		QWidget::keyPressEvent(e);
	}
	return;
}

void FacBuilding::on_ImageLabel_mouseClick(QPoint pos){
	int x = pos.x();
}

void FacBuilding::on_ImageLabel_mouseDrag(QPoint start, QPoint end){

	double scale = (double)src.rows / (double)labelsize.height; //scale
	scale = 1;

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
		
	line(src_temp, Point(s.x(), s.y()), Point(e.x(), e.y()), Scalar(0, 255, 0), 2, CV_AA);
	repaint();
	//imshow("src_temp", src_temp); waitKey(0);
}

void FacBuilding::on_ImageLabel_mouseRelease(vector<QPoint>& points){

	repaint();
}

void FacBuilding::repaint(){
	if (src_temp.rows > 0 && src_temp.cols > 0){
		/*Mat tempS = Mat(labelsize, CV_8UC3);
		cv::resize(src_temp, tempS, labelsize);
		QImage simage = Utility::MatToQImage(tempS);
		testLabel->setPixmap(QPixmap::fromImage(simage));*/
		testLabel->resize(src_temp.cols, src_temp.rows);
		QImage simage = Utility::MatToQImage(src_temp);
		testLabel->setPixmap(QPixmap::fromImage(simage));
	}
	if (dst_temp.rows > 0 && dst_temp.cols > 0){
		/*Mat tempT = Mat(labelsize, CV_8UC3);
		cv::resize(dst_temp, tempT, labelsize);
		QImage dimage = Utility::MatToQImage(tempT);
		tempLabel->setPixmap(QPixmap::fromImage(dimage));*/
		tempLabel->resize(dst_temp.cols, dst_temp.rows);
		QImage simage = Utility::MatToQImage(dst_temp);
		tempLabel->setPixmap(QPixmap::fromImage(simage));
	}
	

	if (dst.rows > 0 && dst.cols > 0){
		/*Mat tempD = Mat(labelsize, CV_8UC3);
		cv::resize(dst, tempD, labelsize);
		QImage dimage = Utility::MatToQImage(tempD);
		resultLabel->setPixmap(QPixmap::fromImage(dimage));*/
		resultLabel->resize(dst.cols, dst.rows);
		QImage simage = Utility::MatToQImage(dst);
		resultLabel->setPixmap(QPixmap::fromImage(simage));
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

void FacBuilding::on_rect_changed(QPoint tl, QPoint br){
	src_temp = src.clone();

	if (br.x() >= src.cols)
		br = QPoint(src.cols - 1, br.y());
	if (br.y() >= src.rows)
		br = QPoint(br.x(), src.rows - 1);

	testLabel->user_br = br;

	line(src_temp, Point(tl.x(), tl.y()), Point(br.x(), tl.y()), Scalar(0, 0, 255), 1, CV_AA);
	line(src_temp, Point(tl.x(), tl.y()), Point(tl.x(), br.y()), Scalar(0, 0, 255), 1, CV_AA);
	line(src_temp, Point(br.x(), br.y()), Point(br.x(), tl.y()), Scalar(0, 0, 255), 1, CV_AA);
	line(src_temp, Point(br.x(), br.y()), Point(tl.x(), br.y()), Scalar(0, 0, 255), 1, CV_AA);
	repaint();
}

void FacBuilding::on_user_selected(QPoint tl, QPoint br){

	QtApi.sendMsg("Please choose label...\n");

	if (br.x() >= src.cols)
		br = QPoint(src.cols - 1, br.y());
	if (br.y() >= src.rows)
		br = QPoint(br.x(), src.rows - 1);

	testLabel->user_br = br;

	/*for (int i = tl.x(); i < br.x();i++)
		for (int j = tl.y(); j < br.y(); j++){
			src_temp.at<Vec3b>(j, i) = Vec3b(0,0,0);
		}*/
	
	QtApi.sendMsg("0 => depth:30			\n" \
				"1 => depth:100			\n" \
				"2 => depth:200			\n" \
				"3 => depth:230			\n" \
				"4 => depth:170			\n" \
				"enter for next. \n" \
				"Causion!! your following stroke will not overlap previous label.\n");

	QtApi.sendMsg("select label (0,1,2,3,4)\n");

	src_temp = seg->_src;
	repaint();
}

void FacBuilding::showMsg(string msg){
	this->command->insertPlainText(QString::fromStdString(msg));
	this->command->moveCursor(QTextCursor::End);
	QApplication::processEvents();
	log << getTime()+": "+msg;
}