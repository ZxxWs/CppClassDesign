#define _CRT_SECURE_NO_WARNINGS

#pragma execution_character_set("UTF-8")//用于qt的编码，如果没有，界面会有中文乱码
#include "../Header/AlterDetailUI.h"
#include "qpushbutton.h"
#include <QtWidgets/QMainWindow>

using namespace std;

//弹出的提示窗口，0表示修改学分细则成功。1表示修改失败
//这里的tag设置为int是为了防止之后有别的窗口调用来显示信息。
AlterDetailUI::AlterDetailUI(int tag,QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->ShowLabel = ui.ShowLabel;
	this->BackButton = ui.BackButton;

	connect(ui.BackButton, SIGNAL(clicked()), this, SLOT(ClickBackButton()));//将按钮和点击事件绑定
	if (tag==0) {
		ShowLabel->setText("学分细则修改成功。");
	}
	else if(tag==1)
	{
		ShowLabel->setText("学分细则修改失败。");
	}
	
}

void AlterDetailUI::ClickBackButton() {//返回按钮点击事件
	emit sendsignal();//给父界面传递被关闭信息
	this->close();
}

void AlterDetailUI::closeEvent(QCloseEvent* event) {
	emit sendsignal(); // 给父界面传递被关闭信息
}
