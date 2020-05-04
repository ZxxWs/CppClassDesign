#define _CRT_SECURE_NO_WARNINGS
#pragma execution_character_set("UTF-8")//用于qt的编码，如果没有，界面会有中文乱码
#include "../Header/Detail.h"
#include "../Header/Search.h"
#include "../Header/Student.h"
#include "../Header/AddInformation.h"
#include <QtWidgets/QMainWindow>
#include<iostream>
#include<Windows.h>
#include<WinSock.h>
#include<mysql.h>
#include <QHeaderView>
#include <qstandarditemmodel.h>


using namespace std;

#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"wsock32.lib")

MYSQL* mysql = new MYSQL; //mysql连接  
MYSQL_RES* res; //这个结构代表返回行的一个查询结果集  
MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列  
char query[150]; //查询语句
string GradeList[9999];//班级列表
int GradeListLen = 0;
string NumList[100];//学号列表
int NumListLen = 0;
Student stu;//展示的学生

QStandardItemModel* dataModel = new QStandardItemModel();	//表格绑定数据模型

//下面函数的声明
bool InitGrade();
bool InitNum(string GradeNum);
bool InitStudent(string GradeNum, string StudentNum);
bool ConnectDatabase();

Search::Search(QWidget* parent)//查询界面的构造函数
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.GradeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(GradeComboBoxChanged()));//绑定控件和数据变化函数
	connect(ui.NumComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(NumComboBoxChanged()));
	connect(ui.AddButton, SIGNAL(clicked()), this, SLOT(ClickAddButton()));//添加按钮和点击函数的绑定
	connect(ui.OutButton, SIGNAL(clicked()), this, SLOT(ClickOutButton()));//添
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

	this->ShowTable->setModel(dataModel);	//绑定数据模型
	this->ShowTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//设置表格宽度自动化
	
	if (ConnectDatabase()) {//连接验证
		if (InitGrade()) {//班级列表初始化验证

			for (int i = 0; i <= GradeListLen; i++) {//填充班级列表
				QString Item = QString::fromStdString(GradeList[i]);
				GradeComboBox->addItem(Item, Item);
			}
		}
		else
		{
			//此处做连接失败处理
		}
	}
	else
	{
		//此处做连接失败处理
	}
}

void Search::GradeComboBoxChanged()//班号列表发生改变
{
	QString Qstr = ui.GradeComboBox->currentText();//获取当前列表的值

	InitNum(Qstr.toStdString());//用班号进行新的查询
	NumComboBox->clear();//先将上次的学号列表清空
	for (int i = 0; i <= NumListLen; i++) {//填充班级列表
		QString Item = QString::fromStdString(NumList[i]);//将String类型转换成Qstring类型
		NumComboBox->addItem(Item, Item);
	}
}

void Search::NumComboBoxChanged()//学号号列表发生改变
{

	QString GraNum = ui.GradeComboBox->currentText();//获取当前列表的值，下一行同理。
	QString StuNum = ui.NumComboBox->currentText();

	if (InitStudent(GraNum.toStdString(),StuNum.toStdString())) {

		LabNum->setText("学号：" + QString::fromStdString(stu.getSnum()));//将信息填充到界面上
		LabName->setText("姓名：" + QString::fromStdString(stu.getSname()));
		LabScore->setText("总分：" + QString::fromStdString(to_string(stu.getSscore())));
		LabRemake->setText("备注：" + QString::fromStdString(stu.getSremark()));
		LabGrade->setText("班级：" + QString::fromStdString(stu.getSgrade()));

		//学分细则表格的填充
		dataModel->clear();//先将表格清空
		dataModel->setHorizontalHeaderItem(0, new QStandardItem("项目"));//设置表头
		dataModel->setHorizontalHeaderItem(1, new QStandardItem("时间"));
		dataModel->setHorizontalHeaderItem(2, new QStandardItem("分数"));
		for (int i = 0; i < 999; i++) {//循环填充表格
			if (stu.getSdetail()[i].getEvent() == "") {//如果遇到空的话就说明到头了（结束）
				break;
			}
			dataModel->setItem(i, 0, new QStandardItem(QString::fromStdString(stu.getSdetail()[i].getEvent())));
			dataModel->setItem(i, 1, new QStandardItem(QString::fromStdString(stu.getSdetail()[i].getTime())));
			dataModel->setItem(i, 2, new QStandardItem(QString::fromStdString(to_string(stu.getSdetail()[i].getScore()))));
		}
		stu. ~Student();//释放内存，如果不释放会到时表格的内容继承上次的
		stu=Student();
	}
	else
	{
		//此处做查询失败处理
	}
}

void Search::ClickAddButton() {//点击“添加”按钮后，打开添加信息界面

	AddInformation *a=new AddInformation();
	a->show();
}

void Search::ClickOutButton() {//点击“添加”按钮后，打开添加信息界面

	//ui.retranslateUi();
	LabRemake->setText("sdsdsdsd");
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
    mysql_query(mysql, "set names utf8"); //设置编码格式
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

	GradeList->clear();
	for (int i = 0; column = mysql_fetch_row(res); i++) {
		GradeList[i] = (column[0]);
		GradeListLen = i;
	}
	return true;
}

bool InitNum(string GradeNum) {//初始化Grade列表，返回值是班号列表（int类型）
	
	sprintf_s(query,"select * from grade"); //执行查询语句
	strcat(query, &GradeNum[0]);
	mysql_query(mysql, "set names utf8"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
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
	NumList->clear();
	for (int i = 0; column = mysql_fetch_row(res); i++) {
		NumList[i] = (column[0]);
		NumListLen = i;
	}
	return true;
}

bool InitStudent(string GradeNum,string StudentNum) {

	string Query = "select * from grade" + GradeNum + " where Snum=" + StudentNum;
	sprintf_s(query,&Query[0]); //执行查询语句
	mysql_query(mysql, "set names utf8"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
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
	

	for (int i = 0; column = mysql_fetch_row(res); i++) {//将查询到的学生信息填充到stu中
		stu.setSnum(column[0]);
		stu.setSname(column[1]);
		stu.setSgrade(column[2]);
		stu.setSscore(atof(column[3]));
		if (column[4]) {//判断细则是否为空，如果不判断话程序会直接卡死（下同）
			stu.setSdetail(column[4]);
		}
		else
		{
			stu.setSdetail("");
		}
		if (column[5]) {
			stu.setSremark(column[5]);
		}
		else
		{
			stu.setSremark("无");
		}
	}
	return true;
}
