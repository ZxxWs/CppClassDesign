#define _CRT_SECURE_NO_WARNINGS
#pragma execution_character_set("UTF-8")//����qt�ı��룬���û�У����������������
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

bool IsAlterTag = false;//��־�����Ƿ��ڸ�����
MYSQL* LogInMysql = new MYSQL; //mysql����  
MYSQL_RES* LogInRes; //����ṹ�������е�һ����ѯ�����  
MYSQL_ROW LogInColumn; //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾ�����е���  
char LogInQuery[100]; //��ѯ���

bool LogInConnectDatabase();
bool  FindPass(string pass);//ͨ��������������������롣
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
	
	connect(ui.LogInButton, SIGNAL(clicked()), this, SLOT(ClickLogInButton()));//����ť�͵���¼���
	connect(ui.AlterPassButton, SIGNAL(clicked()), this, SLOT(ClickAlterPassButton()));//����ť�͵���¼���

	NewPassLabel->hide();
	NewPassLineEdit->hide();
	SurePassLabel->hide();
	SurePassLineEdit->hide();

	this->PassLineEdit->setClearButtonEnabled(true);//�������������һ����հ�ť
	this->NewPassLineEdit->setClearButtonEnabled(true);//�������������һ����հ�ť
	this->SurePassLineEdit->setClearButtonEnabled(true);//�������������һ����հ�ť

	LogInButton->setStyleSheet("background:green");//���ð�ť�ı�����ɫΪ��ɫ
	AlterPassButton->setStyleSheet("background: rgb(207, 207, 207)");
}

void Tz::ClickLogInButton() {//����ȷ�ϰ�ť����¼���ʵ��


	if (IsAlterTag) {//��Ϊȡ����ťʹ��

		IsAlterTag = false;
		TagLabel->clear();
		PassLabel->setText("�������룺");
		AlterPassButton->setText("�޸�����");
		NewPassLabel->hide();
		NewPassLineEdit->hide();
		SurePassLabel->hide();
		SurePassLineEdit->hide();
		LogInButton->show();
		PassLineEdit->clear();
		NewPassLineEdit->clear();
		SurePassLineEdit->clear();
		LogInButton->setStyleSheet("background:green");//���ð�ť�ı�����ɫΪ��ɫ
		AlterPassButton->setStyleSheet("background: rgb(207, 207, 207)");
		LogInButton->setText("��������");
		AlterPassButton->setText("�޸�����");
	}
	else//��Ϊ��½��ťʹ��
	{
		TagLabel->setText("");
		PassLabel->setText("��������");
		NewPassLabel->hide();
		NewPassLineEdit->hide();
		SurePassLabel->hide();
		SurePassLineEdit->hide();
		string pass = PassLineEdit->displayText().toStdString();

		if (LogInConnectDatabase()) {
			if (FindPass(pass)) {//����������������������Ƿ����

				TagLabel->setText("");
				PassLineEdit->clear();
				this->hide();
				//������ת����
				Search* searchWin = new Search(this);
				connect(searchWin, SIGNAL(sendsignal()), this, SLOT(ReShowThis()));//������ӽ���OutButton�������������reshow()����
				searchWin->show();
			}
			else
			{
				TagLabel->setText("�����������");
			}
		}
	}
}

//�޸����밴ť�������
void Tz::ClickAlterPassButton() {

	//�ڶ��ε���޸İ�ť��Ĵ����߼�
	if(IsAlterTag){

		TagLabel->setText("");
		string oldPass, newPass, surePass;
		oldPass = PassLineEdit->displayText().toStdString();
		newPass = NewPassLineEdit->displayText().toStdString();
		surePass = SurePassLineEdit->displayText().toStdString();

		if (newPass == surePass) {//���������������ͬ
			if (LogInConnectDatabase()) {//�������ݿ�
				if (FindPass(oldPass)) {//���Ҿ�����
					if (AlterPassFun(oldPass, newPass)) {//ִ���޸���������ݿⲿ��
						IsAlterTag = false;
						TagLabel->setText("�����޸ĳɹ�");
						PassLabel->setText("�������룺");
						AlterPassButton->setText("�޸�����");
						LogInButton->setText("��½");
						NewPassLabel->hide();
						NewPassLineEdit->hide();
						SurePassLabel->hide();
						SurePassLineEdit->hide();
						LogInButton->show();
						PassLineEdit->clear();
						NewPassLineEdit->clear();
						SurePassLineEdit->clear();
						LogInButton->setStyleSheet("background:green");//���ð�ť�ı�����ɫΪ��ɫ
						AlterPassButton->setStyleSheet("background: rgb(207, 207, 207)");
					}
					else//ִ���޸����뺯��ʧ��
					{
						TagLabel->setText("�����޸�ʧ�ܣ������³���");
						IsAlterTag = true;
					}
				}
				else//�������ѯʧ��
				{
					TagLabel->setText("�������������������ȷ��");
					PassLineEdit->clear();
					IsAlterTag = true;
				}
			}
			else//���ݿ�����ʧ��
			{
				TagLabel->setText("����δ֪���������³���");
				IsAlterTag = true;
			}
		}
		else
		{
			TagLabel->setText("������������벻ͬ��������ȷ��");
			NewPassLineEdit->clear();
			SurePassLineEdit->clear();
			IsAlterTag = true;
		}
	}
	//��һ�ε���޸İ�ť���߼�
	else
	{
		IsAlterTag = true;
		NewPassLabel->show();
		NewPassLineEdit->show();
		SurePassLabel->show();
		SurePassLineEdit->show();
		TagLabel->setText("");
		PassLabel->setText("��������룺");
		AlterPassButton->setText("ȷ���޸�");
		LogInButton->setText("ȡ���޸�");
		LogInButton->setStyleSheet("background: rgb(207, 207, 207)");//���ð�ť�ı�����ɫΪ��ɫ
		AlterPassButton->setStyleSheet("background:green");
		PassLineEdit->clear();
	}

}

void Tz::ReShowThis(){//���ص�������
	this->show();
}

//�������ݿ⺯��
bool LogInConnectDatabase() {
	//��ʼ��mysql  
	mysql_init(LogInMysql);
	//����false������ʧ�ܣ�����true�����ӳɹ�  
	if (!(mysql_real_connect(LogInMysql, "localhost", "root", "123456", "xfgl", 0, NULL, 0))) //�м�ֱ����������û��������룬���ݿ������˿ںţ�����дĬ��0����3306�ȣ���������д�ɲ����ٴ���ȥ  
	{
		return false;
	}
	else
	{
		return true;
	}
	return true;
}

//ʹ���������������ѯ�����Ƿ����
bool  FindPass(string pass) {

	string str = "select * from userpass where pass='"+pass+"'";
	sprintf_s(LogInQuery, &str[0]); //ִ�в�ѯ���
	mysql_query(LogInMysql, "set names utf8"); //���ñ����ʽ
	if (mysql_query(LogInMysql, LogInQuery))    //ִ��SQL���
	{
		return false;
	}
	//��ȡ�����  
	if (!(LogInRes = mysql_store_result(LogInMysql)))   //���sql�������󷵻صĽ����  
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

//�������ݿ��޸����뺯��
bool AlterPassFun(string oldPass,string newPass) {

	string str = "update userpass set pass="+newPass+" where pass="+oldPass;
	sprintf_s(LogInQuery, &str[0]); //��ѯ���
	mysql_query(LogInMysql, "set names utf8");
	if (mysql_query(LogInMysql, LogInQuery))    //ִ��SQL���
	{
		return false;
	}
	else
	{
		return true;
	}
}
