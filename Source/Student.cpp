#include "../Header/Student.h"

#include <iostream>
using namespace std;

Student::Student(int Snum, int Sgrade, string Sname, double Sscore, string Sdetail, string Sremark){}
void Student::setSnum(int Snum){}
void Student::setSgrade(int Sgrade){}
void Student::setSname(string Sname){}
void Student::setSscore(double Sscore){}
void Student::setSdetail(string Sdetail){}
void Student::setSremark(string Sremark){}


int Student::getSnum(){
	return this->Snum;
}
int Student::getSgrade(){
	return this->Sgrade;
}
string Student::getSname(){
	return this->Sname;
}
double Student::getSscore(){
	return this->Sscore;
}
Detail* Student::getSdetail(){
	return this->Sdetail;
}
string Student::setSremark(){
	return this->Sremark;
}


