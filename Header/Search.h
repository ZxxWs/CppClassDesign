#pragma once
#include <QtWidgets/QMainWindow>
#include "../x64/Debug/uic/UI/ui_Search.h"


class Search : public QMainWindow
{
	Q_OBJECT

public:
	Search(QWidget* parent = Q_NULLPTR);

	QComboBox* GradeComboBox = new QComboBox;
	QComboBox* NumComboBox = new QComboBox;
	QLabel* LabNum = new QLabel;
	QLabel* LabName = new QLabel;
	QLabel* LabGrade = new QLabel;
	QLabel* LabScore = new QLabel;
	QLabel* LabRemake = new QLabel;
	QTableView* ShowTable=new QTableView;
	QPushButton* AddButton = new QPushButton;
	QPushButton* DelButton= new QPushButton;
	QPushButton* AlterButton=new QPushButton;
	QPushButton* OutButton=new QPushButton;

public slots:
	void GradeComboBoxChanged();//班号列表里面的值发生了改变之后，自动识别
	void NumComboBoxChanged();//学号号列表…………
	void ClickAddButton();//添加按钮点击函数的声明
	void ClickDelButton();//添加按钮点击函数的声明
	void ClickAlterButton();//添加按钮点击函数的声明
	void ClickOutButton();//添加按钮点击函数的声明

private:
	Ui::Search ui;
};
