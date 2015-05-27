#include "ImageLabel.h"
#include <iostream>

ImageLabel::ImageLabel(QWidget* parent)
	:QLabel(parent)
{
}


ImageLabel::~ImageLabel()
{
}

void ImageLabel::wheelEvent(QWheelEvent* e){
	if (e->angleDelta().y() >= 0)
		int a = 0;
	if (e->angleDelta().y() < 0)
		int a = 0;
	emit wheelrolling(e->angleDelta());

	QLabel::wheelEvent(e);
}

void ImageLabel::mousePressEvent(QMouseEvent* e){
	if (e->button() == Qt::LeftButton){
		_mouseclick = true;
		startPos = e->pos();
		user_tl = startPos;
		points.push_back(startPos);
		emit mousepress(startPos);
	}
	QLabel::mousePressEvent(e);
}

void ImageLabel::mouseMoveEvent(QMouseEvent* e){
	if (_mouseclick){
		if (e->buttons() & Qt::LeftButton) {
			endPos = e->pos();

			//int distance = (endPos - startPos).manhattanLength();
			//if (distance >= QApplication::startDragDistance()){
				_mousemove = true;
				emit mousemove(points[points.size()-1],endPos);
				emit mouserect(startPos, endPos);
				/*std::cout << "points:" << points[points.size() - 1].x() << "  " << points[points.size() - 1].y() << endl
					<< points[points.size() - 1].x() << "  " << points[points.size() - 1].y() << endl << endl;*/
				points.push_back(endPos);
				
			//}
			//_mouserelease = false;
		}
	}
	QLabel::mouseMoveEvent(e);
}

void ImageLabel::mouseReleaseEvent(QMouseEvent* e){
	if (e->button() == Qt::LeftButton){
		_mouserelease = true;
		user_br = endPos;
		if (_mousemove){
			emit mouserelease(points);
			if (endPos.x()>startPos.x() && endPos.y()>startPos.y())
			emit userstroke(startPos, endPos);
			_mousemove = false;
			_mouserelease = false;
			_mouseclick = false;
		}
	}
	
	QLabel::mouseReleaseEvent(e);
}