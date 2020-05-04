#include "../Header/DelInformation.h"
#include "qpushbutton.h"
#include <QtWidgets/QMainWindow>
#include <iostream>
#include<Windows.h>
#include<WinSock.h>
#include<mysql.h>

using namespace std;

#pragma execution_character_set("UTF-8")//用于qt的编码，如果没有，界面会有中文乱码
#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"wsock32.lib")

MYSQL* DelMysql = new MYSQL; //mysql连接  
MYSQL_RES* DelRes; //这个结构代表返回行的一个查询结果集  
MYSQL_ROW DelColumn; //一个行数据的类型安全(type-safe)的表示，表示数据行的列  
char DelQuery[150]; //查询语句
string DelGradeList[999];
string DelStuList[999];

bool InitDelGradeList();
bool InitStuList(string gradeNum);

DelInformation::DelInformation(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.DelGradeButton, SIGNAL(clicked()), this, SLOT(ClickGradeButton()));//将按钮和点击事件绑定
	connect(ui.DelStuButton, SIGNAL(clicked()), this, SLOT(ClickStuButton()));//将按钮和点击事件绑定
	connect(ui.SureButton, SIGNAL(clicked()), this, SLOT(ClickSureButton()));//将按钮和点击事件绑定
	connect(ui.AgainSureButton, SIGNAL(clicked()), this, SLOT(ClickAgainSureButton()));//将按钮和点击事件绑定
	connect(ui.CancelButton, SIGNAL(clicked()), this, SLOT(ClickCancelButton()));//将按钮和点击事件绑定

	this->DelGradeButton = ui.DelGradeButton;
	this->DelStuButton = ui.DelStuButton;
	this->SureButton = ui.SureButton;
	this->AgainSureButton = ui.AgainSureButton;
	this->CancelButton = ui.CancelButton;
	this->GradeComboBox = ui.GradeComboBox;
	this->StuComboBox = ui.StucomboBox;
	this->SureLabel = ui.SureLabel;
	this->StuLabel = ui.Stulabel;
	this->TagLabel = ui.TagLabel;

	//将无关的控件隐藏
	this->StuLabel->hide();
	this->StuComboBox->hide();
	this->SureLabel->hide();
	this->AgainSureButton->hide();
	this->CancelButton->hide();
	this->SureButton->hide();
	this->DelGradeButton->setStyleSheet("background:green");//设置按钮的背景颜色为绿色
	this->DelStuButton->setStyleSheet("background:white");//设置按钮的背景颜色为白色

	if (InitDelGradeList()) {
		/*for (int i = 0; i < DelGradeList->length(); i++) {
			QString Item = QString::fromStdString(DelGradeList[i]);
			GradeComboBox->addItem(Item, Item);
		}*/
	}
}

void DelInformation::ClickGradeButton() {//点击“删除班级”按钮后的逻辑

	this->DelGradeButton->setStyleSheet("background:green");
	this->DelStuButton->setStyleSheet("background:white");
	this->SureButton->show();
	this->StuLabel->hide();
	this->StuComboBox->hide();
	
}
void DelInformation::ClickStuButton(){//点击“删除学生”按钮后的逻辑
	this->DelGradeButton->setStyleSheet("background:white");
	this->DelStuButton->setStyleSheet("background:green");
	this->StuLabel->show();
	this->StuComboBox->show();
	this->SureButton->show();
}
void DelInformation::ClickSureButton(){}
void DelInformation::ClickAgainSureButton() {}
void DelInformation::ClickCancelButton(){}

bool InitDelGradeList() {
	sprintf_s(DelQuery, "select grade from gradelist"); //查询语句
	mysql_query(DelMysql, "set names utf8"); 
	if (mysql_query(DelMysql, DelQuery))    //执行SQL语句
	{
		return false;
	}
	//获取结果集  
	if (!(DelRes = mysql_store_result(DelMysql)))   //获得sql语句结束后返回的结果集  
	{
		return false;
	}
	for (int i = 0; DelColumn = mysql_fetch_row(DelRes); i++) {
		DelGradeList[i] = (DelColumn[0]);
	}
	return true;
}

bool InitStuList(string gradeNum) {
	return true;
}

