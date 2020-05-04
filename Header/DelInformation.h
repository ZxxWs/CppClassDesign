#pragma once
#include <QtWidgets/QMainWindow>
#include "../x64/Debug/uic/UI/ui_DelInformation.h"
#include "qpushbutton.h"

class DelInformation : public QMainWindow
{
	Q_OBJECT

public:
	DelInformation(QWidget* parent = Q_NULLPTR);
	QPushButton* DelGradeButton;
	QPushButton* DelStuButton;
	QPushButton* SureButton;
	QPushButton* AgainSureButton;
	QPushButton* CancelButton;
	QComboBox* GradeComboBox;
	QComboBox* StuComboBox;
	QLabel* SureLabel;
	QLabel* StuLabel;
	//QLabel* Lab = new QLabel;

public slots:
	void ClickButton();//按钮点击事件

private:
	Ui::DelInformation ui;

};
