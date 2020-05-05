#pragma once
#include <QtWidgets/QMainWindow>
#include "../x64/Debug/uic/UI/ui_DelInformation.h"
#include "qpushbutton.h"

class DelInformation : public QMainWindow
{
	Q_OBJECT

public:
	DelInformation(QWidget* parent = Q_NULLPTR);//自动生成的
	QPushButton* DelGradeButton;
	QPushButton* DelStuButton;
	QPushButton* SureButton;
	QPushButton* AgainSureButton;
	QPushButton* CancelButton;
	QComboBox* GradeComboBox;//下拉菜单
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

signals:
	void sendsignal();//这个函数用户向主界面通知关闭的消息

private:
	Ui::DelInformation ui;

protected:
	void closeEvent(QCloseEvent* event);//关闭界面的逻辑，主要是给上个界面来传递参数

};
