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

int DelTag = 0;//删除类型标签。0:删除班级，1：删除学生

string DelStuList[100];
string DelGradeList[999];
int DelGradeListLen = 0;
int DelStuListLen = 0;
char DelQuery[150]; //查询语句

MYSQL* DelMysql = new MYSQL; //mysql连接  
MYSQL_RES* DelRes; //这个结构代表返回行的一个查询结果集  
MYSQL_ROW DelColumn; //一个行数据的类型安全(type-safe)的表示，表示数据行的列  

string ttt;
int iii=-1;

bool  DelConnectDatabase();
bool InitDelGradeList();
bool InitStuList(string gradeNum);
bool DelGradeFun(string grade);
bool DelStuFun(string grade, string stu);

DelInformation::DelInformation(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.DelGradeButton, SIGNAL(clicked()), this, SLOT(ClickGradeButton()));//将按钮和点击事件绑定
	connect(ui.DelStuButton, SIGNAL(clicked()), this, SLOT(ClickStuButton()));//将按钮和点击事件绑定
	connect(ui.SureButton, SIGNAL(clicked()), this, SLOT(ClickSureButton()));//将按钮和点击事件绑定
	connect(ui.AgainSureButton, SIGNAL(clicked()), this, SLOT(ClickAgainSureButton()));//将按钮和点击事件绑定
	connect(ui.CancelButton, SIGNAL(clicked()), this, SLOT(ClickCancelButton()));//将按钮和点击事件绑定
	connect(ui.GradeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(GradeComboBoxChanged()));
	connect(ui.StucomboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(StuComboBoxChanged()));

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
	this->AgainSureButton->hide();
	this->CancelButton->hide();
	this->SureLabel->hide();
	this->DelGradeButton->setStyleSheet("background:green");//设置按钮的背景颜色为绿色
	this->DelStuButton->setStyleSheet("background:white");//设置按钮的背景颜色为白色

	if (DelConnectDatabase()) {//连接数据库，并且填充班号列表
		if (InitDelGradeList()) {//运行班号列表初始化函数

			for (int i = 0; i <= DelGradeListLen; i++) {
				QString Item = QString::fromStdString(DelGradeList[i]);//将每个班号转换Qstring类型（原来是string类型)
				GradeComboBox->addItem(Item,Item);
			}
		}
	}
}

void DelInformation::ClickGradeButton() {//点击“删除班级”按钮后的逻辑

	this->DelGradeButton->setStyleSheet("background:green");
	this->DelStuButton->setStyleSheet("background:white");
	this->StuLabel->hide();
	this->StuComboBox->hide();
	this->AgainSureButton->hide();
	this->CancelButton->hide();
	this->SureLabel->hide();
	this->TagLabel->setText("");
	DelTag = 0;
	
	GradeComboBox->clear();
	if (InitDelGradeList()) {
		for (int i = 0; i <= DelGradeListLen; i++) {
			QString Item = QString::fromStdString(DelGradeList[i]);
			GradeComboBox->addItem(Item, Item);
		}
	}
}

void DelInformation::ClickStuButton(){//点击“删除学生”按钮后的逻辑
	this->DelGradeButton->setStyleSheet("background:white");//设置按钮的背景颜色
	this->DelStuButton->setStyleSheet("background:green");
	this->StuLabel->show();
	this->StuComboBox->show();
	this->SureButton->show();
	this->AgainSureButton->hide();
	this->CancelButton->hide();
	this->SureLabel->hide();
	this->TagLabel->setText("");
	DelTag = 1;//标签表示删除的数据类型。

	this->StuComboBox->clear();//先清空当前的列表再填充
	string gradeNum = this->GradeComboBox->currentText().toStdString();//获取当前班号列表中的值
	if (InitStuList(gradeNum)) {
		for (int i = 0; i <= DelStuListLen; i++) {
			QString Item = QString::fromStdString(DelStuList[i]);//将每个班号转换string类型（原来是Qstring类型)
			StuComboBox->addItem(Item, Item);
		}
	}

}

void DelInformation::ClickSureButton(){
	this->SureButton->hide();
	this->SureLabel->show();
	this->AgainSureButton->show();
	this->CancelButton->show();
}

void DelInformation::ClickAgainSureButton() {
	if (DelTag == 0) {//当tag为0时，表示删除班级

		string grade = this->GradeComboBox->currentText().toStdString();//获取当前班号列表显示的值。Qstring转string
		if (DelGradeFun(grade)) {
			this->TagLabel->setText("班级删除成功");
		}
		else
		{
			this->TagLabel->setText("班级删除失败");
		}
	}
	else//当DelTag为1时，表示删除学生。
	{
		string grade = this->GradeComboBox->currentText().toStdString();//获取当前班号列表显示的值。
		string stu = this->StuComboBox->currentText().toStdString();//获取当前学号列表的值
		if (DelStuFun(grade,stu)) {
			this->TagLabel->setText("学生删除成功");
		}
		else
		{
			this->TagLabel->setText("学生删除失败");
		}
	}

	this->SureLabel->hide();
	this->CancelButton->hide();
	this->AgainSureButton->hide();


}

//点击取消删除按钮事务
void DelInformation::ClickCancelButton(){
	this->AgainSureButton->hide();
	this->SureButton->show();
	this->SureLabel->hide();
	this->CancelButton->hide();
}

void DelInformation::GradeComboBoxChanged() {//当班号列表发生变化时，隐藏“再次确认”按钮，防止删除的过程中换了班级。下面的函数同理。
	this->AgainSureButton->hide();
	this->SureButton->show();
	this->SureLabel->hide();
	this->CancelButton->hide();
	this->TagLabel->setText("");


	this->StuComboBox->clear();//先清空当前的列表再填充
	string gradeNum = this->GradeComboBox->currentText().toStdString();//获取当前班号列表中的值
	if (InitStuList(gradeNum)) {
		
		for (int i = 0; i <= DelStuListLen; i++) {
			/*if (DelStuList[i] == "") {
				break;
			}*/
			QString Item = QString::fromStdString(DelStuList[i]);//将每个班号转换string类型（原来是Qstring类型)
			StuComboBox->addItem(Item, Item);
		}
		//this->TagLabel->setText(QString::fromStdString(to_string(iii)));

		this->TagLabel->setText(QString::fromStdString(ttt));
	}
}

void DelInformation::StuComboBoxChanged() {
	this->AgainSureButton->hide();
	this->SureButton->show();
	this->SureLabel->hide();
	this->CancelButton->hide();
	//this->TagLabel->setText("");
}

bool DelConnectDatabase() {
	//初始化mysql  
	mysql_init(DelMysql);
	//返回false则连接失败，返回true则连接成功  
	if (!(mysql_real_connect(DelMysql, "localhost", "root", "123456", "xfgl", 0, NULL, 0))) //中间分别是主机，用户名，密码，数据库名，端口号（可以写默认0或者3306等），可以先写成参数再传进去  
	{
		return false;
	}
	else
	{
		return true;
	}
}

//查询所有班级号
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
	DelGradeList->clear();
	for (int i = 0; DelColumn = mysql_fetch_row(DelRes); i++) {
		DelGradeList[i] = (DelColumn[0]);
		DelGradeListLen = i;
	}
	return true;
}

bool InitStuList(string gradeNum) {

	string s = "select Snum from grade"+gradeNum;
	sprintf_s(DelQuery,&s[0]); //查询语句
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
	DelStuList->clear();//填充前先清空数组
	for (int i = 0; DelColumn = mysql_fetch_row(DelRes); i++) {
		DelStuList[i] = DelColumn[0];
		DelStuListLen = i;
	}
	return true;
}

bool DelGradeFun(string grade) {//删除班级函数

	string str = "delete from gradelist where grade='" + grade + "';";
	sprintf_s(DelQuery, &str[0]); //查询语句
	mysql_query(DelMysql, "set names utf8");
	if (mysql_query(DelMysql, DelQuery))    //执行SQL语句
	{
		return false;
	}
	else
	{
		str = "drop table grade" + grade;
		sprintf_s(DelQuery, &str[0]); //查询语句
		if (mysql_query(DelMysql, DelQuery))    //执行SQL语句
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

bool DelStuFun(string grade,string stu) {

	string str = "delete from grade"+grade+" where Snum='"+stu+"';";
	sprintf_s(DelQuery,&str[0]); //查询语句
	mysql_query(DelMysql, "set names utf8");
	if (mysql_query(DelMysql, DelQuery))    //执行SQL语句
	{
		return false;
	}
	else
	{
		return true;
	}
}