#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Tz.h"
#include "qpushbutton.h"

class Tz : public QMainWindow
{
	Q_OBJECT

public:
	Tz(QWidget *parent = Q_NULLPTR);
	QPushButton* Sub = new QPushButton;	//����������ȷ�ϰ�ť����
	QLabel* Lab = new QLabel;

public slots:
	void ClickButton();//��ť����¼�

private:
	Ui::TzClass ui;

};
