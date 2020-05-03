#include "../Header/AddInformation.h"
#include "qpushbutton.h"
#include <QtWidgets/QMainWindow>
#include<iostream>
#include<Windows.h>
#include<WinSock.h>
#include<mysql.h>

using namespace std;

#pragma execution_character_set("UTF-8")//用于qt的编码，如果没有，界面会有中文乱码
#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"wsock32.lib")

MYSQL* AddMysql = new MYSQL; //mysql连接  
MYSQL_RES* AddRes; //这个结构代表返回行的一个查询结果集  
MYSQL_ROW AddColumn; //一个行数据的类型安全(type-safe)的表示，表示数据行的列  
char AddQuery[150]; //查询语句
int AddTag;//用这个Tag区分是添加班级还是添加学生(0:班级，1：学生）
string TagString;//用于记录添加过程的问题
string AddGradeList[9999];//班级列表


bool AddConnectDatabase();


AddInformation::AddInformation(QWidget* parent)//添加信息界面的构造函数
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->AddGradeButton = ui.AddGradeButton;
	this->AddStuButton = ui.AddStuButton;
	this->SureButton = ui.SureButton;
	this->GradeListLable = ui.GradeListLabel;
	this->NameLable = ui.NameLabel;
	this->NumLabel = ui.NumLabel;
	this->RemarkLable = ui.RemarkLabel;
	this->TagLable = ui.TagLabel;
	this->GradeComboBox = ui.GradeComboBox;
	this->RemarkTextEdit = ui.RemarkTextEdit;
	this->NumLineEdit = ui.NumLineEdit;
	this->NameLineEdit = ui.NameLineEdit;

	this->label=ui.label;

	//添加按钮和点击函数的绑定
	connect(ui.AddGradeButton, SIGNAL(clicked()), this, SLOT(ClickGradeButton()));
	connect(ui.AddStuButton, SIGNAL(clicked()), this, SLOT(ClickStuButton()));

	this->SureButton->setFocusPolicy(Qt::NoFocus);//初始化的时候确认添加按钮不能点击
	//初始化的时候，控件都隐藏
	this->GradeListLable -> hide();
	this->NameLable -> hide();
	this->NumLabel ->hide();
	this->RemarkLable -> hide();
	this->TagLable -> hide();
	this->GradeComboBox -> hide();
	this->RemarkTextEdit -> hide();
	this->NumLineEdit -> hide();
	this->NameLineEdit -> hide();
}


void AddInformation::ClickGradeButton(){//点击添加班级按钮后的事件

	AddTag = 0;
	this->SureButton->setFocusPolicy(Qt::ClickFocus);//点击添加按钮后，将确认按钮设置为可以点击
	this->NumLabel->show();
	this->NumLineEdit->show();
	this->GradeListLable->hide();
	this->NameLable->hide();
	this->RemarkLable->hide();
	this->TagLable->hide();
	this->GradeComboBox->hide();
	this->RemarkTextEdit->hide();
	this->NameLineEdit->hide();
	this->NumLabel->setText("输入班号");
}

void AddInformation::ClickStuButton() {
	AddTag = 1;
	this->SureButton->setFocusPolicy(Qt::ClickFocus);//点击添加按钮后，将确认按钮设置为可以点击
	this->NumLabel->show();
	this->NumLineEdit->show();
	this->GradeListLable->show();
	this->NameLable->show();
	this->RemarkLable->show();
	this->TagLable->hide();
	this->GradeComboBox->show();
	this->RemarkTextEdit->show();
	this->NameLineEdit->show();
	this->NumLabel->setText("输入学号");

	if (AddConnectDatabase()) {//填充班号列表
		GradeComboBox->clear();//先将列表清空
		for (int i = 0; i < AddGradeList->length(); i++) {//填充班级列表
			QString Item = QString::fromStdString(AddGradeList[i]);//将String类型转换成Qstring类型
			GradeComboBox->addItem(Item, Item);
		}
		label->setText("sdsd");
	}


}

bool AddConnectDatabase() {
	//初始化mysql  
	mysql_init(AddMysql);
	//返回false则连接失败，返回true则连接成功  
	if (!(mysql_real_connect(AddMysql, "localhost", "root", "123456", "xfgl", 0, NULL, 0))) //中间分别是主机，用户名，密码，数据库名，端口号（可以写默认0或者3306等），可以先写成参数再传进去  
	{
		return false;
	}
	else
	{
		return true;
	}
	return true;
}

bool InitGradeList() {

	sprintf_s(AddQuery, "select * from gradelist"); //执行查询语句，这里是查询所有
	mysql_query(AddMysql, "set names utf8"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(AddMysql, AddQuery))    //执行SQL语句
	{
		return false;
	}

	//获取结果集  
	if (!(AddRes = mysql_store_result(AddMysql)))   //获得sql语句结束后返回的结果集  
	{
		return false;
	}

	for (int i = 0; AddColumn = mysql_fetch_row(AddRes); i++) {
		AddGradeList[i] = (AddColumn[0]);
	}
	return true;
}



