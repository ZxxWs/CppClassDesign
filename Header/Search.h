#pragma once

#include <QtWidgets/QMainWindow>
#include "../x64/Debug/uic/ui_Search.h"
#include "qpushbutton.h"

class Search : public QMainWindow
{
	Q_OBJECT

public:
	Search(QWidget* parent = Q_NULLPTR);
	QPushButton* Sub = new QPushButton;	//����������ȷ�ϰ�ť����

public slots:
	void ClickButton();//��ť����¼�

private:
	Ui::Form ui;

};
