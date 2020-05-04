#include "../Header/DelInformation.h"
#include "qpushbutton.h"
#include <QtWidgets/QMainWindow>
#include <iostream>
#include<Windows.h>
#include<WinSock.h>
#include<mysql.h>

using namespace std;

#pragma execution_character_set("UTF-8")//����qt�ı��룬���û�У����������������
#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"wsock32.lib")

MYSQL* DelMysql = new MYSQL; //mysql����  
MYSQL_RES* DelRes; //����ṹ�������е�һ����ѯ�����  
MYSQL_ROW DelColumn; //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾ�����е���  
char DelQuery[150]; //��ѯ���
string DelGradeList[999];
string DelStuList[999];

bool InitDelGradeList();
bool InitStuList(string gradeNum);

DelInformation::DelInformation(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.DelGradeButton, SIGNAL(clicked()), this, SLOT(ClickGradeButton()));//����ť�͵���¼���
	connect(ui.DelStuButton, SIGNAL(clicked()), this, SLOT(ClickStuButton()));//����ť�͵���¼���
	connect(ui.SureButton, SIGNAL(clicked()), this, SLOT(ClickSureButton()));//����ť�͵���¼���
	connect(ui.AgainSureButton, SIGNAL(clicked()), this, SLOT(ClickAgainSureButton()));//����ť�͵���¼���
	connect(ui.CancelButton, SIGNAL(clicked()), this, SLOT(ClickCancelButton()));//����ť�͵���¼���

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

	//���޹صĿؼ�����
	this->StuLabel->hide();
	this->StuComboBox->hide();
	this->SureLabel->hide();
	this->AgainSureButton->hide();
	this->CancelButton->hide();
	this->SureButton->hide();
	this->DelGradeButton->setStyleSheet("background:green");//���ð�ť�ı�����ɫΪ��ɫ
	this->DelStuButton->setStyleSheet("background:white");//���ð�ť�ı�����ɫΪ��ɫ

	if (InitDelGradeList()) {
		/*for (int i = 0; i < DelGradeList->length(); i++) {
			QString Item = QString::fromStdString(DelGradeList[i]);
			GradeComboBox->addItem(Item, Item);
		}*/
	}
}

void DelInformation::ClickGradeButton() {//�����ɾ���༶����ť����߼�

	this->DelGradeButton->setStyleSheet("background:green");
	this->DelStuButton->setStyleSheet("background:white");
	this->SureButton->show();
	this->StuLabel->hide();
	this->StuComboBox->hide();
	
}
void DelInformation::ClickStuButton(){//�����ɾ��ѧ������ť����߼�
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
	sprintf_s(DelQuery, "select grade from gradelist"); //��ѯ���
	mysql_query(DelMysql, "set names utf8"); 
	if (mysql_query(DelMysql, DelQuery))    //ִ��SQL���
	{
		return false;
	}
	//��ȡ�����  
	if (!(DelRes = mysql_store_result(DelMysql)))   //���sql�������󷵻صĽ����  
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

