#include "Tz.h"
#include "Header/Search.h"
#include <QtWidgets/QApplication>
#include <qtextcodec.h>

int main(int argc, char *argv[])
{


	QTextCodec* codec = QTextCodec::codecForName("GBK");//�޸������� 
	QApplication a(argc, argv);
	//Tz w;
	//w.show();
	Search s;
	s.show();
	return a.exec();
}
