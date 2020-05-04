#include "Header/Tz.h"
#include "Header/AddInformation.h"
#include "Header/Search.h"
#include "Header/DelInformation.h"
#include <QtWidgets/QApplication>
#include <qtextcodec.h>

int main(int argc, char *argv[])
{


	//QTextCodec* codec = QTextCodec::codecForName("GBK");//修改这两行 
	QApplication a(argc, argv);
	//Tz w;
	////w.show();
	//Search s;
	//s.show();
	//AddInformation add;
	//add.show();
	DelInformation dd;
	dd.show();
	return a.exec();
}
