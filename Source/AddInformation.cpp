#include "../Header/AddInformation.h"
#include "qpushbutton.h"
#include <QtWidgets/QMainWindow>
#include<iostream>
#include<Windows.h>
#include<WinSock.h>
#include<mysql.h>

using namespace std;

#pragma execution_character_set("UTF-8")//����qt�ı��룬���û�У����������������
#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"wsock32.lib")

MYSQL* AddMysql = new MYSQL; //mysql����  
MYSQL_RES* AddRes; //����ṹ�������е�һ����ѯ�����  
MYSQL_ROW AddColumn; //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾ�����е���  
char AddQuery[150]; //��ѯ���
int AddTag;//�����Tag��������Ӱ༶�������ѧ��(0:�༶��1��ѧ����
string TagString;//���ڼ�¼��ӹ��̵�����
string AddGradeList[9999];//�༶�б�


bool AddConnectDatabase();


AddInformation::AddInformation(QWidget* parent)//�����Ϣ����Ĺ��캯��
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->AddGradeButton = ui.AddGradeButton;
	this->AddStuButton = ui.AddStuButton;
	this->SureButton = ui.SureButton;
	this->GradeListLable = ui.GradeListLabel;
	this->NameLable = ui.NameLabel;
	this->NumLabel = ui.NumLabel;
	this->RemarkLable = ui.RemarkLabel;
	this->TagLable = ui.TagLabel;
	this->GradeComboBox = ui.GradeComboBox;
	this->RemarkTextEdit = ui.RemarkTextEdit;
	this->NumLineEdit = ui.NumLineEdit;
	this->NameLineEdit = ui.NameLineEdit;

	this->label=ui.label;

	//��Ӱ�ť�͵�������İ�
	connect(ui.AddGradeButton, SIGNAL(clicked()), this, SLOT(ClickGradeButton()));
	connect(ui.AddStuButton, SIGNAL(clicked()), this, SLOT(ClickStuButton()));

	this->SureButton->setFocusPolicy(Qt::NoFocus);//��ʼ����ʱ��ȷ����Ӱ�ť���ܵ��
	//��ʼ����ʱ�򣬿ؼ�������
	this->GradeListLable -> hide();
	this->NameLable -> hide();
	this->NumLabel ->hide();
	this->RemarkLable -> hide();
	this->TagLable -> hide();
	this->GradeComboBox -> hide();
	this->RemarkTextEdit -> hide();
	this->NumLineEdit -> hide();
	this->NameLineEdit -> hide();
}


void AddInformation::ClickGradeButton(){//�����Ӱ༶��ť����¼�

	AddTag = 0;
	this->SureButton->setFocusPolicy(Qt::ClickFocus);//�����Ӱ�ť�󣬽�ȷ�ϰ�ť����Ϊ���Ե��
	this->NumLabel->show();
	this->NumLineEdit->show();
	this->GradeListLable->hide();
	this->NameLable->hide();
	this->RemarkLable->hide();
	this->TagLable->hide();
	this->GradeComboBox->hide();
	this->RemarkTextEdit->hide();
	this->NameLineEdit->hide();
	this->NumLabel->setText("������");
}

void AddInformation::ClickStuButton() {
	AddTag = 1;
	this->SureButton->setFocusPolicy(Qt::ClickFocus);//�����Ӱ�ť�󣬽�ȷ�ϰ�ť����Ϊ���Ե��
	this->NumLabel->show();
	this->NumLineEdit->show();
	this->GradeListLable->show();
	this->NameLable->show();
	this->RemarkLable->show();
	this->TagLable->hide();
	this->GradeComboBox->show();
	this->RemarkTextEdit->show();
	this->NameLineEdit->show();
	this->NumLabel->setText("����ѧ��");

	if (AddConnectDatabase()) {//������б�
		GradeComboBox->clear();//�Ƚ��б����
		for (int i = 0; i < AddGradeList->length(); i++) {//���༶�б�
			QString Item = QString::fromStdString(AddGradeList[i]);//��String����ת����Qstring����
			GradeComboBox->addItem(Item, Item);
		}
		label->setText("sdsd");
	}


}

bool AddConnectDatabase() {
	//��ʼ��mysql  
	mysql_init(AddMysql);
	//����false������ʧ�ܣ�����true�����ӳɹ�  
	if (!(mysql_real_connect(AddMysql, "localhost", "root", "123456", "xfgl", 0, NULL, 0))) //�м�ֱ����������û��������룬���ݿ������˿ںţ�����дĬ��0����3306�ȣ���������д�ɲ����ٴ���ȥ  
	{
		return false;
	}
	else
	{
		return true;
	}
	return true;
}

bool InitGradeList() {

	sprintf_s(AddQuery, "select * from gradelist"); //ִ�в�ѯ��䣬�����ǲ�ѯ����
	mysql_query(AddMysql, "set names utf8"); //���ñ����ʽ��SET NAMES GBKҲ�У�������cmd����������  
	//����0 ��ѯ�ɹ�������1��ѯʧ��  
	if (mysql_query(AddMysql, AddQuery))    //ִ��SQL���
	{
		return false;
	}

	//��ȡ�����  
	if (!(AddRes = mysql_store_result(AddMysql)))   //���sql�������󷵻صĽ����  
	{
		return false;
	}

	for (int i = 0; AddColumn = mysql_fetch_row(AddRes); i++) {
		AddGradeList[i] = (AddColumn[0]);
	}
	return true;
}



