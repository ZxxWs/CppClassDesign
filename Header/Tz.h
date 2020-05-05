#pragma once

#include <QtWidgets/QMainWindow>
#include "../x64/Debug/uic/UI/ui_Tz.h"
#include "qpushbutton.h"

class Tz : public QMainWindow
{
	Q_OBJECT

public:
	Tz(QWidget *parent = Q_NULLPTR);
	QLabel* PassLabel;
	QLabel* NewPassLabel;
	QLabel* SurePassLabel;
	QLabel* TagLabel;//用于输出提示信息
	QLineEdit* PassLineEdit;
	QLineEdit* NewPassLineEdit;
	QLineEdit* SurePassLineEdit;
	QPushButton* AlterPassButton;
	QPushButton* LogInButton;	//输入密码后的确认按钮声明

public slots:
	void ClickLogInButton();//按钮点击事件
	void ClickAlterPassButton();//按钮点击事件

private:
	Ui::TzClass ui;

};
