#include "../Header/Detail.h"

double TurnScore(string Score) {
	int front, back;
	front = (int)Score[0] - '0';
	back = (int)Score[2] - '0';
	double re = front + (0.1 * back);
	return re;
}

Detail::Detail() {
	//默认构造函数
	//如果没有默认构造函数，Studen类在初始化的时候会出错
}

Detail::Detail(string Sdetail) {

	int i = Sdetail.find("#");
	this->Event = Sdetail.substr(0,i);
	Sdetail = Sdetail.substr(i+1);
	i= Sdetail.find("#");
	this->Time = Sdetail.substr(0, i);
	Sdetail = Sdetail.substr(i + 1);
	this->Score = TurnScore(Sdetail);
}
string Detail::getEvent() {
	return this->Event;
}

string Detail::getTime() {
	return this->Time;
}

double Detail::getScore(){
	return this->Score;
}