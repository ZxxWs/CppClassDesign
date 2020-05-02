#define _CRT_SECURE_NO_WARNINGS
#include "../Header/Search.h"
#include <QtWidgets/QMainWindow>
#include<iostream>
#include<Windows.h>
#include<WinSock.h>
#include<mysql.h>

using namespace std;

#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"wsock32.lib")

MYSQL* mysql = new MYSQL; //mysql连接  
MYSQL_RES* res; //这个结构代表返回行的一个查询结果集  
MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列  
char query[150]; //查询语句
string GradeList[9999];//班级列表
string NumList[100];//学号列表

string Tag;

bool InitGrade();
bool InitNum(string GradeNum);
bool ConnectDatabase();

Search::Search(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.GradeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(GradeComboBoxChanged()));//绑定控件和数据变化函数

	//setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);//将Search窗口放到Tz窗口中（绑定）

	//界面控件的绑定
	this->GradeComboBox = ui.GradeComboBox;
	this->NumComboBox = ui.NumComboBox;
	this->LabNum = ui.LabNum;
	this->LabName = ui.LabName;
	this->LabGrade = ui.LabGrade;
	this->LabScore = ui.LabScore;
	this->LabRemake = ui.LabRemark;
	this->ShowTable=ui.ShowTable;
	this->AlterButton = ui.AlterButton;
	this->OutButton = ui.OutButton;
 
	this->lab = ui.lable;
	if (ConnectDatabase()) {//连接验证
		if (InitGrade()) {//班级列表初始化验证

			int GradeListLen = GradeList->length();
			for (int i = 0; i < GradeListLen; i++) {//填充班级列表
				QString Item = QString::fromStdString(GradeList[i]);
				GradeComboBox->addItem(Item, Item);
			}
			if (InitNum(GradeList[0])) {

				int NumListLen = NumList->length();
				for (int i = 0; i < NumListLen; i++) {//填充班级列表
					QString Item = QString::fromStdString(NumList[i]);
					NumComboBox->addItem(Item, Item);
				}
				//lab->setText(QString::fromStdString(to_string(NumListLen)));
			}
		}
		else
		{
			//此处做连接失败处理
			//lab->setText("运行失败");
		}
	}
	else
	{
		//此处做连接失败处理
	}

}


void Search::GradeComboBoxChanged()//班号列表发生改变
{

	QString str = ui.GradeComboBox->currentText();//获取当前列表的值

	InitNum(str.toStdString());//用班号进行新的查询
	NumComboBox->clear();//先将上次的学号列表清空
	int NumListLen = NumList->length();
	for (int i = 0; i < NumListLen; i++) {//填充班级列表
		QString Item = QString::fromStdString(NumList[i]);
		NumComboBox->addItem(Item, Item);
	}
}



bool ConnectDatabase() {
	//初始化mysql  
	mysql_init(mysql);
	//返回false则连接失败，返回true则连接成功  
	if (!(mysql_real_connect(mysql, "localhost", "root", "123456", "xfgl", 0, NULL, 0))) //中间分别是主机，用户名，密码，数据库名，端口号（可以写默认0或者3306等），可以先写成参数再传进去  
	{
		return false;
	}
	else
	{
		return true;
	}
	return true;
}

bool InitGrade() {//初始化Grade列表，返回值是班号列表（int类型）

    sprintf_s(query, "select * from gradelist"); //执行查询语句，这里是查询所有
    mysql_query(mysql, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
    //返回0 查询成功，返回1查询失败  
    if (mysql_query(mysql, query))    //执行SQL语句
    {
        return false;
    }
 
    //获取结果集  
    if (!(res = mysql_store_result(mysql)))   //获得sql语句结束后返回的结果集  
    {
        return false;
    }

	for (int i = 0; column = mysql_fetch_row(res); i++) {
		GradeList[i] = (column[0]);
	}

	return true;
}

bool InitNum(string GradeNum) {//初始化Grade列表，返回值是班号列表（int类型）
	
	sprintf_s(query,"select * from grade"); //执行查询语句
	strcat(query, &GradeNum[0]);
	mysql_query(mysql, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(mysql, query))    //执行SQL语句
	{
		return false;
	}
	//获取结果集  
	if (!(res = mysql_store_result(mysql)))   //获得sql语句结束后返回的结果集  
	{
		return false;
	}

	for (int i = 0; column = mysql_fetch_row(res); i++) {
		NumList[i] = (column[0]);
	}
	return true;
}