#include "ImageLabel.h"


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
		startPos = e->pos();
		emit mousepress(startPos);
	}
	QLabel::mousePressEvent(e);
}

void ImageLabel::mouseMoveEvent(QMouseEvent* e){
	
	if (e->buttons() & Qt::LeftButton) {
		endPos = e->pos();
		int distance = (endPos - startPos).manhattanLength();
		if (distance >= QApplication::startDragDistance())
			_mousemove = true;

		//_mouserelease = false;
		QLabel::mouseMoveEvent(e);
	}

}

void ImageLabel::mouseReleaseEvent(QMouseEvent* e){
	if (e->button() == Qt::LeftButton){
		endPos = e->pos();
		_mouserelease = true;
		if (_mousemove){
			emit mousemove(startPos, endPos);
			_mousemove = false;
			_mouserelease = false;
		}
	}
	
	QLabel::mouseReleaseEvent(e);
}