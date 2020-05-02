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

MYSQL* mysql = new MYSQL; //mysql����  
MYSQL_RES* res; //����ṹ�������е�һ����ѯ�����  
MYSQL_ROW column; //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾ�����е���  
char query[150]; //��ѯ���
string GradeList[9999];//�༶�б�

bool InitGrade();
bool ConnectDatabase();

Search::Search(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);//��Search���ڷŵ�Tz�����У��󶨣�

	//����ؼ��İ�
	this->GradeList = ui.GradeList;
	this->NumList = ui.NumList;
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
		if (InitGrade) {//�༶�б��ʼ����֤

		}

		//QString qstr = QString::fromStdString(InitGrade());
		//lab->setText(qstr);
	}
	else
	{
		//�˴�������ʧ�ܴ���
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
    mysql_query(mysql, "set names gbk"); //���ñ����ʽ��SET NAMES GBKҲ�У�������cmd����������  
    //����0 ��ѯ�ɹ�������1��ѯʧ��  
    if (mysql_query(mysql, query))    //ִ��SQL���
    {
        //printf("Query failed (%s)\n", mysql_error(mysql));
        return false;
    }
 
    //��ȡ�����  
    if (!(res = mysql_store_result(mysql)))   //���sql�������󷵻صĽ����  
    {
        return false;
    }
	int i = 0;
    while (column = mysql_fetch_row(res))   //����֪�ֶ���������£���ȡ����ӡ��һ��  
    {
		GradeList[i]=(column[0]);
		i++;
    }
	return true;
}

