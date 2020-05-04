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
	QLabel* TagLabel;


public slots:
	void ClickGradeButton();//按钮点击事件
	void ClickStuButton();
	void ClickSureButton();
	void ClickAgainSureButton();
	void ClickCancelButton();
	void GradeComboBoxChanged();
	void StuComboBoxChanged();
private:
	Ui::DelInformation ui;

};
