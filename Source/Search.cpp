#define _CRT_SECURE_NO_WARNINGS
#pragma execution_character_set("UTF-8")//����qt�ı��룬���û�У����������������
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

MYSQL* mysql = new MYSQL; //mysql����  
MYSQL_RES* res; //����ṹ�������е�һ����ѯ�����  
MYSQL_ROW column; //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾ�����е���  
char query[150]; //��ѯ���
string GradeList[9999];//�༶�б�
int GradeListLen = 0;
string NumList[100];//ѧ���б�
int NumListLen = 0;
Student stu;//չʾ��ѧ��

QStandardItemModel* dataModel = new QStandardItemModel();	//��������ģ��

//���溯��������
bool InitGrade();
bool InitNum(string GradeNum);
bool InitStudent(string GradeNum, string StudentNum);
bool ConnectDatabase();

Search::Search(QWidget* parent)//��ѯ����Ĺ��캯��
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.GradeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(GradeComboBoxChanged()));//�󶨿ؼ������ݱ仯����
	connect(ui.NumComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(NumComboBoxChanged()));
	connect(ui.AddButton, SIGNAL(clicked()), this, SLOT(ClickAddButton()));//��Ӱ�ť�͵�������İ�
	connect(ui.OutButton, SIGNAL(clicked()), this, SLOT(ClickOutButton()));//��
	//setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);//��Search���ڷŵ�Tz�����У��󶨣�

	//����ؼ��İ�
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

	this->ShowTable->setModel(dataModel);	//������ģ��
	this->ShowTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//���ñ�����Զ���
	
	if (ConnectDatabase()) {//������֤
		if (InitGrade()) {//�༶�б��ʼ����֤

			for (int i = 0; i <= GradeListLen; i++) {//���༶�б�
				QString Item = QString::fromStdString(GradeList[i]);
				GradeComboBox->addItem(Item, Item);
			}
		}
		else
		{
			//�˴�������ʧ�ܴ���
		}
	}
	else
	{
		//�˴�������ʧ�ܴ���
	}
}

void Search::GradeComboBoxChanged()//����б����ı�
{
	QString Qstr = ui.GradeComboBox->currentText();//��ȡ��ǰ�б��ֵ

	InitNum(Qstr.toStdString());//�ð�Ž����µĲ�ѯ
	NumComboBox->clear();//�Ƚ��ϴε�ѧ���б����
	for (int i = 0; i <= NumListLen; i++) {//���༶�б�
		QString Item = QString::fromStdString(NumList[i]);//��String����ת����Qstring����
		NumComboBox->addItem(Item, Item);
	}
}

void Search::NumComboBoxChanged()//ѧ�ź��б����ı�
{

	QString GraNum = ui.GradeComboBox->currentText();//��ȡ��ǰ�б��ֵ����һ��ͬ��
	QString StuNum = ui.NumComboBox->currentText();

	if (InitStudent(GraNum.toStdString(),StuNum.toStdString())) {

		LabNum->setText("ѧ�ţ�" + QString::fromStdString(stu.getSnum()));//����Ϣ��䵽������
		LabName->setText("������" + QString::fromStdString(stu.getSname()));
		LabScore->setText("�ܷ֣�" + QString::fromStdString(to_string(stu.getSscore())));
		LabRemake->setText("��ע��" + QString::fromStdString(stu.getSremark()));
		LabGrade->setText("�༶��" + QString::fromStdString(stu.getSgrade()));

		//ѧ��ϸ��������
		dataModel->clear();//�Ƚ�������
		dataModel->setHorizontalHeaderItem(0, new QStandardItem("��Ŀ"));//���ñ�ͷ
		dataModel->setHorizontalHeaderItem(1, new QStandardItem("ʱ��"));
		dataModel->setHorizontalHeaderItem(2, new QStandardItem("����"));
		for (int i = 0; i < 999; i++) {//ѭ�������
			if (stu.getSdetail()[i].getEvent() == "") {//��������յĻ���˵����ͷ�ˣ�������
				break;
			}
			dataModel->setItem(i, 0, new QStandardItem(QString::fromStdString(stu.getSdetail()[i].getEvent())));
			dataModel->setItem(i, 1, new QStandardItem(QString::fromStdString(stu.getSdetail()[i].getTime())));
			dataModel->setItem(i, 2, new QStandardItem(QString::fromStdString(to_string(stu.getSdetail()[i].getScore()))));
		}
		stu. ~Student();//�ͷ��ڴ棬������ͷŻᵽʱ�������ݼ̳��ϴε�
		stu=Student();
	}
	else
	{
		//�˴�����ѯʧ�ܴ���
	}
}

void Search::ClickAddButton() {//�������ӡ���ť�󣬴������Ϣ����

	AddInformation *a=new AddInformation();
	a->show();
}

void Search::ClickOutButton() {//�������ӡ���ť�󣬴������Ϣ����

	//ui.retranslateUi();
	LabRemake->setText("sdsdsdsd");
}

bool ConnectDatabase() {
	//��ʼ��mysql  
	mysql_init(mysql);
	//����false������ʧ�ܣ�����true�����ӳɹ�  
	if (!(mysql_real_connect(mysql, "localhost", "root", "123456", "xfgl", 0, NULL, 0))) //�м�ֱ����������û��������룬���ݿ������˿ںţ�����дĬ��0����3306�ȣ���������д�ɲ����ٴ���ȥ  
	{
		return false;
	}
	else
	{
		return true;
	}
	return true;
}

bool InitGrade() {//��ʼ��Grade�б�����ֵ�ǰ���б�int���ͣ�

    sprintf_s(query, "select * from gradelist"); //ִ�в�ѯ��䣬�����ǲ�ѯ����
    mysql_query(mysql, "set names utf8"); //���ñ����ʽ
    //����0 ��ѯ�ɹ�������1��ѯʧ��  

    if (mysql_query(mysql, query))    //ִ��SQL���
    {
        return false;
    }
 
    //��ȡ�����  
    if (!(res = mysql_store_result(mysql)))   //���sql�������󷵻صĽ����  
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

bool InitNum(string GradeNum) {//��ʼ��Grade�б�����ֵ�ǰ���б�int���ͣ�
	
	sprintf_s(query,"select * from grade"); //ִ�в�ѯ���
	strcat(query, &GradeNum[0]);
	mysql_query(mysql, "set names utf8"); //���ñ����ʽ��SET NAMES GBKҲ�У�������cmd����������  
	//����0 ��ѯ�ɹ�������1��ѯʧ��  
	if (mysql_query(mysql, query))    //ִ��SQL���
	{
		return false;
	}
	//��ȡ�����  
	if (!(res = mysql_store_result(mysql)))   //���sql�������󷵻صĽ����  
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
	sprintf_s(query,&Query[0]); //ִ�в�ѯ���
	mysql_query(mysql, "set names utf8"); //���ñ����ʽ��SET NAMES GBKҲ�У�������cmd����������  
	//����0 ��ѯ�ɹ�������1��ѯʧ��  
	if (mysql_query(mysql, query))    //ִ��SQL���
	{
		return false;
	}
	//��ȡ�����  
	if (!(res = mysql_store_result(mysql)))   //���sql�������󷵻صĽ����  
	{
		return false;
	}
	

	for (int i = 0; column = mysql_fetch_row(res); i++) {//����ѯ����ѧ����Ϣ��䵽stu��
		stu.setSnum(column[0]);
		stu.setSname(column[1]);
		stu.setSgrade(column[2]);
		stu.setSscore(atof(column[3]));
		if (column[4]) {//�ж�ϸ���Ƿ�Ϊ�գ�������жϻ������ֱ�ӿ�������ͬ��
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
			stu.setSremark("��");
		}
	}
	return true;
}
