#include "Tz.h"
#include "Header/AddInformation.h"
#include <QtWidgets/QApplication>
#include <qtextcodec.h>

int main(int argc, char *argv[])
{


	QTextCodec* codec = QTextCodec::codecForName("GBK");//�޸������� 
	QApplication a(argc, argv);
	//Tz w;
	//w.show();
	//Search s;
	//s.show();
	AddInformation add;
	add.show();
	return a.exec();
}
