#include "../Header/Search.h"
#include <QtWidgets/QMainWindow>

Search::Search(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);//将Search窗口放到Tz窗口中（绑定）

	this->GradeList = ui.GradeList;
	this->NumList = ui.NumList;
	this->LabNum = ui.LabNum;
	this->LabName = ui.LabName;
	this->LabGrade = ui.LabGrade;
	this->LabScore = ui.LabScore;
	this->LabRemake = ui.LabRemark;
	this->ShowTable=ui.ShowTable;
	this->AlterButton = ui.AlterButton;
	this->OutButton = ui.OutButton;
}




