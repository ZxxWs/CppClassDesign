#pragma once
#include <iostream>
#include "Detail.h"
using namespace std;

//ѧ���ࡣ����Ϊ
class Student
{
private:
	string Snum;//ѧ��ѧ��
	string Sgrade;//ѧ���༶
	string Sname;//ѧ������
	double Sscore;//ѧ��ѧ��
	Detail Sdetail[999];//ѧ��ѧ��ϸ��,����999����Ӧ�ù����˰�
	string Sremark;//��ע

public:

	Student();
	Student(string Snum, string Sgrade,string Sname,double Sscore,string Sdetail, string Sremark);
	void setSnum(string Snum);
	void setSgrade(string Sgrade);
	void setSname(string Sname);
	void setSscore(double Sscore);
	void setSdetail(string Sdetail);
	void setSremark(string Sremark);

	string getSnum();
	string getSgrade();
	string getSname();
	double getSscore();
	Detail* getSdetail();
	string getSremark();
};

