#include "facbuilding.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FacBuilding w;
	w.myLayout();
	return a.exec();
}
