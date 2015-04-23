#include "facbuilding.h"
#include <qstring.h>
#include <qfiledialog.h>
#include <qmovie.h>
#include <qpixmap.h>

FacBuilding::FacBuilding(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	window = new QWidget;
	gLayout = new QGridLayout;

	testLabel = new QLabel();
	resultLabel = new QLabel();
	startButton = new QPushButton("start");
	button1 = new QPushButton("start");
	button2 = new QPushButton("start");
	button3 = new QPushButton("start");
	button4 = new QPushButton("start");
	button5 = new QPushButton("start");
	button6 = new QPushButton("start");
	button7 = new QPushButton("start");

	cmdButton1 = new QPushButton("excute1");
	cmdButton2 = new QPushButton("clear");

	button1->setEnabled(false);
	button2->setEnabled(false);
	button3->setEnabled(false);
	button4->setEnabled(false);
	button5->setEnabled(false);
	button6->setEnabled(false);
	button7->setEnabled(false);

	command = new CommandTextEdit;
//	command->setFocusPolicy(Qt::StrongFocus);
	command->setPlainText("start...\n");

	connect(startButton, SIGNAL(clicked()), this, SLOT(on_startButton_clicked()));
	connect(cmdButton1, SIGNAL(clicked()), this, SLOT(on_cmdButton1_clicked()));
	connect(cmdButton2, SIGNAL(clicked()), this, SLOT(on_cmdButton2_clicked()));
	connect(command, SIGNAL(pressEnter()), this, SLOT(on_CommandEdit_Entered()));
	//connect(command, SIGNAL(pressEnter()), command, SLOT(executeCommand()));
}

FacBuilding::~FacBuilding()
{
}

void FacBuilding::myLayout(){

	window->setWindowTitle("FacBuilding");
	window->setGeometry(150, 100, WINDOW_W, WINDOW_H);

	testLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	testLabel->setFrameShape(QFrame::Box);
	testLabel->setAutoFillBackground(true);
	testLabel->setScaledContents(true);
	testLabel->setMinimumHeight(WINDOW_H /3);
	testLabel->setMinimumWidth(WINDOW_W / 3);
	testLabel->setMaximumHeight(WINDOW_H /2);
	testLabel->setMaximumWidth(WINDOW_W /2);
	testLabel->setText("testImage");

	resultLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	resultLabel->setFrameShape(QFrame::Box);
	resultLabel->setScaledContents(true);
	resultLabel->setMinimumHeight(WINDOW_H / 3);
	resultLabel->setMinimumWidth(WINDOW_W / 3);
	resultLabel->setMaximumHeight(WINDOW_H / 2);
	resultLabel->setMaximumWidth(WINDOW_W /2);
	resultLabel->setText("resultImage");

	startButton->setText("start");
	button1->setText("start");
	button2->setText("start");
	button3->setText("start");
	button4->setText("start");
	button5->setText("start");
	button6->setText("start");
	button7->setText("start");



	command->setAutoFillBackground(true);
	command->setMaximumHeight(WINDOW_H / 3);
	command->setMinimumHeight(WINDOW_H / 5);

	gLayout->addWidget(testLabel, 0, 0, 3, 4);
	gLayout->addWidget(resultLabel, 0, 5, 3, 4);

	gLayout->addWidget(startButton, 4, 0, 1, 1);
	gLayout->addWidget(button1, 4, 1, 1, 1);
	gLayout->addWidget(button2, 4, 2, 1, 1);
	gLayout->addWidget(button3, 4, 3, 1, 1);
	gLayout->addWidget(button4, 4, 5, 1, 1);
	gLayout->addWidget(button5, 4, 6, 1, 1);
	gLayout->addWidget(button6, 4, 7, 1, 1);
	gLayout->addWidget(button7, 4, 8, 1, 1);
	
	gLayout->addWidget(command, 6, 0, 3, 8);
	
	gLayout->addWidget(cmdButton1, 7, 8, 1, 1);
	gLayout->addWidget(cmdButton2, 8, 8, 1, 1);

	window->setLayout(gLayout);
	window->setWindowModified(true);
	window->show();
}

void FacBuilding::on_startButton_clicked(){
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("ImageFile(*.jpg *.jpg *.bmp)"));
	if (fileName != NULL){
		src = cv::imread(fileName.toStdString());
		//const uchar *qImageBuffer = (const uchar*)src.data;
		//imshow("src", src);
		/*QMovie *mov = new QMovie(fileName);
		mov->start();
		testLabel->setMovie(mov);*/

		

		QImage qimage = Utility::MatToQImage(src);
		testLabel->setPixmap(QPixmap::fromImage(qimage));
		resultLabel->setPixmap(QPixmap::fromImage(qimage));

	}
	command->setEnabled(true);
	
	
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
	Utility::testCommand(current);

	command->append("Current Command Line: "+current);

	return;
}

