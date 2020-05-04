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

int DelTag = 0;//ɾ�����ͱ�ǩ��0:ɾ���༶��1��ɾ��ѧ��

string DelStuList[100];
string DelGradeList[999];
int DelGradeListLen = 0;
int DelStuListLen = 0;
char DelQuery[150]; //��ѯ���

MYSQL* DelMysql = new MYSQL; //mysql����  
MYSQL_RES* DelRes; //����ṹ�������е�һ����ѯ�����  
MYSQL_ROW DelColumn; //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾ�����е���  

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
	connect(ui.DelGradeButton, SIGNAL(clicked()), this, SLOT(ClickGradeButton()));//����ť�͵���¼���
	connect(ui.DelStuButton, SIGNAL(clicked()), this, SLOT(ClickStuButton()));//����ť�͵���¼���
	connect(ui.SureButton, SIGNAL(clicked()), this, SLOT(ClickSureButton()));//����ť�͵���¼���
	connect(ui.AgainSureButton, SIGNAL(clicked()), this, SLOT(ClickAgainSureButton()));//����ť�͵���¼���
	connect(ui.CancelButton, SIGNAL(clicked()), this, SLOT(ClickCancelButton()));//����ť�͵���¼���
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

	//���޹صĿؼ�����
	this->StuLabel->hide();
	this->StuComboBox->hide();
	this->AgainSureButton->hide();
	this->CancelButton->hide();
	this->SureLabel->hide();
	this->DelGradeButton->setStyleSheet("background:green");//���ð�ť�ı�����ɫΪ��ɫ
	this->DelStuButton->setStyleSheet("background:white");//���ð�ť�ı�����ɫΪ��ɫ

	if (DelConnectDatabase()) {//�������ݿ⣬����������б�
		if (InitDelGradeList()) {//���а���б��ʼ������

			for (int i = 0; i <= DelGradeListLen; i++) {
				QString Item = QString::fromStdString(DelGradeList[i]);//��ÿ�����ת��Qstring���ͣ�ԭ����string����)
				GradeComboBox->addItem(Item,Item);
			}
		}
	}
}

void DelInformation::ClickGradeButton() {//�����ɾ���༶����ť����߼�

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

void DelInformation::ClickStuButton(){//�����ɾ��ѧ������ť����߼�
	this->DelGradeButton->setStyleSheet("background:white");//���ð�ť�ı�����ɫ
	this->DelStuButton->setStyleSheet("background:green");
	this->StuLabel->show();
	this->StuComboBox->show();
	this->SureButton->show();
	this->AgainSureButton->hide();
	this->CancelButton->hide();
	this->SureLabel->hide();
	this->TagLabel->setText("");
	DelTag = 1;//��ǩ��ʾɾ�����������͡�

	this->StuComboBox->clear();//����յ�ǰ���б������
	string gradeNum = this->GradeComboBox->currentText().toStdString();//��ȡ��ǰ����б��е�ֵ
	if (InitStuList(gradeNum)) {
		for (int i = 0; i <= DelStuListLen; i++) {
			QString Item = QString::fromStdString(DelStuList[i]);//��ÿ�����ת��string���ͣ�ԭ����Qstring����)
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
	if (DelTag == 0) {//��tagΪ0ʱ����ʾɾ���༶

		string grade = this->GradeComboBox->currentText().toStdString();//��ȡ��ǰ����б���ʾ��ֵ��Qstringתstring
		if (DelGradeFun(grade)) {
			this->TagLabel->setText("�༶ɾ���ɹ�");
		}
		else
		{
			this->TagLabel->setText("�༶ɾ��ʧ��");
		}
	}
	else//��DelTagΪ1ʱ����ʾɾ��ѧ����
	{
		string grade = this->GradeComboBox->currentText().toStdString();//��ȡ��ǰ����б���ʾ��ֵ��
		string stu = this->StuComboBox->currentText().toStdString();//��ȡ��ǰѧ���б��ֵ
		if (DelStuFun(grade,stu)) {
			this->TagLabel->setText("ѧ��ɾ���ɹ�");
		}
		else
		{
			this->TagLabel->setText("ѧ��ɾ��ʧ��");
		}
	}

	this->SureLabel->hide();
	this->CancelButton->hide();
	this->AgainSureButton->hide();


}

//���ȡ��ɾ����ť����
void DelInformation::ClickCancelButton(){
	this->AgainSureButton->hide();
	this->SureButton->show();
	this->SureLabel->hide();
	this->CancelButton->hide();
}

void DelInformation::GradeComboBoxChanged() {//������б����仯ʱ�����ء��ٴ�ȷ�ϡ���ť����ֹɾ���Ĺ����л��˰༶������ĺ���ͬ��
	this->AgainSureButton->hide();
	this->SureButton->show();
	this->SureLabel->hide();
	this->CancelButton->hide();
	this->TagLabel->setText("");


	this->StuComboBox->clear();//����յ�ǰ���б������
	string gradeNum = this->GradeComboBox->currentText().toStdString();//��ȡ��ǰ����б��е�ֵ
	if (InitStuList(gradeNum)) {
		
		for (int i = 0; i <= DelStuListLen; i++) {
			/*if (DelStuList[i] == "") {
				break;
			}*/
			QString Item = QString::fromStdString(DelStuList[i]);//��ÿ�����ת��string���ͣ�ԭ����Qstring����)
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
	//��ʼ��mysql  
	mysql_init(DelMysql);
	//����false������ʧ�ܣ�����true�����ӳɹ�  
	if (!(mysql_real_connect(DelMysql, "localhost", "root", "123456", "xfgl", 0, NULL, 0))) //�м�ֱ����������û��������룬���ݿ������˿ںţ�����дĬ��0����3306�ȣ���������д�ɲ����ٴ���ȥ  
	{
		return false;
	}
	else
	{
		return true;
	}
}

//��ѯ���а༶��
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
	DelGradeList->clear();
	for (int i = 0; DelColumn = mysql_fetch_row(DelRes); i++) {
		DelGradeList[i] = (DelColumn[0]);
		DelGradeListLen = i;
	}
	return true;
}

bool InitStuList(string gradeNum) {

	string s = "select Snum from grade"+gradeNum;
	sprintf_s(DelQuery,&s[0]); //��ѯ���
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
	DelStuList->clear();//���ǰ���������
	for (int i = 0; DelColumn = mysql_fetch_row(DelRes); i++) {
		DelStuList[i] = DelColumn[0];
		DelStuListLen = i;
	}
	return true;
}

bool DelGradeFun(string grade) {//ɾ���༶����

	string str = "delete from gradelist where grade='" + grade + "';";
	sprintf_s(DelQuery, &str[0]); //��ѯ���
	mysql_query(DelMysql, "set names utf8");
	if (mysql_query(DelMysql, DelQuery))    //ִ��SQL���
	{
		return false;
	}
	else
	{
		str = "drop table grade" + grade;
		sprintf_s(DelQuery, &str[0]); //��ѯ���
		if (mysql_query(DelMysql, DelQuery))    //ִ��SQL���
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
	sprintf_s(DelQuery,&str[0]); //��ѯ���
	mysql_query(DelMysql, "set names utf8");
	if (mysql_query(DelMysql, DelQuery))    //ִ��SQL���
	{
		return false;
	}
	else
	{
		return true;
	}
}