#include "Tz.h"
#include "Header/Search.h"
#include "qpushbutton.h"
#include <QtWidgets/QMainWindow>

#include "Header/Student.h"

Tz::Tz(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	Sub = ui.SubButton;//将按钮和UI上的控件绑定
	Lab = ui.label;
	connect(ui.SubButton, SIGNAL(clicked()), this, SLOT(ClickButton()));//将按钮和点击事件绑定

}


void Tz::ClickButton() {//密码确认按钮点击事件的实现

	Sub->hide();
	Lab->hide();
	Search* s;
	s = new Search(this);
	s->show();

}
