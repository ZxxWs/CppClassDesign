#include "Tz.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Tz w;
	w.show();
	return a.exec();
}
