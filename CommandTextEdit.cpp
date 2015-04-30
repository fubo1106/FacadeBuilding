#include "CommandTextEdit.h"


CommandTextEdit::CommandTextEdit(QWidget* parent)
	:QTextEdit(parent)
{
	//connect(this, SIGNAL(keyPressEvent()), this, SLOT(executeCommand()));
}


CommandTextEdit::~CommandTextEdit()
{
}

//void CommandTextEdit::keyPressEvent(QKeyEvent* ev){
//	if (ev->key() == Qt::Key_Enter){
//		emit pressEnter();
//	}
//}

void CommandTextEdit::keyPressEvent(QKeyEvent* ev){
	if (ev->key() == Qt::Key_Enter || ev->key()==Qt::Key_Return){
		emit pressEnter();
	//	ev->accept();
//		return;
		//executeCommand();
	}

	QTextEdit::keyPressEvent(ev);
}

void CommandTextEdit::executeCommand(){
	int a = 0;
	return;
}

