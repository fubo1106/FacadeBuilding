#pragma once
#include <qobject.h>
#include <string.h>
using namespace std;

class QtAPI :
	public QObject
{
	Q_OBJECT
public:
	QtAPI();
	~QtAPI();

	void sendMsg(string msg);

signals:
	void message(string msg);
};

