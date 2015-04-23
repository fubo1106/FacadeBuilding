#pragma once
#include <QTextEdit>
#include <QKeyEvent>

class CommandTextEdit 
	: public QTextEdit
{
	Q_OBJECT

public:
	CommandTextEdit(QWidget* parent = 0);
	~CommandTextEdit();

signals:
	void pressEnter();

public:
	void  keyPressEvent(QKeyEvent * ev);

public slots:
	void executeCommand();


};

