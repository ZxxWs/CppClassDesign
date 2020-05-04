#define _CRT_SECURE_NO_WARNINGS
#pragma execution_character_set("UTF-8")//����qt�ı��룬���û�У����������������
#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"wsock32.lib")
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

//��������������������������������198����Ҫ�Ż�������������������������

using namespace std;

MYSQL* mysql = new MYSQL; //mysql����  
MYSQL_RES* res; //����ṹ�������е�һ����ѯ�����  
MYSQL_ROW column; //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾ�����е���  
char query[150]; //��ѯ���
string GradeList[9999];//�༶�б�
int GradeListLen = 0;
string NumList[100];//ѧ���б�
int NumListLen = 0;
Student stu;//չʾ��ѧ��
int AlterTag=0;//�޸İ�ť��״̬��0�������޸ģ�1�����޸�
int RowCount = 0;//��������
QStandardItemModel* dataModel = new QStandardItemModel();	//��������ģ��

//���溯��������
bool ConnectDatabase();
bool InitGrade();
bool InitNum(string GradeNum);
bool InitStudent(string GradeNum, string StudentNum);
bool AlterDetaile(string gradeNum, string stuNum, string AllDetail, double AllScore);
string ToSting(double d, int i);

Search::Search(QWidget* parent)//��ѯ����Ĺ��캯��
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.GradeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(GradeComboBoxChanged()));//�󶨿ؼ������ݱ仯����
	connect(ui.NumComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(NumComboBoxChanged()));
	connect(ui.AddButton, SIGNAL(clicked()), this, SLOT(ClickAddButton()));//��Ӱ�ť�͵�������İ�
	connect(ui.DelButton, SIGNAL(clicked()), this, SLOT(ClickDelButton()));
	connect(ui.AlterButton, SIGNAL(clicked()), this, SLOT(ClickAlterButton()));
	connect(ui.OutButton, SIGNAL(clicked()), this, SLOT(ClickOutButton()));
	//setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);//��Search���ڷŵ�Tz�����У��󶨣�

	//����ؼ��İ�
	this->GradeComboBox = ui.GradeComboBox;
	this->NumComboBox = ui.NumComboBox;
	this->LabNum = ui.LabNum;
	this->LabName = ui.LabName;
	this->LabGrade = ui.LabGrade;
	this->LabScore = ui.LabScore;
	this->LabRemake = ui.LabRemark;
	this->LabAlter = ui.LabAlter;
	this->ShowTable=ui.ShowTable;
	this->AddButton = ui.AddButton;
	this->DelButton = ui.DelButton;
	this->AlterButton = ui.AlterButton;
	this->OutButton = ui.OutButton;

	this->ShowTable->setModel(dataModel);	//������ģ��
	this->ShowTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//���ñ�����Զ���
	this->ShowTable->setEditTriggers(QAbstractItemView::NoEditTriggers);//���ñ��Ϊ���ɱ༭״̬
	
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

//����б�仯�¼�����
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

//ѧ���б��±仯������
void Search::NumComboBoxChanged()//ѧ�ź��б����ı�
{
	//LabAlter->setText("");//���label���޸�ѧ��ѧ��ϸ��� ��ʾlabel
	QString GraNum = ui.GradeComboBox->currentText();//��ȡ��ǰ�б��ֵ����һ��ͬ��
	QString StuNum = ui.NumComboBox->currentText();

	if (InitStudent(GraNum.toStdString(),StuNum.toStdString())) {

		LabNum->setText("ѧ�ţ�" + QString::fromStdString(stu.getSnum()));//����Ϣ��䵽������
		LabName->setText("������" + QString::fromStdString(stu.getSname()));
		LabScore->setText("�ܷ֣�" + QString::fromStdString(ToSting(stu.getSscore(),1)));
		LabRemake->setText("��ע��" + QString::fromStdString(stu.getSremark()));
		LabGrade->setText("�༶��" + QString::fromStdString(stu.getSgrade()));

		//ѧ��ϸ��������
		dataModel->clear();//�Ƚ�������
		RowCount = 0;
		dataModel->setHorizontalHeaderItem(0, new QStandardItem("��Ŀ"));//���ñ�ͷ
		dataModel->setHorizontalHeaderItem(1, new QStandardItem("ʱ��"));
		dataModel->setHorizontalHeaderItem(2, new QStandardItem("����"));
		for (int i = 0; i < 999; i++) {//ѭ�������
			if (stu.getSdetail()[i].getEvent() == "") {//��������յĻ���˵����ͷ�ˣ�������
				break;
			}
			dataModel->setItem(i, 0, new QStandardItem(QString::fromStdString(stu.getSdetail()[i].getEvent())));
			dataModel->setItem(i, 1, new QStandardItem(QString::fromStdString(stu.getSdetail()[i].getTime())));
			dataModel->setItem(i, 2, new QStandardItem(QString::fromStdString(ToSting(stu.getSdetail()[i].getScore(),1))));
			RowCount = i+1;
		}
		stu. ~Student();//�ͷ��ڴ棬������ͷŻᵽʱ�������ݼ̳��ϴε�
		stu=Student();
	}
	else
	{
		//�˴�����ѯʧ�ܴ���
	}
}

//��Ӱ�ť�¼�����
void Search::ClickAddButton() {//�������ӡ���ť�󣬴������Ϣ����

	AddInformation *a=new AddInformation();
	a->show();
}

//ɾ����ť�¼�����
void Search::ClickDelButton(){}

//�޸İ�ť�¼�����
void Search::ClickAlterButton(){
	
	if (AlterTag == 0) {//�ڲ����޸�״̬�µ��
		AlterTag = 1;
		//�Ȱ����������ؼ�����
		this->GradeComboBox->hide();
		this->NumComboBox->hide();
		this->AddButton->hide();
		this->DelButton->hide();
		this->AlterButton->setStyleSheet("background: red");
		this->OutButton->setStyleSheet("background: green");
		this->OutButton->setText("ȡ��");

		this->ShowTable->setEditTriggers(QAbstractItemView::AllEditTriggers);
		dataModel->setItem(RowCount, 0, new QStandardItem(QString::fromStdString("")));
		dataModel->setItem(RowCount, 1, new QStandardItem(QString::fromStdString("")));
		dataModel->setItem(RowCount, 2, new QStandardItem(QString::fromStdString("")));
	}
	else
	{
		AlterTag = 0;
		//����Ҫ��ʾ�����Ŀؼ���ʾ����
		this->GradeComboBox->show();
		this->NumComboBox->show();
		this->AddButton->show();
		this->DelButton->show();
		this->AlterButton->setStyleSheet("background: rgb(207, 207, 207)");
		this->OutButton->setStyleSheet("background: rgb(207, 207, 207)");
		this->OutButton->setText("�˳�");

		//���ڼ�����һ���Ƿ����
		if (dataModel->data(dataModel->index(RowCount, 0)).toString().toStdString() != "") {
			RowCount += 1;//���������һ�в�Ϊ�գ���������һ
		}
		else
		{
			dataModel->removeRow(RowCount);
		}

		string AllDetail = "";
		double AllScore = 0;
		string event, time, score;
		//������Ҫ���������⡣��Ҫ�����������ַ���1.�������ơ�2.�����쳣���
		for (int i = 0; i < RowCount; i++) {
			event=dataModel->data(dataModel->index(i,0)).toString().toStdString();
			time = dataModel->data(dataModel->index(i, 1)).toString().toStdString();
			score = dataModel->data(dataModel->index(i, 2)).toString().toStdString();
			//�������������û�пյ�
			if ((event.length()==0 || time.length() == 0)|| score.length() == 0) {//����п��ǿյ�
		
				NumComboBoxChanged();
				LabAlter->setText("�������ݲ�����Ϊ��");
				return;
				
			}
			AllDetail = AllDetail + "#" + event + "#" + time + "#" + score + "%";
			AllScore += atof(&score[0]);
		}

		//��ȡ��ǰ��ź�ѧ��
		string gradeNum = LabGrade->selectedText().toStdString();
		string stuNum = LabNum->selectedText().toStdString();
		
		//�������ݿ⣬�޸�ѧ��ϸ��
		if (AlterDetaile(gradeNum, stuNum, AllDetail, AllScore)) {
			LabAlter->setText("ѧ��ϸ���޸ĳɹ�");
		}
		else
		{
			LabAlter->setText("ѧ��ϸ���޸�ʧ�ܣ���������");
		}
		NumComboBoxChanged();//ˢ��ѧ��ϸ���
	}
}

//�˳���ť�¼�����
void Search::ClickOutButton() {//�������ӡ���ť�󣬴������Ϣ����

	if (AlterTag == 1) {//��ѧ��ϸ�������޸ĵ�ʱ����Ϊȡ����ťʹ�ã�
		AlterTag = 0;
		this->GradeComboBox->show();
		this->NumComboBox->show();
		this->AddButton->show();
		this->DelButton->show();
		this->AlterButton->setStyleSheet("background: rgb(207, 207, 207)");
		this->OutButton->setStyleSheet("background: rgb(207, 207, 207)");
		this->OutButton->setText("�˳�");
		NumComboBoxChanged();//ִ��һ��ѧ���б�ı亯��������ѧ��ϸ��
		this->ShowTable->setEditTriggers(QAbstractItemView::NoEditTriggers);//���ñ��Ϊ���ɱ༭״̬
	}
	else//��ѧ��ϸ��û���޸�״̬����Ϊ�˳���ťʹ��
	{




	}

}

//�������ݿ⺯��
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

//��Ų�ѯ����
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

//ѧ�Ų�ѯ����
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

//ѧ����ѯ����
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

//�޸�ѧ��ѧ��ϸ��
bool AlterDetaile(string gradeNum, string stuNum, string AllDetail, double AllScore);


//�Զ������ڽ�doubleת��string������iλС��
string ToSting(double d, int i) {
	string str = to_string(d);
	int tag = str.find(".");
	return str.substr(0, tag + 1 + i);
}

