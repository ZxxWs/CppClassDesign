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
	QPushButton* AlterButton=new QPushButton;
	QPushButton* OutButton=new QPushButton;

	QLabel* lab = new QLabel;

private:
	Ui::Form ui;
};
