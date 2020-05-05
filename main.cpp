#include "Header/Tz.h"
#include "Header/Search.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);//程序自动生成
	/*Tz w;
	w.show();*/
	Search s;
	s.show();
	return a.exec();
}
