#pragma once
#include <iostream>
#include "Detail.h"
using namespace std;

//学生类。变量为
class Student
{
private:
	int Snum;//学生学号
	int Sgrade;//学生班级
	string Sname;//学生姓名
	double Sscore;//学生学分
	Detail Sdetail[999];//学生学分细则,上限999条。应该够用了吧
	string Sremark;//备注

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

