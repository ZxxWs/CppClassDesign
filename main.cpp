#include "Header/Tz.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);//�����Զ�����
	Tz w;
	w.show();

	return a.exec();
}
