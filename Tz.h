#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Tz.h"
#include "qpushbutton.h"

class Tz : public QMainWindow
{
	Q_OBJECT

public:
	Tz(QWidget *parent = Q_NULLPTR);
	QPushButton* Sub = new QPushButton;	//输入密码后的确认按钮声明
	QLabel* Lab = new QLabel;

public slots:
	void ClickButton();//按钮点击事件

private:
	Ui::TzClass ui;

};
