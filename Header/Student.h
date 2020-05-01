#pragma once
#include <iostream>
#include "Detail.h"
using namespace std;

//ѧ���ࡣ����Ϊ
class Student
{
private:
	int Snum;//ѧ��ѧ��
	int Sgrade;//ѧ���༶
	string Sname;//ѧ������
	double Sscore;//ѧ��ѧ��
	Detail Sdetail[999];//ѧ��ѧ��ϸ��,����999����Ӧ�ù����˰�
	string Sremark;//��ע

public:

	Student(int Snum,int Sgrade,string Sname,double Sscore,string Sdetail, string Sremark);
	void setSnum(int Snum);
	void setSgrade(int Sgrade);
	void setSname(string Sname);
	void setSscore(double Sscore);
	void setSdetail(string Sdetail);
	void setSremark(string Sremark);

	int getSnum();
	int getSgrade();
	string getSname();
	double getSscore();
	Detail* getSdetail();
	string setSremark();
};

