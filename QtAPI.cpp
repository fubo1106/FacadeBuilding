#include "QtAPI.h"
QtAPI QtApi;


QtAPI::QtAPI()
{

}

QtAPI::~QtAPI()
{
}

void QtAPI::sendMsg(string msg){
	emit message(msg);
	return;
}
