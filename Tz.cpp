#include "Tz.h"
#include "Header/Search.h"
#include "qpushbutton.h"
#include <QtWidgets/QMainWindow>

#include "Header/Student.h"

Tz::Tz(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	Sub = ui.SubButton;//����ť��UI�ϵĿؼ���
	Lab = ui.label;
	connect(ui.SubButton, SIGNAL(clicked()), this, SLOT(ClickButton()));//����ť�͵���¼���

}


void Tz::ClickButton() {//����ȷ�ϰ�ť����¼���ʵ��

	Sub->hide();
	Lab->hide();
	Search* s;
	s = new Search(this);
	s->show();

}
