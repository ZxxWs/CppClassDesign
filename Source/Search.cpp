#define _CRT_SECURE_NO_WARNINGS
#pragma execution_character_set("UTF-8")
#include "../Header/Search.h"
#include "../Header/Student.h"
#include <QtWidgets/QMainWindow>
#include<iostream>
#include<Windows.h>
#include<WinSock.h>
#include<mysql.h>

using namespace std;

#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"wsock32.lib")

MYSQL* mysql = new MYSQL; //mysql����  
MYSQL_RES* res; //����ṹ�������е�һ����ѯ�����  
MYSQL_ROW column; //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾ�����е���  
char query[150]; //��ѯ���
string GradeList[9999];//�༶�б�
string NumList[100];//ѧ���б�
Student stu;//չʾ��ѧ��

bool InitGrade();
bool InitNum(string GradeNum);
bool InitStudent(string GradeNum, string StudentNum);
bool ConnectDatabase();

Search::Search(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.GradeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(GradeComboBoxChanged()));//�󶨿ؼ������ݱ仯����
	connect(ui.NumComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(NumComboBoxChanged()));
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
 
	this->lab = ui.lable;
	if (ConnectDatabase()) {//������֤
		if (InitGrade()) {//�༶�б��ʼ����֤

			int GradeListLen = GradeList->length();
			for (int i = 0; i < GradeListLen; i++) {//���༶�б�
				QString Item = QString::fromStdString(GradeList[i]);
				GradeComboBox->addItem(Item, Item);
			}
			if (InitNum(GradeList[0])) {//ѧ���б��ʼ����֤
				int NumListLen = NumList->length();
				for (int i = 0; i < NumListLen; i++) {//���༶�б�
					QString Item = QString::fromStdString(NumList[i]);
					NumComboBox->addItem(Item, Item);
				}
				if (InitStudent(GradeList[0], NumList[0])) {
					LabNum->setText("ѧ�ţ�"+QString::fromStdString(stu.getSnum()));
					LabName->setText("������"+QString::fromStdString(stu.getSname()));
					LabScore->setText("�ܷ֣�"+QString::fromStdString(to_string(stu.getSscore())));
					LabRemake->setText("��ע��"+QString::fromStdString(stu.getSremark()));
					LabGrade->setText("�༶��"+QString::fromStdString(stu.getSgrade()));
				}
				else
				{
					//�˴�����ѯʧ�ܴ���
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
			//lab->setText("����ʧ��");
		}
	}
	else
	{
		//�˴�������ʧ�ܴ���
	}
}

void Search::GradeComboBoxChanged()//����б����ı�
{
	QString str = ui.GradeComboBox->currentText();//��ȡ��ǰ�б��ֵ

	InitNum(str.toStdString());//�ð�Ž����µĲ�ѯ
	NumComboBox->clear();//�Ƚ��ϴε�ѧ���б����
	int NumListLen = NumList->length();
	for (int i = 0; i < NumListLen; i++) {//���༶�б�
		QString Item = QString::fromStdString(NumList[i]);
		NumComboBox->addItem(Item, Item);
	}
}

void Search::NumComboBoxChanged()//����б����ı�
{

	QString GraNum = ui.GradeComboBox->currentText();
	QString StuNum = ui.NumComboBox->currentText();//��ȡ��ǰ�б��ֵ

	if (InitStudent(GraNum.toStdString(),StuNum.toStdString())) {
		LabNum->setText("ѧ�ţ�" + QString::fromStdString(stu.getSnum()));
		LabName->setText("������" + QString::fromStdString(stu.getSname()));
		LabScore->setText("�ܷ֣�" + QString::fromStdString(to_string(stu.getSscore())));
		LabRemake->setText("��ע��" + QString::fromStdString(stu.getSremark()));
		LabGrade->setText("�༶��" + QString::fromStdString(stu.getSgrade()));

	}
	else
	{
		//�˴�����ѯʧ�ܴ���
	}
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

	for (int i = 0; column = mysql_fetch_row(res); i++) {
		GradeList[i] = (column[0]);
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

	for (int i = 0; column = mysql_fetch_row(res); i++) {
		NumList[i] = (column[0]);
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

	for (int i = 0; column = mysql_fetch_row(res); i++) {
		stu.setSnum(column[0]);
		stu.setSname(column[1]);
		stu.setSgrade(column[2]);
		stu.setSscore(atof(column[3]));
		if (column[4]) {
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
		//
	}
	return true;
}
