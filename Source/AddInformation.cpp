#include "../Header/AddInformation.h"
#include "../Header/Search.h"
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
string StringTag;//���ڼ�¼��ӹ��̵�����
string AddGradeList[9999];//�༶�б�


bool AddConnectDatabase();
bool InitGradeList();
bool InitStuList(string grade,string num);
bool InsertGrade(string GradeNum);
bool InsertStudent(string num, string name, string grade, string remark);

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
	connect(ui.SureButton, SIGNAL(clicked()), this, SLOT(ClickSureButton()));

	//��ʼ����ʱ�򣬿ؼ�������
	this->GradeListLable -> hide();
	this->NameLable -> hide();
	this->NumLabel ->hide();
	this->RemarkLable -> hide();
	this->GradeComboBox -> hide();
	this->RemarkTextEdit -> hide();
	this->NumLineEdit -> hide();
	this->NameLineEdit -> hide();
	this->SureButton->hide();
	this->NumLineEdit->setClearButtonEnabled(true);//�������������һ����հ�ť
	this->NameLineEdit->setClearButtonEnabled(true);
}

void AddInformation::ClickGradeButton(){//�����Ӱ༶��ť����¼�

	AddTag = 0;
	this->SureButton->show();
	this->NumLabel->show();
	this->NumLineEdit->show();
	this->GradeListLable->hide();
	this->NameLable->hide();
	this->RemarkLable->hide();
	this->GradeComboBox->hide();
	this->RemarkTextEdit->hide();
	this->NameLineEdit->hide();
	this->NumLabel->setText("������");
	this->NumLineEdit->setText("");
	this->TagLable->setText("");
	this->NumLineEdit->setMaxLength(9);//���ð���������󳤶�
}

void AddInformation::ClickStuButton() {
	AddTag = 1;
	this->SureButton->show();
	this->NumLabel->show();
	this->NumLineEdit->show();
	this->GradeListLable->show();
	this->NameLable->show();
	this->RemarkLable->show();
	this->GradeComboBox->show();
	this->RemarkTextEdit->show();
	this->NameLineEdit->show();
	this->NumLabel->setText("����ѧ��");
	this->NumLineEdit->setText("");
	this->NameLineEdit->setText("");
	this->RemarkTextEdit->append("");
	this->TagLable->setText("");
	this->NumLineEdit->setMaxLength(11);//����ѧ���������󳤶�
	//������б�
	if (AddConnectDatabase()) {//�������ݿ�
		if (InitGradeList()) {
			GradeComboBox->clear();//�Ƚ��б����
			for (int i = 0; i < AddGradeList->length(); i++) {//���༶�б�
				QString Item = QString::fromStdString(AddGradeList[i]);//��String����ת����Qstring����
				GradeComboBox->addItem(Item, Item);
			}
			label->setText(QString::fromStdString(AddGradeList[0]));
		}
	}
}

void AddInformation::ClickSureButton() {

	if (AddTag == 0) {//��Ӱ༶
		string GradeNum = this->NumLineEdit->displayText().toStdString();
		this->NumLineEdit;
		if (AddConnectDatabase()) {//�������ݿ�
			if (InitGradeList()) {
				for (int i = 0; i < AddGradeList->length(); i++) {//���༶�б�
					if (GradeNum == AddGradeList[i]) {//�������İ���Ѿ�����
						this->TagLable->setText("�˰���Ѿ�����");
						this->NumLineEdit->setText("");
						return;//ֱ�������������
					}
				}
				if (InsertGrade(GradeNum)) {
					TagLable->setText("��Ӱ༶�ɹ�");
				}else{
					TagLable->setText("��Ӱ༶ʧ��");
				}
			}
		}
	}
	else//���ѧ��
	{
		string StuNum = this->NumLineEdit->displayText().toStdString();
		string StuName = this->NameLineEdit->displayText().toStdString();
		string StuGrade = this->GradeComboBox->currentText().toStdString();
		string StuRemark = this->RemarkTextEdit->toPlainText().toStdString();

		if (StuNum == "" ) {
			TagLable->setText("������ѧ�ţ�ѧ�Ų���Ϊ��");
		}
		else if(StuName == "")
		{
			TagLable->setText("��������������������Ϊ��");
		}
		else
		{
			if (InitStuList(StuGrade, StuNum)) {
				if (InsertStudent(StuNum, StuName, StuGrade, StuRemark)) {
					TagLable->setText("ѧ����Ϣ��ӳɹ�");
				}
				else
				{
					TagLable->setText("ѧ����Ϣ���ʧ��");
					return;
				}
			}
			else {
				TagLable->setText("��ѧ���Ѵ��ڣ���ȷ�Ϻ��������롣");
				return;
			}
		}
	}

	this->SureButton->hide();	//�����������û������
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

//��ѯѧ��
bool InitStuList(string grade,string num) {

	string str = "select Snum from grade" + grade +" where Snum="+num;
	sprintf_s(AddQuery, &str[0]); //ִ�в�ѯ��䣬�����ǲ�ѯ����
	mysql_query(AddMysql, "set names utf8"); //���ñ����ʽ
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
		if (num == AddColumn[0]) {
			return false;
		}
		else
		{
			return true;
		}
	}
}

//����༶����  
bool InsertGrade(string GradeNum) {

	string str = "insert into gradelist (grade) values ('" +GradeNum+ "')";
	sprintf_s(AddQuery, &str[0]);
	if (mysql_query(AddMysql,AddQuery))        //ִ��SQL���  
	{
		return false;
	}
	else
	{
		str = "create table grade" + GradeNum + " like gradeempty;";//��ģ����и���һ�ű�����ʽ��������Ϊgrade+���
		sprintf_s(AddQuery, &str[0]);
		if (mysql_query(AddMysql, AddQuery))        //ִ��SQL���  
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

//����ѧ����Ϣ
bool InsertStudent(string num,string name,string grade,string remark) {

	string str = "insert into grade"+grade+" (Snum,Sname,Sgrade,Sremark) values ('" +num+"','"+name+"','"+grade+"','"+remark+"')";
	sprintf_s(AddQuery, &str[0]);
	if (mysql_query(AddMysql, AddQuery))        //ִ��SQL���  
	{
		return false;
	}
	else
	{
		return true;
	}
}
