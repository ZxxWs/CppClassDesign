#pragma once
#include <QtWidgets/QMainWindow>
#include "../x64/Debug/uic/UI/ui_AddInformation.h"
#include <qtextedit.h>
#include <QLabel>

class AddInformation : public QMainWindow
{
	Q_OBJECT

public:
	AddInformation(QWidget* parent = Q_NULLPTR);
	QPushButton* AddGradeButton;
	QPushButton* AddStuButton;
	QPushButton* SureButton;
	QLabel* GradeListLable;
	QLabel* NameLable;
	QLabel* NumLabel;
	QLabel* RemarkLable;
	QLabel* TagLable;
	QComboBox* GradeComboBox;
	QTextEdit* RemarkTextEdit;
	QLineEdit* NumLineEdit;
	QLineEdit* NameLineEdit;

public slots:
	void ClickGradeButton();//按钮点击函数的声明
	void ClickStuButton();//按钮点击函数的声明
	void ClickSureButton();//按钮点击函数的声明

signals:
	void sendsignal();//这个函数用户向主界面通知关闭的消息

private:
	Ui::AddInformation ui;

protected:
	void closeEvent(QCloseEvent* event);//关闭界面的逻辑，主要是给上个界面来传递参数

};
