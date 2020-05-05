#define _CRT_SECURE_NO_WARNINGS

#pragma execution_character_set("UTF-8")//用于qt的编码，如果没有，界面会有中文乱码
#include "../Header/AlterDetailUI.h"
#include "qpushbutton.h"
#include <QtWidgets/QMainWindow>


using namespace std;

AlterDetailUI::AlterDetailUI(bool tag,QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->ShowLabel = ui.ShowLabel;
	this->BackButton = ui.BackButton;
	connect(ui.BackButton, SIGNAL(clicked()), this, SLOT(ClickBackButton()));//将按钮和点击事件绑定
	if (tag) {
		ShowLabel->setText("学分细则修改成功。");
	}
	else
	{
		ShowLabel->setText("学分细则修改失败。");
	}
}

void AlterDetailUI::ClickBackButton() {
	emit sendsignal();
	this->close();
}

void AlterDetailUI::closeEvent(QCloseEvent* event) {
	emit sendsignal();
}
