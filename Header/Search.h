#pragma once

#include <QtWidgets/QMainWindow>
#include "../x64/Debug/uic/ui_Search.h"
#include "qpushbutton.h"

class Search : public QMainWindow
{
	Q_OBJECT

public:
	Search(QWidget* parent = Q_NULLPTR);
	QPushButton* Sub = new QPushButton;	//输入密码后的确认按钮声明

public slots:
	void ClickButton();//按钮点击事件

private:
	Ui::Form ui;

};
