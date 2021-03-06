#define _CRT_SECURE_NO_WARNINGS
#pragma execution_character_set("UTF-8")//用于qt的编码，如果没有，界面会有中文乱码
#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"wsock32.lib")
#include "../Header/Detail.h"
#include "../Header/Search.h"
#include "../Header/Student.h"
#include "../Header/AddInformation.h"
#include "../Header/DelInformation.h"
#include "../Header/AlterDetailUI.h"
#include <QtWidgets/QMainWindow>
#include<iostream>
#include<Windows.h>
#include<WinSock.h>
#include<mysql.h>
#include <QHeaderView>
#include <qstandarditemmodel.h>

//…………………………………………190行需要优化………………………………

using namespace std;

MYSQL* mysql = new MYSQL; //mysql连接  
MYSQL_RES* res; //这个结构代表返回行的一个查询结果集  
MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列  
char query[5000]; //查询语句
string GradeList[9999];//班级列表
int GradeListLen = 0;
string NumList[100];//学号列表
int NumListLen = 0;
Student stu;//展示的学生
int AlterTag=0;//修改按钮的状态。0：不可修改；1：可修改
int RowCount = 0;//表格的行数
QStandardItemModel* dataModel = new QStandardItemModel();	//表格绑定数据模型

//下面函数的声明
bool ConnectDatabase();
bool InitGrade();
bool InitNum(string GradeNum);
bool InitStudent(string GradeNum, string StudentNum);
bool AlterDetaile(string gradeNum, string stuNum, string AllDetail, double AllScore);
string ToString(double d, int i);
int CheckInPut(string event, string time, string score);

Search::Search(QWidget* parent)//查询界面的构造函数
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.GradeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(GradeComboBoxChanged()));//绑定控件和数据变化函数
	connect(ui.NumComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(NumComboBoxChanged()));
	connect(ui.AddButton, SIGNAL(clicked()), this, SLOT(ClickAddButton()));//添加按钮和点击函数的绑定
	connect(ui.DelButton, SIGNAL(clicked()), this, SLOT(ClickDelButton()));
	connect(ui.AlterButton, SIGNAL(clicked()), this, SLOT(ClickAlterButton()));
	connect(ui.OutButton, SIGNAL(clicked()), this, SLOT(ClickOutButton()));

	//界面控件的绑定
	this->GradeComboBox = ui.GradeComboBox;
	this->NumComboBox = ui.NumComboBox;
	this->LabNum = ui.LabNum;
	this->LabName = ui.LabName;
	this->LabGrade = ui.LabGrade;
	this->LabScore = ui.LabScore;
	this->LabRemake = ui.LabRemark;
	this->ShowTable=ui.ShowTable;
	this->AddButton = ui.AddButton;
	this->DelButton = ui.DelButton;
	this->AlterButton = ui.AlterButton;
	this->OutButton = ui.OutButton;

	this->ShowTable->setModel(dataModel);	//绑定数据模型
	this->ShowTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//设置表格宽度自动化
	this->ShowTable->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置表格为不可编辑状态
	
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

//班号列表变化事件处理
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

//学号列表事变化件处理
void Search::NumComboBoxChanged()//学号号列表发生改变
{
	//LabAlter->setText("");//这个label是修改学生学分细则的 提示label
	QString GraNum = ui.GradeComboBox->currentText();//获取当前列表的值，下一行同理。
	QString StuNum = ui.NumComboBox->currentText();

	if (InitStudent(GraNum.toStdString(),StuNum.toStdString())) {

		LabNum->setText("学号：" + QString::fromStdString(stu.getSnum()));//将信息填充到界面上
		LabName->setText("姓名：" + QString::fromStdString(stu.getSname()));
		LabScore->setText("总分：" + QString::fromStdString(ToString(stu.getSscore(),1)));
		LabRemake->setText("备注：" + QString::fromStdString(stu.getSremark()));
		LabGrade->setText("班级：" + QString::fromStdString(stu.getSgrade()));

		//学分细则表格的填充
		dataModel->clear();//先将表格清空
		RowCount = 0;
		dataModel->setHorizontalHeaderItem(0, new QStandardItem("项目"));//设置表头
		dataModel->setHorizontalHeaderItem(1, new QStandardItem("时间"));
		dataModel->setHorizontalHeaderItem(2, new QStandardItem("分数"));
		for (int i = 0; i < 999; i++) {//循环填充表格
			if (stu.getSdetail()[i].getEvent() == "") {//如果遇到空的话就说明到头了（结束）
				break;
			}
			dataModel->setItem(i, 0, new QStandardItem(QString::fromStdString(stu.getSdetail()[i].getEvent())));
			dataModel->setItem(i, 1, new QStandardItem(QString::fromStdString(stu.getSdetail()[i].getTime())));
			dataModel->setItem(i, 2, new QStandardItem(QString::fromStdString(ToString(stu.getSdetail()[i].getScore(),1))));
			RowCount = i+1;
		}
		stu. ~Student();//释放内存，如果不释放会到时表格的内容继承上次的
		stu=Student();
	}
	else
	{
		//此处做查询失败处理
	}
}

//添加按钮事件处理
void Search::ClickAddButton() {//点击“添加”按钮后，打开添加信息界面

	AddInformation *addWin=new AddInformation(this);
	connect(addWin, SIGNAL(sendsignal()), this, SLOT(ReShowWin()));//当点击子界面OutButton，调用
	addWin->setWindowModality(Qt::ApplicationModal);
	addWin->show();
}

//删除按钮事件处理
void Search::ClickDelButton(){

	DelInformation* delWin = new DelInformation(this);
	connect(delWin, SIGNAL(sendsignal()), this, SLOT(ReShowWin()));//当点击子界面OutButton，调用
	delWin->setWindowModality(Qt::ApplicationModal);
	delWin->show();
}

//修改按钮事件处理
void Search::ClickAlterButton(){
	
	if (AlterTag == 0) {//在不可修改状态下点击
		AlterTag = 1;
		//先把其他操作控件隐藏
		this->GradeComboBox->hide();
		this->NumComboBox->hide();
		this->AddButton->hide();
		this->DelButton->hide();
		this->AlterButton->setStyleSheet("background: red");
		this->OutButton->setStyleSheet("background: green");
		this->OutButton->setText("取消");

		this->ShowTable->setEditTriggers(QAbstractItemView::AllEditTriggers);//表格设置为可编辑状态
		for (int i = 0; i < 8; i++) {//为表格增加9行空行
			RowCount++;
			dataModel->setItem(RowCount, 0, new QStandardItem(QString::fromStdString("")));
			dataModel->setItem(RowCount, 1, new QStandardItem(QString::fromStdString("")));
			dataModel->setItem(RowCount, 2, new QStandardItem(QString::fromStdString("")));
		}
	}
	else
	{
		AlterTag = 0;
		//将需要显示出来的控件显示出来
		this->GradeComboBox->show();
		this->NumComboBox->show();
		this->AddButton->show();
		this->DelButton->show();
		this->AlterButton->setStyleSheet("background: rgb(207, 207, 207)");
		this->OutButton->setStyleSheet("background: rgb(207, 207, 207)");
		this->OutButton->setText("退出");

		string AllDetail = "";
		double AllScore = 0;
		string event, time, score;

		for (int i = 0; i <= RowCount; i++) {//循环读取表格内容

			//读取每个位置的内容
			event=dataModel->data(dataModel->index(i,0)).toString().toStdString();
			time = dataModel->data(dataModel->index(i, 1)).toString().toStdString();
			score = dataModel->data(dataModel->index(i, 2)).toString().toStdString();
			//检测输入内容由没有空的
			if ((event=="" || time=="")|| score=="") {//如果有空是空的,直接跳过。
				continue;
			}

			//进行输入异常检测:	
			int checkTag = CheckInPut(event, time, score);
			if (checkTag != 0) {
				AlterDetailUI* AU = new AlterDetailUI(checkTag);
				connect(AU, SIGNAL(sendsignal()), this, SLOT(ReShowWin()));//当点击子界面OutButton，调用	
				AU->setWindowModality(Qt::ApplicationModal);
				AU->show();
				return;
			}

			AllDetail = AllDetail + "#" + event + "#" + time + "#" + score + "%%";//这里设置两个%是为了防止转义
			AllScore += atof(&score[0]);//将string转为浮点类型
		}

		//获取当前班号和学号
		string gradeNum = GradeComboBox->currentText().toStdString();
		string stuNum = NumComboBox->currentText().toStdString();
		
		//处理数据库，修改学分细则
		int AlterSuccee;
		if (AlterDetaile(gradeNum, stuNum, AllDetail, AllScore)) {//执行细则修改函数
			AlterSuccee = 0;
		}
		else
		{
			AlterSuccee = 1;
		}
		AlterDetailUI* AlterWin = new AlterDetailUI(AlterSuccee,this);//new一个提示界面，第一个参数是传给下一界面的数据，第二个参数是指在当前界面上启动
		connect(AlterWin, SIGNAL(sendsignal()), this, SLOT(ReShowWin()));//当子界面传回信息后，调用ReShowWin
		AlterWin->setWindowModality(Qt::ApplicationModal);//将子界面设置为最上层
		AlterWin->show();
	}
}

//退出按钮事件处理
void Search::ClickOutButton() {

	if (AlterTag == 1) {//当学分细则正在修改的时候（作为取消按钮使用）
		AlterTag = 0;
		this->GradeComboBox->show();
		this->NumComboBox->show();
		this->AddButton->show();
		this->DelButton->show();
		this->AlterButton->setStyleSheet("background: rgb(207, 207, 207)");
		this->OutButton->setStyleSheet("background: rgb(207, 207, 207)");
		this->OutButton->setText("退出");
		NumComboBoxChanged();//执行一次学号列表改变函数，重置学分细则
		this->ShowTable->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置表格为不可编辑状态
	}
	else//当学分细则没在修改状态，作为退出按钮使用
	{
		emit sendsignal();
		this->close();
	}

}

//从添加、删除界面返回后的操作逻辑
void Search::ReShowWin() {

	this->ShowTable->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置细节表格不可修改

	GradeComboBoxChanged();//刷新一下当前界面
	NumComboBoxChanged();
}

//连接数据库函数
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

//班号查询函数
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

//学号查询函数
bool InitNum(string GradeNum) {//初始化Grade列表，返回值是班号列表（int类型）
	
	sprintf_s(query,"select * from grade"); //执行查询语句
	strcat(query, &GradeNum[0]);
	mysql_query(mysql, "set names utf8"); //设置编码格式（
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

//学生查询函数
bool InitStudent(string GradeNum,string StudentNum) {

	string Query = "select * from grade" + GradeNum + " where Snum=" + StudentNum;
	sprintf_s(query,&Query[0]); //执行查询语句
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

//修改学生学分细则
bool AlterDetaile(string gradeNum, string stuNum, string AllDetail, double AllScore) {

	
	string str = "update grade"+gradeNum+" set Sscore="+ToString(AllScore,1)+",Sdetail='"+AllDetail+"' where Snum="+stuNum;

	sprintf_s(query, &str[0]); //查询语句
	mysql_query(mysql, "set names utf8");
	if (mysql_query(mysql, query))    //执行SQL语句
	{
		return false;
	}
	else
	{
		return true;
	}
}

//自定义用于将double转成string，保留i位小数
string ToString(double d, int i) {
	string str = to_string(d);
	int tag = str.find(".");
	return str.substr(0, tag + 1 + i);
}


//用于检测输入的数据是否合规。	
//返回0：合格，11：输入有#或者%，12：时间格式有问题，13：分数格式有问题（12未用）	
int CheckInPut(string event, string time, string score) {

	if ((event.find("#") == event.npos) == 0 || (event.find("%%") == event.npos) == 0) {//检测出输入有#或者%
		return 11;
	}

	if ((time.find("#") == time.npos) == 0 || (time.find("%%") == time.npos) == 0) {//检测出输入有#或者%	
		return 11;
	}

	int DotCount = 0;
	for (int i = 0; i < score.length(); i++) {
		char strScore = score[i];
		if (strScore >= '0' && strScore <= '9') {
			continue;
		}
		else if (strScore == '.')
		{
			DotCount += 1;
			if (DotCount == 2) {
				return 13;
			}
		}
		else
		{
			return 13;
		}
	}
	return 0;
}


