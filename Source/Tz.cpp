#define _CRT_SECURE_NO_WARNINGS
#pragma execution_character_set("UTF-8")//用于qt的编码，如果没有，界面会有中文乱码
#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"wsock32.lib")
#include "../Header/Tz.h"
#include "../Header/Search.h"
#include "qpushbutton.h"
#include <QtWidgets/QMainWindow>
#include <iostream>
#include<Windows.h>
#include<WinSock.h>
#include<mysql.h>

using namespace std;

bool IsAlterTag = false;//标志现在是否在改密码
MYSQL* LogInMysql = new MYSQL; //mysql连接  
MYSQL_RES* LogInRes; //这个结构代表返回行的一个查询结果集  
MYSQL_ROW LogInColumn; //一个行数据的类型安全(type-safe)的表示，表示数据行的列  
char LogInQuery[100]; //查询语句

bool LogInConnectDatabase();
bool  FindPass(string pass);//通过输入的密码来查找密码。
bool AlterPassFun(string oldPass, string newPass);


Tz::Tz(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->PassLabel = ui.PassLabel;
	this->NewPassLabel = ui.NewPassLabel;
	this->SurePassLabel = ui.SurePassLabel;
	this->TagLabel = ui.TagLabel;
	this->PassLineEdit = ui.PassLineEdit;
	this->NewPassLineEdit = ui.NewPassLineEdit;
	this->SurePassLineEdit = ui.SurePassLineEdit;
	this->LogInButton = ui.LogInButton;
	this->AlterPassButton = ui.AlterPassButton;
	
	connect(ui.LogInButton, SIGNAL(clicked()), this, SLOT(ClickLogInButton()));//将按钮和点击事件绑定
	connect(ui.AlterPassButton, SIGNAL(clicked()), this, SLOT(ClickAlterPassButton()));//将按钮和点击事件绑定

	NewPassLabel->hide();
	NewPassLineEdit->hide();
	SurePassLabel->hide();
	SurePassLineEdit->hide();

	this->PassLineEdit->setClearButtonEnabled(true);//在输入框中设置一个清空按钮
	this->NewPassLineEdit->setClearButtonEnabled(true);//在输入框中设置一个清空按钮
	this->SurePassLineEdit->setClearButtonEnabled(true);//在输入框中设置一个清空按钮

	LogInButton->setStyleSheet("background:green");//设置按钮的背景颜色为绿色
	AlterPassButton->setStyleSheet("background: rgb(207, 207, 207)");
}

void Tz::ClickLogInButton() {//密码确认按钮点击事件的实现


	if (IsAlterTag) {//作为取消按钮使用

		IsAlterTag = false;
		TagLabel->clear();
		PassLabel->setText("输入密码：");
		AlterPassButton->setText("修改密码");
		NewPassLabel->hide();
		NewPassLineEdit->hide();
		SurePassLabel->hide();
		SurePassLineEdit->hide();
		LogInButton->show();
		PassLineEdit->clear();
		NewPassLineEdit->clear();
		SurePassLineEdit->clear();
		LogInButton->setStyleSheet("background:green");//设置按钮的背景颜色为绿色
		AlterPassButton->setStyleSheet("background: rgb(207, 207, 207)");
		LogInButton->setText("输入密码");
		AlterPassButton->setText("修改密码");
	}
	else//作为登陆按钮使用
	{
		TagLabel->setText("");
		PassLabel->setText("输入密码");
		NewPassLabel->hide();
		NewPassLineEdit->hide();
		SurePassLabel->hide();
		SurePassLineEdit->hide();
		string pass = PassLineEdit->displayText().toStdString();

		if (LogInConnectDatabase()) {
			if (FindPass(pass)) {//用输入的密码来查找密码是否存在

				TagLabel->setText("");
				PassLineEdit->clear();
				this->hide();
				//界面跳转函数
				Search* searchWin = new Search(this);
				connect(searchWin, SIGNAL(sendsignal()), this, SLOT(ReShowThis()));//当点击子界面OutButton，调用主界面的reshow()函数
				searchWin->show();
			}
			else
			{
				TagLabel->setText("密码输入错误");
			}
		}
	}
}

//修改密码按钮点击处理
void Tz::ClickAlterPassButton() {

	//第二次点击修改按钮后的处理逻辑
	if(IsAlterTag){

		TagLabel->setText("");
		string oldPass, newPass, surePass;
		oldPass = PassLineEdit->displayText().toStdString();
		newPass = NewPassLineEdit->displayText().toStdString();
		surePass = SurePassLineEdit->displayText().toStdString();

		if (newPass == surePass) {//两次输入的密码相同
			if (LogInConnectDatabase()) {//连接数据库
				if (FindPass(oldPass)) {//查找旧密码
					if (AlterPassFun(oldPass, newPass)) {//执行修改密码的数据库部分
						IsAlterTag = false;
						TagLabel->setText("密码修改成功");
						PassLabel->setText("输入密码：");
						AlterPassButton->setText("修改密码");
						LogInButton->setText("登陆");
						NewPassLabel->hide();
						NewPassLineEdit->hide();
						SurePassLabel->hide();
						SurePassLineEdit->hide();
						LogInButton->show();
						PassLineEdit->clear();
						NewPassLineEdit->clear();
						SurePassLineEdit->clear();
						LogInButton->setStyleSheet("background:green");//设置按钮的背景颜色为绿色
						AlterPassButton->setStyleSheet("background: rgb(207, 207, 207)");
					}
					else//执行修改密码函数失败
					{
						TagLabel->setText("密码修改失败，请重新尝试");
						IsAlterTag = true;
					}
				}
				else//旧密码查询失败
				{
					TagLabel->setText("旧密码输入错误，请重新确认");
					PassLineEdit->clear();
					IsAlterTag = true;
				}
			}
			else//数据库连接失败
			{
				TagLabel->setText("遇到未知错误，请重新尝试");
				IsAlterTag = true;
			}
		}
		else
		{
			TagLabel->setText("两次输入的密码不同，请重新确认");
			NewPassLineEdit->clear();
			SurePassLineEdit->clear();
			IsAlterTag = true;
		}
	}
	//第一次点击修改按钮的逻辑
	else
	{
		IsAlterTag = true;
		NewPassLabel->show();
		NewPassLineEdit->show();
		SurePassLabel->show();
		SurePassLineEdit->show();
		TagLabel->setText("");
		PassLabel->setText("输入旧密码：");
		AlterPassButton->setText("确认修改");
		LogInButton->setText("取消修改");
		LogInButton->setStyleSheet("background: rgb(207, 207, 207)");//设置按钮的背景颜色为白色
		AlterPassButton->setStyleSheet("background:green");
		PassLineEdit->clear();
	}

}

void Tz::ReShowThis(){//跳回到本界面
	this->show();
}

//连接数据库函数
bool LogInConnectDatabase() {
	//初始化mysql  
	mysql_init(LogInMysql);
	//返回false则连接失败，返回true则连接成功  
	if (!(mysql_real_connect(LogInMysql, "localhost", "root", "123456", "xfgl", 0, NULL, 0))) //中间分别是主机，用户名，密码，数据库名，端口号（可以写默认0或者3306等），可以先写成参数再传进去  
	{
		return false;
	}
	else
	{
		return true;
	}
	return true;
}

//使用输入的密码来查询密码是否存在
bool  FindPass(string pass) {

	string str = "select * from userpass where pass='"+pass+"'";
	sprintf_s(LogInQuery, &str[0]); //执行查询语句
	mysql_query(LogInMysql, "set names utf8"); //设置编码格式
	if (mysql_query(LogInMysql, LogInQuery))    //执行SQL语句
	{
		return false;
	}
	//获取结果集  
	if (!(LogInRes = mysql_store_result(LogInMysql)))   //获得sql语句结束后返回的结果集  
	{
		return false;
	}
	
	for (int i = 0; LogInColumn = mysql_fetch_row(LogInRes); i++) {
		if (pass == LogInColumn[0]) {
			return true;
		}
	}
	return false;
}

//操作数据库修改密码函数
bool AlterPassFun(string oldPass,string newPass) {

	string str = "update userpass set pass="+newPass+" where pass="+oldPass;
	sprintf_s(LogInQuery, &str[0]); //查询语句
	mysql_query(LogInMysql, "set names utf8");
	if (mysql_query(LogInMysql, LogInQuery))    //执行SQL语句
	{
		return false;
	}
	else
	{
		return true;
	}
}
