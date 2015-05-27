#pragma once
#include <QLabel>
#include <QWheelEvent>
#include <QPoint>
#include <qapplication.h>

#include <vector>
using namespace std;

class ImageLabel :
	public QLabel
{
	Q_OBJECT
public:
	ImageLabel(QWidget* parent = 0);
	~ImageLabel();

	QPoint user_tl;
	QPoint user_br;

signals:
	void wheelrolling(QPoint angleDelta);
	void mousepress(QPoint p);
	void mousemove(QPoint s, QPoint e);
	void mouserelease(vector<QPoint>& points);
	void mouserect(QPoint tl, QPoint br);
	void userstroke(QPoint tl, QPoint br);

public:
	void wheelEvent(QWheelEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);
	
	
private:
	QPoint startPos; //mouse start press position
	QPoint endPos;
	vector<QPoint> points;

	bool _mouseclick = false;
	bool _mouserelease = false;
	bool _mousemove = false;

};

