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
	Tag = ui.TagShow;
	connect(ui.SubButton, SIGNAL(clicked()), this, SLOT(ClickButton()));//����ť�͵���¼���


	//string c = "#asd#2018.7#0.3%#fgfg#2020.01.2#2.9%";
	//Student ss = Student(10202, 120102, "����", 3.2, c, "");
	//string a =(ss.getSdetail()[1].getTime());
	//QString qstr2 = QString::fromStdString(a);
	//Tag->setText(qstr2);

}


void Tz::ClickButton() {//����ȷ�ϰ�ť����¼���ʵ��

	
	Sub->hide();
	Lab->hide();
	Search* s;
	s = new Search(this);
	s->show();

}
