#pragma once
#include <QLabel>
#include <QWheelEvent>
#include <QPoint>
#include <qapplication.h>

class ImageLabel :
	public QLabel
{
	Q_OBJECT
public:
	ImageLabel(QWidget* parent = 0);
	~ImageLabel();

signals:
	void wheelrolling(QPoint angleDelta);
	void mousepress(QPoint p);
	void mousemove(QPoint start, QPoint end);
	void mouserelease();

public:
	void wheelEvent(QWheelEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);

private:
	QPoint startPos; //mouse start press position
	QPoint endPos;

	bool _mouseclick = false;
	bool _mouserelease = false;
	bool _mousemove = false;

};

