#include "../Header/Student.h"
#include <iostream>
using namespace std;

Student::Student(int Snum, int Sgrade, string Sname, double Sscore, string Sdetail, string Sremark){
	this->Snum = Snum;
	this->Sgrade = Sgrade;
	this->Sname = Sname;
	this->Sscore = Sscore;
	setSdetail(Sdetail);
	this->Sremark = Sremark;
}
void Student::setSnum(int Snum){
	this->Snum = Snum;
}
void Student::setSgrade(int Sgrade){
	this->Sgrade = Sgrade;
}
void Student::setSname(string Sname){
	this->Sname = Sname;
}
void Student::setSscore(double Sscore){
	this->Sscore = Sscore;
}
void Student::setSdetail(string Sdetail){

}

void Student::setSremark(string Sremark){
	this->Sremark = Sremark;
}


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


