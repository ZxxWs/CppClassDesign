#include "../Header/Search.h"
#include <QtWidgets/QMainWindow>

Search::Search(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);//��Search���ڷŵ�Tz�����У��󶨣�
}




